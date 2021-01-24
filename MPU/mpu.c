/*******************************************************************************
	Copyright (C) 2018

	System Name	:	A1
	Filename	: 	mpu.c V1.0.0
	Date		:	2018-12-18   22:30

	History	:   	MPU PORT is not exsit in A1,so,we used GPIO
	1:  (2018-12-18 : Yjh)
*******************************************************************************/
#ifndef		__MPU_C__
#define		__MPU_C__

#ifdef  __cplusplus
extern  "C"{
#endif
/*********************************************START OF THE FILE********************************************************/
//#include <cmsis_armcc.h>
#include "global.h"
#include "mpu.h"

UINT32 g_timerSendCount = 0;
UINT32 g_timerReadCount = 0;

extern UINT32 g_timerMsCount;			/* ms timer counter */
extern UINT8  g_timerMsEn;

UINT32 received_speed;
UINT32 send_speed;

//static UINT8 test_flag=0;//关闭测试							//测试标志，默认打开

#define Gpio2OutData	0x4000e020
#define Gpio2OutEn		0x4000e024
#define Gpio2InData		0x4000e028
#define Gpio2Fun		0x4000e02c

/*!*****************************************************************************
\brief      往A2s写标准MIDI数据
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author
*******************************************************************************/
__asm void asm_Mpu_WriteData(R0)
{
    LDR R1, =Gpio2OutEn								//空间和数据端口设置为输出
    LDR R2,	=0xfff
    STR R2,	[R1]
	
		LDR R1, =Gpio2OutData						//DC=1,WR=1,RD=1,CS=0
    LDR R2,	=0xe00
    STR R2,	[R1]
	
		LDR R1, =Gpio2OutData						//DC=1,WR=0,RD=1,CS=0
    LDR R2,	=0xa00
    STR R2,	[R1]
	
    LDR R1, =Gpio2OutData							//发数据
    LDR R2,	=0xa00
    ADD	R2,	R2,	r0
    STR R2,	[R1] 
    
    LDR R1, =Gpio2OutData							//DC=1,WR=1,RD=1,CS=0
    LDR R2,	=0xe00
		ADD	R2,	R2,	r0
    STR R2,	[R1] 
	
    LDR R1, =Gpio2OutData							//DC=1,WR=1,RD=1,CS=1
    LDR R2,	=0xfff
    STR R2,	[R1]
    			
    BX LR
}


/*!*****************************************************************************
\brief      往A2s写控制命令
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author
*******************************************************************************/
__asm void asm_Mpu_WriteCmd(R0)
{
    LDR R1, =Gpio2OutEn								//空间和数据端口设置为输出
    LDR R2,	=0xfff
    STR R2,	[R1]
	
		LDR R1, =Gpio2OutData						//DC=0,WR=1,RD=1,CS=0
    LDR R2,	=0x600
    STR R2,	[R1]
	
		LDR R1, =Gpio2OutData						//DC=0,WR=0,RD=1,CS=0
    LDR R2,	=0x200
    STR R2,	[R1]
	
    LDR R1, =Gpio2OutData							//发数据
    LDR R2,	=0x200
    ADD	R2,	R2,	r0
    STR R2,	[R1] 
    
    LDR R1, =Gpio2OutData							//DC=0,WR=1,RD=1,CS=0
    LDR R2,	=0x600
		ADD	R2,	R2,	r0
    STR R2,	[R1] 
	
    LDR R1, =Gpio2OutData							//DC=1,WR=1,RD=1,CS=1
    LDR R2,	=0xfff
    STR R2,	[R1]
    			
    BX LR
}


/*!*****************************************************************************
\brief      读取A2s-MPU状态寄存器
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author
*******************************************************************************/
__asm UINT8 asm_Mpu_ReadStatus(void)
{
	
    LDR R1, =Gpio2OutEn
    LDR R2,	=0xf00					//控制脚为输出，数据口为输入
    STR R2,	[R1]
	
    LDR R1, =Gpio2OutData		
    LDR R2,	=0x700					//DC=0
    STR R2,	[R1] 
	
		LDR R1, =Gpio2OutData		
    LDR R2,	=0x600					//DC=0、CS=0
    STR R2,	[R1] 
	
		LDR R1, =Gpio2OutData		
    LDR R2,	=0x400					//DC=0、CS=0、RD=0
    STR R2,	[R1]  
	
    LDR R1, =Gpio2InData
    LDR R0,	[R1]  					//读数据
    
		LDR R1, =Gpio2OutData		
    LDR R2,	=0x700					//DC=0、CS=1、RD=1
    STR R2,	[R1]  
		
		LDR R1, =Gpio2OutData		
    LDR R2,	=0xf00					//DC=1、CS=1、RD=1
    STR R2,	[R1] 
		    			
    BX LR
}


/*!*****************************************************************************
\brief      从A2s读取数据
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author
*******************************************************************************/
__asm UINT8 asm_Mpu_ReadData(void)
{
    LDR R1, =Gpio2OutEn
    LDR R2,	=0xf00					//控制脚为输出，数据口为输入, D/C -- output
    STR R2,	[R1]
		
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xF00					//控制脚为1
    STR R2,	[R1] 
		
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xe00					//CS=0
    STR R2,	[R1] 
		
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xc00					//CS=0、RD=0
    STR R2,	[R1] 
			
    LDR R1, =Gpio2InData
    LDR R0,	[R1]
		    
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xe00					//CS=0、RD=1
    STR R2,	[R1]
		   
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xf00					//CS=1、RD=1
    STR R2,	[R1]  	
			
    BX LR
}

//__asm UINT16 asm_Mpu_ReadData(void)
//{
//		LDR R1, =Gpio2OutEn
//    LDR R2,	=0x700					//控制脚为输出，数据口为输入, D/C -- input
//    STR R2,	[R1]
//		
//    LDR R1, =Gpio2OutData		
//    LDR R2,	=0x700					//控制脚为1
//    STR R2,	[R1] 
//		
//		LDR R1, =Gpio2OutData		
//    LDR R2,	=0x600					//CS=0
//    STR R2,	[R1] 
//		
//		LDR R1, =Gpio2OutData		
//    LDR R2,	=0x400					//CS=0、RD=0
//    STR R2,	[R1] 
//			
//    LDR R1, =Gpio2InData
//    LDR R0,	[R1]
//		    
//    LDR R1, =Gpio2OutData		
//    LDR R2,	=0x600					//CS=0、RD=1
//    STR R2,	[R1]
//		   
//		LDR R1, =Gpio2OutData		
//    LDR R2,	=0xf00					//CS=1、RD=1
//    STR R2,	[R1]  	
//			
//    BX LR
//	
//}
//#pragma endasm






//UINT8	datastatus=0;
//UINT8	readstatus=0;
//UINT8 MpuReceiveData_loop(void)
//{
//	UINT8 data[64]={0};
//	UINT8 i,sendcnt;
//	
//	if(test_flag)
//	{	
//			readstatus = (UINT8)asm_Mpu_ReadStatus();
//			
//			if((readstatus&0x7f)<=0x3f)
//			{
//				if(readstatus&0x3f)
//				{
//						sendcnt = readstatus&0x3f;
//				}	
//				else
//				{
//						sendcnt = 64;					//当TX_FIFO_NUM为零，但TX_EMPTY不为空时，表示内部数据为64个（最大）。
//				}	
//				
//				for(i=0;i<sendcnt;i++)
//				{

//					data[i] = (UINT8)asm_Mpu_ReadData();

//					if(data[i] == 0x55 && datastatus==0  )
//					{
//						datastatus=1;
//					}
//					else if(data[i] == 0xaa && datastatus==1  )
//					{
//						datastatus=0;
//					}
//					else
//					{
//						test_flag=0;		//停止接收
//						return MPU_R_TEST_FALSE;
//					}
//					
//				}	
//				
//				g_timerMsEn = 1;
//				g_timerReadCount = g_timerReadCount + i;

//				#ifndef MPU_STABILITY_TEST
//				if(g_timerReadCount>(10*1024*1024))
//				{
//					received_speed = (g_timerReadCount/g_timerMsCount)*1000;
//					
//					g_timerMsEn = 0;
//					test_flag=0;		//停止接收
//				}		
//				#endif
//			}	
//			
//			return MPU_R_TEST_LOOP;
//	}
//	
//	return MPU_R_TEST_OVER;
//}




////发送测试 -- 发送速率测试，发送稳定性测试。
//void MpuSendData_loop(void)
//{
//		UINT8 i;
//				
//		UINT8 senddata[64] = {
//			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,	
//			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,
//			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,
//			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,
//			
//			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,
//			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,	
//			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,
//			0x55,0xaa,			0x55,0xaa,			0x55,0xaa,			0x55,0xaa
//			
//		};
//		
//		if(test_flag && ((0x80 & asm_Mpu_ReadStatus()) == 0x80))
//		{		
//			for(i=0;i<64;i++)
//			{		
//					#ifdef MPU_SENDDATA_TEST
//					asm_Mpu_WriteData(senddata[i]);		
//					#else
//					asm_Mpu_WriteCmd(senddata[i]);		
//					#endif
//			}	
//						
//			g_timerMsEn = 1;
//			g_timerSendCount = g_timerSendCount + i;	
//		}	
//		
//		#ifndef MPU_STABILITY_TEST
//		if(g_timerSendCount>(10*1024*1024) && test_flag==1)				//测试10M数据的速度
//		{
//			send_speed = (g_timerSendCount/g_timerMsCount)*1000;
//			
//			test_flag=0;
//			g_timerMsEn = 0;
//			
//			asm_Mpu_WriteData(0xf0);			//发一个其它码，触发A2S发出接收到的个数

//		}
//		#endif
//}

/*********************************************END OF THE FILE**********************************************************/
#ifdef  __cplusplus
}
#endif

#endif




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

//static UINT8 test_flag=0;//�رղ���							//���Ա�־��Ĭ�ϴ�

#define Gpio2OutData	0x4000e020
#define Gpio2OutEn		0x4000e024
#define Gpio2InData		0x4000e028
#define Gpio2Fun		0x4000e02c

/*!*****************************************************************************
\brief      ��A2sд��׼MIDI����
	
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
    LDR R1, =Gpio2OutEn								//�ռ�����ݶ˿�����Ϊ���
    LDR R2,	=0xfff
    STR R2,	[R1]
	
		LDR R1, =Gpio2OutData						//DC=1,WR=1,RD=1,CS=0
    LDR R2,	=0xe00
    STR R2,	[R1]
	
		LDR R1, =Gpio2OutData						//DC=1,WR=0,RD=1,CS=0
    LDR R2,	=0xa00
    STR R2,	[R1]
	
    LDR R1, =Gpio2OutData							//������
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
\brief      ��A2sд��������
	
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
    LDR R1, =Gpio2OutEn								//�ռ�����ݶ˿�����Ϊ���
    LDR R2,	=0xfff
    STR R2,	[R1]
	
		LDR R1, =Gpio2OutData						//DC=0,WR=1,RD=1,CS=0
    LDR R2,	=0x600
    STR R2,	[R1]
	
		LDR R1, =Gpio2OutData						//DC=0,WR=0,RD=1,CS=0
    LDR R2,	=0x200
    STR R2,	[R1]
	
    LDR R1, =Gpio2OutData							//������
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
\brief      ��ȡA2s-MPU״̬�Ĵ���
	
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
    LDR R2,	=0xf00					//���ƽ�Ϊ��������ݿ�Ϊ����
    STR R2,	[R1]
	
    LDR R1, =Gpio2OutData		
    LDR R2,	=0x700					//DC=0
    STR R2,	[R1] 
	
		LDR R1, =Gpio2OutData		
    LDR R2,	=0x600					//DC=0��CS=0
    STR R2,	[R1] 
	
		LDR R1, =Gpio2OutData		
    LDR R2,	=0x400					//DC=0��CS=0��RD=0
    STR R2,	[R1]  
	
    LDR R1, =Gpio2InData
    LDR R0,	[R1]  					//������
    
		LDR R1, =Gpio2OutData		
    LDR R2,	=0x700					//DC=0��CS=1��RD=1
    STR R2,	[R1]  
		
		LDR R1, =Gpio2OutData		
    LDR R2,	=0xf00					//DC=1��CS=1��RD=1
    STR R2,	[R1] 
		    			
    BX LR
}


/*!*****************************************************************************
\brief      ��A2s��ȡ����
	
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
    LDR R2,	=0xf00					//���ƽ�Ϊ��������ݿ�Ϊ����, D/C -- output
    STR R2,	[R1]
		
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xF00					//���ƽ�Ϊ1
    STR R2,	[R1] 
		
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xe00					//CS=0
    STR R2,	[R1] 
		
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xc00					//CS=0��RD=0
    STR R2,	[R1] 
			
    LDR R1, =Gpio2InData
    LDR R0,	[R1]
		    
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xe00					//CS=0��RD=1
    STR R2,	[R1]
		   
    LDR R1, =Gpio2OutData		
    LDR R2,	=0xf00					//CS=1��RD=1
    STR R2,	[R1]  	
			
    BX LR
}

//__asm UINT16 asm_Mpu_ReadData(void)
//{
//		LDR R1, =Gpio2OutEn
//    LDR R2,	=0x700					//���ƽ�Ϊ��������ݿ�Ϊ����, D/C -- input
//    STR R2,	[R1]
//		
//    LDR R1, =Gpio2OutData		
//    LDR R2,	=0x700					//���ƽ�Ϊ1
//    STR R2,	[R1] 
//		
//		LDR R1, =Gpio2OutData		
//    LDR R2,	=0x600					//CS=0
//    STR R2,	[R1] 
//		
//		LDR R1, =Gpio2OutData		
//    LDR R2,	=0x400					//CS=0��RD=0
//    STR R2,	[R1] 
//			
//    LDR R1, =Gpio2InData
//    LDR R0,	[R1]
//		    
//    LDR R1, =Gpio2OutData		
//    LDR R2,	=0x600					//CS=0��RD=1
//    STR R2,	[R1]
//		   
//		LDR R1, =Gpio2OutData		
//    LDR R2,	=0xf00					//CS=1��RD=1
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
//						sendcnt = 64;					//��TX_FIFO_NUMΪ�㣬��TX_EMPTY��Ϊ��ʱ����ʾ�ڲ�����Ϊ64������󣩡�
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
//						test_flag=0;		//ֹͣ����
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
//					test_flag=0;		//ֹͣ����
//				}		
//				#endif
//			}	
//			
//			return MPU_R_TEST_LOOP;
//	}
//	
//	return MPU_R_TEST_OVER;
//}




////���Ͳ��� -- �������ʲ��ԣ������ȶ��Բ��ԡ�
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
//		if(g_timerSendCount>(10*1024*1024) && test_flag==1)				//����10M���ݵ��ٶ�
//		{
//			send_speed = (g_timerSendCount/g_timerMsCount)*1000;
//			
//			test_flag=0;
//			g_timerMsEn = 0;
//			
//			asm_Mpu_WriteData(0xf0);			//��һ�������룬����A2S�������յ��ĸ���

//		}
//		#endif
//}

/*********************************************END OF THE FILE**********************************************************/
#ifdef  __cplusplus
}
#endif

#endif




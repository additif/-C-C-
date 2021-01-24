/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_MpuDecode_Struct.h
	Date		:	2018-11-27   17:08	

	History	:	
	1:  (2018-11-27 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_MPUDECODE_STRUCT_H__
#define __DRV_MPUDECODE_STRUCT_H__


/*********************************************START OF THE FILE********************************************************/
#include "global.h"
#include "Drv_MpuDecode_Enum.h"
#include "Drv_MpuDecode_Preference.h"

/*!*******************************************************************************************************************
\brief	MPU  Out Midi buffer结构	
*********************************************************************************************************************/
typedef struct
{
	BYTE  *psMpuOutMidiBuf;
    DWORD dwWr;         /*写指针*/
    DWORD dwRd;         /*读指针*/
    
}SDrv_MpuOutMidiBuf;


/*!*******************************************************************************************************************
\brief	MPU  Out Cmd buffer结构	
*********************************************************************************************************************/
typedef struct
{
	BYTE  *psMpuOutCmdBuf;
    DWORD dwWr;         /*写指针*/
    DWORD dwRd;         /*读指针*/
    
}SDrv_MpuOutCmdBuf;


/*!*******************************************************************************************************************
\brief	MPU  In  buffer结构	
*********************************************************************************************************************/
typedef struct
{
	UINT8  *psMpuInBuf;
    DWORD dwWr;         /*写指针*/
    DWORD dwRd;         /*读指针*/
    
}SDrv_MpuInBuf;


/*!*******************************************************************************************************************
\brief	MPU  In Cmd buffer结构	
*********************************************************************************************************************/
//typedef struct
//{
//	BYTE  *psMpuInCmdBuf;
//    DWORD dwWr;         /*写指针*/
//    DWORD dwRd;         /*读指针*/
//    
//}SDrv_MpuInCmdBuf;



typedef struct
{
	UINT8 CommandHeadFlag;//命令判别标志,遇到E0则置起
	UINT8 CmdDecodeFlag;//可以进行命令解码标志
	UINT8 MidiSendFlag;//Midi码可以发送标志

	UINT8 arrReceivedBuf[20];//用做保存接收到的一条命令的数据
	int	nReceivedDataLen;		//当前实际收到的命令数据长度
	int	nCmdLen;				//命令的实际长度

	UINT8 arrMidiReceiveBuf[IO_MIDI_FROM_A2S_SYSEX_MAX_LENGTH];//用于保存接收到的一个完整的MIDI事件
	int nMidiReceivedLen;//当前实际收到的MIDI数据的长度
	int nMidiLen;//MIDI的实际长度

	EMpuDecodeStatus eDecodeStatus;//当前解码的状态，是正在解码E7,midi,还是什么都没有
	EMpuMidiDecodeStatus eMidiStatus;//当前MIDI解码的状态
}SMpuCommonVar;

/*********************************************END OF THE FILE**********************************************************/
#endif





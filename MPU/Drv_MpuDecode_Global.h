/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_MpuDecode_Global.h
	Date		:	2018-11-27   17:08	

	History	:	
	1:  (2018-11-27 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_MPUDECODE_GLOBAL_H__
#define __DRV_MPUDECODE_GLOBAL_H__

/*********************************************START OF THE FILE********************************************************/
#include "global.h"

void Drv_MpuInitial(void);

//芯片握手与状态查询
void Drv_McuSlaveHandshake(void);
void Drv_McuStartupModeWait(void);
void Drv_McuStartupModeReply(void);
void Drv_McuQueryActive(void);
void Drv_A2sInitialSuccess(void);


//MPU OUT MIDI
UINT32 Drv_MpuOutMidiWr(UINT8 *datapt, UINT32 dataLen);
void Drv_MpuOutMidiBufProc(BYTE OutMidiData);
//MPU OUT COMMAND
UINT32 Drv_MpuOutCmdWr(UINT8 *datapt, UINT32 dataNumber);
void Drv_MpuOutCmdBufProc(BYTE OutCmdData);
//MPU IN
UINT32 Drv_MpuInWr(UINT8 *datapt, UINT32 dataNumber);
void Drv_MpuInBufProc(UINT8 InData);
BOOL MpuInBufRd(BYTE *pBuf, UINT32 uSize);

//MPU数据发送处理
void Drv_MpuWriteMidiData(void);
void Drv_MpuWriteCmdData(void);

//MPU解码处理
void Drv_ReceiveMpuData(void);
void Drv_MidiToUsbDecode(UINT8 *pdata, UINT16 num);
void Drv_MidiDataTransmit(UINT8 *pdata, UINT16 num);
void Drv_MpuInDataDecode(void);

//正常模式功能实现
void Drv_MpuSingleLedDecode(void);
void Drv_MpuMultiLedDecode(void);
void Drv_LedTimingLight(UINT16 Led_position, UINT16 Led_status, UINT16 LightingTime);
void Drv_LedFlicker(UINT16 PedalData, UINT16 Led_status, UINT16 LightingTime, UINT16 ExtinguishedTime);
void Drv_LedTimeProcess(void);
void Drv_MpuPedalAnswer(UINT32 PedelStatus);
void Drv_AuxInControl(void);
void Drv_AudioVolumeControl(void);
void Drv_UsbStatusToA2s(void);
void Drv_GlobalVersionToA2S(void);
/*********************************************END OF THE FILE**********************************************************/
#endif





/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_MpuDecode_Preference.h
	Date		:	2018-11-27   17:08	

	History	:	
	1:  (2018-11-27 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_MPUDECODE_PREFERENCE_H__
#define __DRV_MPUDECODE_PREFERENCE_H__


/*********************************************START OF THE FILE********************************************************/
#include "SysCfg_System_Preference.h"

#ifdef SYSCFG_DEBUG_MODE
	#define IO_MIDI_FROM_A2S_SYSEX_MAX_LENGTH			(SYSTEM_VELOCITY_CURVE_DOT_NUM + 50)	/* 可支持最长的系统外信息码，超过此长度的系统外信息码将被丢弃 */
#else
	#define IO_MIDI_FROM_A2S_SYSEX_MAX_LENGTH			(256)	/* 可支持最长的系统外信息码，超过此长度的系统外信息码将被丢弃 */
#endif

#define CMD_OUT_ASK					(0x00)
#define CMD_OUT_REPLY				(0x01)

#define CMD_IN_ASK					(0x00)
#define CMD_IN_REPLY				(0x01)

#define MPU_OUT_MIDI_BUF_SIZE       (8192)		/*MPU Out Buff的大小*/
#define MPU_OUT_CMD_BUF_SIZE        (8192)		/*MPU Out Buff的大小*/
#define MPU_IN_BUF_SIZE             (12288)		/*MPU In Buff的大小*/
#define MPU_IN_CMD_BUF_SIZE         (1024)		/*MPU In Buff的大小*/


/*********************************************END OF THE FILE**********************************************************/
#endif





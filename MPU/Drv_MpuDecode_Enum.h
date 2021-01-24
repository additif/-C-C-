/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_MpuDecode_Enum.h
	Date		:	2018-11-27   17:08	

	History	:	
	1:  (2018-11-27 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_MPUDECODE_ENUM_H__
#define __DRV_MPUDECODE_ENUM_H__

/*********************************************START OF THE FILE********************************************************/
typedef enum
{
	CmdHeadOne	= 0xE0,//命令头
	CmdHeadTwo	= 0xE7,
}EMpuCmdHead;

typedef enum
{
	MPU_CMD_TYPE_NONE,
	MPU_CMD_TYPE_E0E7,
	MPU_MCD_TYPE_MIDI,
}EMpuDecodeStatus;

typedef enum
{
	MPU_MIDI_NONE,//没有状态
	MPU_MIDI_BYTE,//固定byte长度的MIDI码
	MPU_MIDI_F0F7,//系统外信息码，F0开头F7结尾
}EMpuMidiDecodeStatus;


//切换MIDI组与数据发送目标命令
typedef enum
{
	Midi_Group0         = 0xB0,
	Midi_Group1         = 0xB1,
	Midi_Group2         = 0xB2,
	Midi_Group3         = 0xB3,
	Midi_GroupAll		= 0xB4,
//	Midi_B5		        = 0xB5,
//	Midi_B6		        = 0xB6,
//	Midi_B7		        = 0xB7,
	Midi_Local	        = 0xB8,
	Midi_Usb_Out        = 0xB9,
	Midi_Usb_Local_Out  = 0xBA,
//	Midi_BB		        = 0xBB,
//	Midi_BC		        = 0xBC,
//	Midi_BD		        = 0xBD,
//	Midi_BE		        = 0xBE,
//	Midi_BF		        = 0xBF,
}EMidiGroupId;


//芯片握手与状态查询
typedef enum
{
	HandShake_Cmd       = 0x00,
	Start_Mode_Cmd      = 0x01,
	Query_Active_Cmd    = 0x02,
    A2sInitial_Cmd      = 0x03,
	GlobalVersion_Cmd	= 0x04,
	USBConnectStatus_Cmd= 0x05,
	ConfigNum_Cmd		= 0x06,
}EMcuCmdId;

typedef enum
{
	Handshake_Ask	= 0x00,
	Handshake_Reply = 0x01,
}EHandshakeData;

typedef enum
{
	NormalMode		= 0x00,
	UpdateMode		= 0x01,
	SelfCheckMode	= 0x02,
	PcbaMode		= 0x03,

	StartMode_Ask	= 0x00,
	StartMode_Reply = 0x01,
}EStartModeData;

typedef enum
{
	QueryActive_Ask		= 0x00,
	QueryActive_Reply	= 0x01,
}EQueryActiveData;

typedef enum
{
    Initial_Data        = 0x01,
}EA2sInitialData;

typedef enum
{
	GlobalVersionl_Inquire	= 0x00,
	GlobalVersionl_Reply	= 0x01,
}EGlobalVersionl;


//正常功能实现
typedef enum
{
	Led_SingleCommand   = 0x10,
	//Pedal_Command     = 0x11,
    AuxIn_Command       = 0x14,
    Led_MultiCommand    = 0x15,
	AudioVolumeCommand	= 0x16,
}ENormalModeCmdId;

typedef enum
{
	Mpu_Pedal_StepOn	= 0x7F,
    Mpu_Pedal_Release	= 0x00,
}EMpuPedalMode;

typedef enum
{
	LedFlickerOn	= 0x55AA,
	LedFlickerOff	= 0xAA55,
}EMpuLedFlicker;

typedef enum
{
	LedBrightnessLow	= 0x00,
	LedBrightnessHigh	= 0x63,
}ELedBrightness;

typedef	enum
{
	AuxIn_Close = 0x00,
	AuxIn_Open	= 0x01,
}EAuxInControl;

typedef enum
{
	MaxVolume = 0x1000,
}EAudioVolume;

typedef enum
{
	EXPORT_SALE = 0x00,/*外销*/
	DOMESTIC_SALE = 0x01,/*内销*/
}ESaleType;


/*********************************************END OF THE FILE**********************************************************/
#endif





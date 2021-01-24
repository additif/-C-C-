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
\brief	MPU  Out Midi buffer�ṹ	
*********************************************************************************************************************/
typedef struct
{
	BYTE  *psMpuOutMidiBuf;
    DWORD dwWr;         /*дָ��*/
    DWORD dwRd;         /*��ָ��*/
    
}SDrv_MpuOutMidiBuf;


/*!*******************************************************************************************************************
\brief	MPU  Out Cmd buffer�ṹ	
*********************************************************************************************************************/
typedef struct
{
	BYTE  *psMpuOutCmdBuf;
    DWORD dwWr;         /*дָ��*/
    DWORD dwRd;         /*��ָ��*/
    
}SDrv_MpuOutCmdBuf;


/*!*******************************************************************************************************************
\brief	MPU  In  buffer�ṹ	
*********************************************************************************************************************/
typedef struct
{
	UINT8  *psMpuInBuf;
    DWORD dwWr;         /*дָ��*/
    DWORD dwRd;         /*��ָ��*/
    
}SDrv_MpuInBuf;


/*!*******************************************************************************************************************
\brief	MPU  In Cmd buffer�ṹ	
*********************************************************************************************************************/
//typedef struct
//{
//	BYTE  *psMpuInCmdBuf;
//    DWORD dwWr;         /*дָ��*/
//    DWORD dwRd;         /*��ָ��*/
//    
//}SDrv_MpuInCmdBuf;



typedef struct
{
	UINT8 CommandHeadFlag;//�����б��־,����E0������
	UINT8 CmdDecodeFlag;//���Խ�����������־
	UINT8 MidiSendFlag;//Midi����Է��ͱ�־

	UINT8 arrReceivedBuf[20];//����������յ���һ�����������
	int	nReceivedDataLen;		//��ǰʵ���յ����������ݳ���
	int	nCmdLen;				//�����ʵ�ʳ���

	UINT8 arrMidiReceiveBuf[IO_MIDI_FROM_A2S_SYSEX_MAX_LENGTH];//���ڱ�����յ���һ��������MIDI�¼�
	int nMidiReceivedLen;//��ǰʵ���յ���MIDI���ݵĳ���
	int nMidiLen;//MIDI��ʵ�ʳ���

	EMpuDecodeStatus eDecodeStatus;//��ǰ�����״̬�������ڽ���E7,midi,����ʲô��û��
	EMpuMidiDecodeStatus eMidiStatus;//��ǰMIDI�����״̬
}SMpuCommonVar;

/*********************************************END OF THE FILE**********************************************************/
#endif





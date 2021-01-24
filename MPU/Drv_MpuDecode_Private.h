/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_MpuDecode_Private.h
	Date		:	2018-11-27   17:08	

	History	:	
	1:  (2018-11-27 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_MPUDECODE_PRIVATE_H__
#define __DRV_MPUDECODE_PRIVATE_H__


/*********************************************START OF THE FILE********************************************************/
#include "global.h"
#include "Drv_MpuDecode_Struct.h"

extern  UINT8   A2sInitialSuccessFlag;
extern  UINT8	MidiLocalFlag;
extern  UINT8	MidiUsbOutFlag;
extern  UINT8	Group0Flag;
extern	UINT8	Group1Flag;
extern	UINT8	Group2Flag;
extern	UINT8	Group3Flag;

extern SMpuCommonVar g_sMpuCommonVar;//MPUģ�����ݵĹ�������

extern BOOL SaleType;/*���������֣�TRUE������FALSE����*/
extern BOOL ConfigNumReceiveFlag ;/*�Ƿ���ܵ����������ú�����*/

extern UINT32	ControlVolume;//The Value for A1 to Set Audio Volume Function 
extern BOOL AudioVloumeReceive;/*�Ƿ���ܵ�Audio��ʱ������*/
/*********************************************END OF THE FILE**********************************************************/
#endif





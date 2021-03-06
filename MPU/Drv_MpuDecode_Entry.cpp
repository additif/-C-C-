/*******************************************************************************
	Copyright (C) 2018

	System Name	:	A1
	Filename	: 	Drv_MpuDecode_Entry.c
	Date		:	2018-11-27   17:08

	History	:
	1:  (2018-11-27 : Yjh)
*******************************************************************************/
#ifndef __DRV_MPUDECODE_ENTRY_C__
#define __DRV_MPUDECODE_ENTRY_C__


/*********************************************START OF THE FILE********************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/*                                           External References Variables                                           */
/*-------------------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------------------------------------------*/
/*                                             External References Files                                             */
/*-------------------------------------------------------------------------------------------------------------------*/
#include "global.h"

#ifdef  __cplusplus
extern  "C"{
#endif
#include "buffer.h"
#ifdef  __cplusplus
}
#endif

#include "mpu.h"
#include "Drv_Led_Global.h"
#include "Drv_Led_Enum.h"
#include "Drv_Pedal_Global.h"
#include "UsbAudio.h"
#include "A2sCmdMod_Global.h"
#include "Drv_USB_Audio_Global.h"
#include "ProGlobalDataConfig.h"
/*-------------------------------------------------------------------------------------------------------------------*/
/*                                               Internal Head Files                                                 */
/*-------------------------------------------------------------------------------------------------------------------*/
#include "mpu.h"
#include "Drv_MpuDecode_GlobalInc.h"
/*-------------------------------------------------------------------------------------------------------------------*/
/*                                                 Internal C Files                                                  */
/*-------------------------------------------------------------------------------------------------------------------*/
#include "Drv_MpuDecode.cpp"
#include "Drv_MpuDataAnalysis.cpp"
#include "Drv_MpuDecode_Func.cpp"


/*********************************************END OF THE FILE**********************************************************/
#endif





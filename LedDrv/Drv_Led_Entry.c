/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_Led_Entry.c
	Date		:	2018-08-28   12:14	

	History	:   Edition 1:使用SPI中断发送数据(会被高优先级中断打断，不采用了)
                Edition 2:使用定时查询标志，SPI查询发送，LED数据改变则在while循环一次性发完所有数据
	1:  (2018-08-28 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_LED_ENTRY_C__
#define __DRV_LED_ENTRY_C__

#ifdef  __cplusplus
extern  "C"{
#endif
/*********************************************START OF THE FILE********************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/*                                           External References Variables                                           */
/*-------------------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------------------------------------------*/
/*                                             External References Files                                             */
/*-------------------------------------------------------------------------------------------------------------------*/
#include "GpioLib.h"
#include "global.h"
#include "SpiLib.h"
#include "uart.h"

#include "Drv_MpuDecode_GlobalInc.h"
/*-------------------------------------------------------------------------------------------------------------------*/
/*                                               Internal Head Files                                                 */
/*-------------------------------------------------------------------------------------------------------------------*/
#include "Drv_Led_GlobalInc.h"
#include "Drv_Led_Test.h"
/*-------------------------------------------------------------------------------------------------------------------*/
/*                                                 Internal C Files                                                  */
/*-------------------------------------------------------------------------------------------------------------------*/
#include "Drv_Led_Process.c"
#include "Drv_Led_Test.c"
#include "Drv_Led_BootEffect.c"


/*********************************************END OF THE FILE**********************************************************/
#ifdef  __cplusplus
}
#endif

#endif



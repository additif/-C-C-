/*******************************************************************************
	Copyright (C) 2018	
																				
	System Name	:	A1
	Filename	: 	Drv_Led_Global.h
	Date		:	2018-10-16   15:01	

	History	:	
	1:  (2018-10-16 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_LED_GLOBAL_H__
#define __DRV_LED_GLOBAL_H__

#ifdef  __cplusplus
extern  "C"{
#endif
/*********************************************START OF THE FILE********************************************************/
#include "global.h"
#include <stdbool.h>
#include "Drv_Led_Enum.h"

/*全局变量*/
//extern UINT8  g_Led_Change_Flag;                        /*Led的状态变化标志，1：已变动，0：无变动*/
//extern SDrv_SpiTxIrqInfo g_sLedSpiTxIrqInfo;
//extern BYTE g_arrbySPI_SendBuf[LED_SPI_SEND_BUF_SIZE];
//extern UINT8   g_Led_Change_Flag;

extern UINT32	g_BootUp;//开机灯效标志 1 == 未完成开机灯效,0 == 开机等效完成


void Drv_Led_Init(void);
void Drv_AllLedControl(ELedMapNum Led_status);
void Drv_Led_AllOn(void);
void Drv_Led_AllOff(void);
void Drv_Led_AllDark(void);
void Drv_Led_Com_Proc(void);
void Drv_Led_SPI_Data_Proc(void);
void Drv_Led_SPI_DeInit(void);
void Drv_Led_Test_Proc(void);
void Drv_Led_CmdWrite(ELedId Led_position, ELedMapNum Led_status);

void Drv_BootEffect_Proc(void);
/*********************************************END OF THE FILE**********************************************************/
#ifdef  __cplusplus
}
#endif

#endif


/*******************************************************************************
	Copyright (C) 2018	
																				
	System Name	:	A1
	Filename	: 	Drv_Led_Test.h
	Date		:	2018-11-19   14:51	

	History	:	
	1:  (2018-11-19 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_LED_TEST_H__
#define __DRV_LED_TEST_H__

#ifdef  __cplusplus
extern  "C"{
#endif

/*********************************************START OF THE FILE********************************************************/
#include "global.h"

//Tset Led
extern DWORD dwLed_Time_Num_One;
extern DWORD dwLed_Time_Num_Two;
extern DWORD dwLed_Test_Flag;


void Test_Led_prc(void);
void Test_Led_open(void);
void Test_Led_close(void);
void TestAllOn(void);
void TestAllOff(void);
/*********************************************END OF THE FILE**********************************************************/
#ifdef  __cplusplus
}
#endif

#endif





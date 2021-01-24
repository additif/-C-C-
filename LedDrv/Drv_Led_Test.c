/*******************************************************************************
	Copyright (C) 2018	
																				
	System Name	:	A1
	Filename	: 	Drv_Led_Test.c
	Date		:	2018-11-19   14:51	

	History	:	
	1:  (2018-11-19 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_LED_TEST_C__
#define __DRV_LED_TEST_C__

#ifdef  __cplusplus
extern  "C"{
#endif
/*********************************************START OF THE FILE********************************************************/
DWORD Test_Led_Num = 0;

DWORD dwLed_Time_Num_One = 0;
DWORD dwLed_Time_Num_Two = 0;
DWORD dwLed_Test_Flag = 0;

/*!*****************************************************************************
\brief  	LED测试模块
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/11/19 14:51
*******************************************************************************/
void Test_Led_prc(void)
{/*闪烁和循环流水测试，放在定时线程里面调用*/
    //Test Led
    dwLed_Time_Num_One++;
    if(dwLed_Test_Flag == 0)
    {
        if(dwLed_Time_Num_One == 15)/*等待500ms*/
        {
            TestAllOn();
            //Test_Led_open();
            dwLed_Time_Num_One = 0;
        }
    }
    else if(dwLed_Test_Flag == 1)
    {
        dwLed_Time_Num_Two++;
        if(dwLed_Time_Num_Two == 15)
        {
            TestAllOff();
            //Test_Led_close();
            
            dwLed_Time_Num_Two = 0;
        }
    }
    //Test Led End
    return;
}

void DelayUs(DWORD dwUs)
{
	DWORD i, j;
	
	for(j = 0; j < dwUs; j++)
	{
		for(i = 0; i < 5; i++);
	}
	return;
}

void Test_Led_open()
{
    #if 1
        Drv_Led_CmdWrite((ELedId)Test_Led_Num, Encode_Tab_16Level);
        Test_Led_Num++;
        if( Test_Led_Num >= LED_TOTAL_NUM)
        {
            Test_Led_Num = 0;
            dwLed_Test_Flag = 1;
        }

    #elif 0
        Drv_Led_CmdWrite((ELedId)Test_Led_Num, Encode_Tab_96Level);
        Test_Led_Num++;
        if( Test_Led_Num >= LED_TOTAL_NUM)
        {
            Test_Led_Num = 0;
            dwLed_Test_Flag = 1;
        }
    #endif
}

void Test_Led_close()
{
    Drv_Led_CmdWrite((ELedId)Test_Led_Num, Encode_Tab_0Level);
    Test_Led_Num++;
    if( Test_Led_Num >= LED_TOTAL_NUM)
    {
        Test_Led_Num = 0;
        
        dwLed_Time_Num_One = 0;
        dwLed_Test_Flag = 0;
    }
}

void TestAllOn()
{
    DWORD i;
    for (i = 0;i <= LED_TOTAL_NUM; i++)
    {
        Test_Led_Num++;
        Drv_Led_CmdWrite((ELedId)i, Encode_Tab_16Level);
    }
    if( Test_Led_Num >= LED_TOTAL_NUM)
        {
            Test_Led_Num = 0;
            dwLed_Test_Flag = 1;
        }
}

void TestAllOff()
{
    DWORD i;
    for (i = 0;i <= LED_TOTAL_NUM; i++)
    {
        Test_Led_Num++;
        Drv_Led_CmdWrite((ELedId)i, Encode_Tab_0Level);
    }
    if( Test_Led_Num >= LED_TOTAL_NUM)
    {
        Test_Led_Num = 0;
        
        dwLed_Time_Num_One = 0;
        dwLed_Test_Flag = 0;
    }
}


/*********************************************END OF THE FILE**********************************************************/
#ifdef  __cplusplus
}
#endif

#endif



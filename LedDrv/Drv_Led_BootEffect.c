/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_Led_BootEffect.c
	Date		:	2018-12-26   16:44	

	History	:	
	1:  (2018-12-26 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_LED_BOOTEFFECT_C__
#define __DRV_LED_BOOTEFFECT_C__

#ifdef  __cplusplus
extern  "C"{
#endif

/*********************************************START OF THE FILE********************************************************/
#define Led_Light_time		(0x60)

UINT32	g_BootUp = 1;
UINT32	g_LedBootSign = 1;
UINT32	g_LedQueue = 0;

UINT32  Open_step = 0;

UINT32  LED_ONE_OPEN_FLAG = 1;
UINT32	LED_TWO_OPEN_FLAG = 1;
UINT32	LED_THR_OPEN_FLAG = 1;
UINT32	LED_FOU_OPEN_FLAG = 1;

UINT32	g_arrLightTimeCount[LED_TOTAL_NUM];

/*!*****************************************************************************
\brief  	SP200开机效果
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/12/26 16:48
*******************************************************************************/
void Drv_BootEffect_Proc(void)
{
	UINT32	i;
    BYTE LedOpenOne[1] = 
	{
        //LED21,
        LED22,
	};

	BYTE LedOpenTwo[4] = 
	{
		LED13,
        LED14,
        LED15,
        LED16,
	};
	BYTE LedOpenThree[4] = 
	{
    	LED17,
        LED18,
        LED19,
        LED20,
	};
    BYTE LedOpenFour[3] =
    {
        LED1,       /*0x00是第1个灯的位置*/
        LED2,
        LED3,
    };
	BYTE LedOpenFive[9] = 
	{
        LED4,
    	LED5,
        LED11,
    	LED12,
    	LED6,
    	LED7,
    	LED8,
    	LED9,
    	LED10,
//    	LED13,
//    	LED14,
//    	LED15,
//    	LED16,
//    	LED17,
//    	LED18,
//    	LED19,
//    	LED20,
//    	LED21,
//    	LED22,
//    	LED23,
//    	LED24,
	};

	if(g_BootUp)
	{
		if(Open_step == 0)
		{
			if(LED_ONE_OPEN_FLAG)
			{
				for(i = 0; i < sizeof(LedOpenOne); i++)
				{
					Drv_Led_CmdWrite((ELedId)LedOpenOne[i], Encode_Tab_96Level);
					g_arrLightTimeCount[LedOpenOne[i]] = Led_Light_time;
				}
				LED_ONE_OPEN_FLAG = 0;
			}
			if(g_arrLightTimeCount[LedOpenOne[0]] != 0)
			{
				for(i = 0; i < sizeof(LedOpenOne); i++)
				{
					g_arrLightTimeCount[LedOpenOne[i]] = (g_arrLightTimeCount[LedOpenOne[i]] -1);
				}
			}
			if(g_arrLightTimeCount[LedOpenOne[0]] == 0)
			{
				for(i = 0; i < sizeof(LedOpenOne); i++)
				{
					Drv_Led_CmdWrite((ELedId)LedOpenOne[i], Encode_Tab_0Level);
				}
				Open_step = Open_step +1;
			}
		}
		if(Open_step == 1)
		{
			if(LED_TWO_OPEN_FLAG)
			{
				for(i = 0; i < sizeof(LedOpenTwo); i++)
				{
					Drv_Led_CmdWrite((ELedId)LedOpenTwo[i], Encode_Tab_96Level);
					g_arrLightTimeCount[LedOpenTwo[i]] = Led_Light_time;
				}
				LED_TWO_OPEN_FLAG = 0;
			}
			if(g_arrLightTimeCount[LedOpenTwo[3]] != 0)
			{
				for(i = 0; i < sizeof(LedOpenTwo); i++)
				{
					g_arrLightTimeCount[LedOpenTwo[i]] = (g_arrLightTimeCount[LedOpenTwo[i]] -1);
				}
				
			}
			if(g_arrLightTimeCount[LedOpenTwo[3]] == 0)
			{
				for(i = 0; i < sizeof(LedOpenTwo); i++)
				{
					Drv_Led_CmdWrite((ELedId)LedOpenTwo[i], Encode_Tab_0Level);
				}
				Open_step = Open_step +1;
			}
		}
		else if(Open_step == 2)
		{
			if(LED_THR_OPEN_FLAG)
			{
				for(i = 0; i < sizeof(LedOpenThree); i++)
				{
					Drv_Led_CmdWrite((ELedId)LedOpenThree[i], Encode_Tab_96Level);
					g_arrLightTimeCount[LedOpenThree[i]] = Led_Light_time;
				}
				LED_THR_OPEN_FLAG = 0;
			}
			if(g_arrLightTimeCount[LedOpenThree[3]] != 0)
			{
				for(i = 0; i < sizeof(LedOpenThree); i++)
				{
					g_arrLightTimeCount[LedOpenThree[i]] = (g_arrLightTimeCount[LedOpenThree[i]] -1);
				}
				
			}
			if(g_arrLightTimeCount[LedOpenThree[3]] == 0)
			{
				for(i = 0; i < sizeof(LedOpenThree); i++)
				{
					Drv_Led_CmdWrite((ELedId)LedOpenThree[i], Encode_Tab_0Level);
				}
				Open_step = Open_step +1;
			}
		}
        else if(Open_step == 3)
		{
			if(LED_FOU_OPEN_FLAG)
			{
				for(i = 0; i < sizeof(LedOpenFour); i++)
				{
					Drv_Led_CmdWrite((ELedId)LedOpenFour[i], Encode_Tab_96Level);
					g_arrLightTimeCount[LedOpenFour[i]] = Led_Light_time;
				}
				LED_FOU_OPEN_FLAG = 0;
			}
			if(g_arrLightTimeCount[LedOpenFour[2]] != 0)
			{
				for(i = 0; i < sizeof(LedOpenFour); i++)
				{
					g_arrLightTimeCount[LedOpenFour[i]] = (g_arrLightTimeCount[LedOpenFour[i]] -1);
				}
				
			}
			if(g_arrLightTimeCount[LedOpenFour[2]] == 0)
			{
				for(i = 0; i < sizeof(LedOpenFour); i++)
				{
					Drv_Led_CmdWrite((ELedId)LedOpenFour[i], Encode_Tab_0Level);
				}
				Open_step = Open_step +1;
			}
		}
		else if(Open_step == 4)
		{
			if(g_LedBootSign)
			{
                g_LedBootSign = 0;
                g_arrLightTimeCount[LedOpenFive[g_LedQueue]] = Led_Light_time;
                Drv_Led_CmdWrite((ELedId)LedOpenFive[g_LedQueue], Encode_Tab_96Level);
			}
			if(g_arrLightTimeCount[LedOpenFive[g_LedQueue]] != 0)
			{
				g_arrLightTimeCount[LedOpenFive[g_LedQueue]] = (g_arrLightTimeCount[LedOpenFive[g_LedQueue]] -1);

				if(g_arrLightTimeCount[LedOpenFive[g_LedQueue]] ==0)
				{
					Drv_Led_CmdWrite((ELedId)LedOpenFive[g_LedQueue], Encode_Tab_0Level);
					if(g_LedQueue < (sizeof(LedOpenFive)-1))
					{
						g_LedQueue = (g_LedQueue + 1);
						g_arrLightTimeCount[LedOpenFive[g_LedQueue]] = Led_Light_time;
						Drv_Led_CmdWrite((ELedId)LedOpenFive[g_LedQueue], Encode_Tab_96Level);
					}
					else if(g_LedQueue == (sizeof(LedOpenFive)-1))
					{
						Drv_Led_CmdWrite((ELedId)LedOpenFive[g_LedQueue], Encode_Tab_0Level);

						g_BootUp = 0;
					}
				}
			}
		}
	}
}


/*********************************************END OF THE FILE**********************************************************/
#ifdef  __cplusplus
}
#endif

#endif




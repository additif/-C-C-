/*******************************************************************************
	Copyright (C) 2018

	System Name	:	A1
	Filename	: 	Drv_MpuDecode_Func.c
	Date		:	2018-12-18   16:49

	History	:
	1:  (2018-12-18 : Yjh)
*******************************************************************************/
#ifndef __DRV_MPUDECODE_FUNC_C__
#define __DRV_MPUDECODE_FUNC_C__


/*********************************************START OF THE FILE********************************************************/
UINT8 	arrPedalStatusSave[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

UINT32	arrLightingTimeCount[LED_TOTAL_NUM];//LED亮时间的倒计时
UINT32	arrLightingTimeSave[LED_TOTAL_NUM];//保存上一次亮时间

UINT32	arrExtinguishedTimeCount[LED_TOTAL_NUM];//LED灭时间的倒计时
UINT32	arrExtinguishedTimeSave[LED_TOTAL_NUM];//保存上一次灭时间

UINT32	arrLastLedStatus[LED_TOTAL_NUM];//保存LED上一次或当前的状态

UINT32	arrFlickerFlag[LED_TOTAL_NUM];//LED用于闪烁标志位

BOOL    USBLastStatus = FALSE;

UINT32	ControlVolume;//The Value for A1 to Set Audio Volume Function 

/*!*****************************************************************************
\brief  	单功能LED控制命令解码

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks    单纯点亮或熄灭某一个LED
	\li

\author		Yjh 2018/12/08 11:38
*******************************************************************************/
void Drv_MpuSingleLedDecode(void)
{
	UINT8	Led_position;
	UINT8	Led_status;

	Led_position = g_sMpuCommonVar.arrReceivedBuf[1];

	if(Led_position != All_Led)
    {
        Led_position = (Led_position - 1);//第一个灯的序号是0x01，所以-1
    }
	//LED状态
	Led_status = g_sMpuCommonVar.arrReceivedBuf[2];

	if(Led_status >= Encode_Tab_248Level)
	{
		Led_status = Encode_Tab_248Level;
	}

	if(Led_position == All_Led)
	{
		Drv_AllLedControl((ELedMapNum)Led_status);
	}
	else
	{
		Drv_Led_CmdWrite((ELedId)(Led_position), (ELedMapNum)Led_status);
	}

}

/*!*****************************************************************************
\brief  	多功能LED控制命令解码

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks    可以控制LED定时点亮定时熄灭或亮灭循环
	\li

\author		Yjh 2018/12/08 11:38
*******************************************************************************/
void Drv_MpuMultiLedDecode(void)
{
	UINT8	Led_position;
	UINT8	Led_status;
	UINT8	LightingTime;
	UINT8	ExtinguishedTime;

	UINT32	i;

	Led_position = g_sMpuCommonVar.arrReceivedBuf[1];
    
    if(Led_position != All_Led)
    {
        Led_position = (Led_position - 1);//第一个灯的序号是0x01，所以-1
    }
    
	//LED状态
	Led_status = g_sMpuCommonVar.arrReceivedBuf[2];
	//LED点亮时间
	LightingTime = g_sMpuCommonVar.arrReceivedBuf[3];
	//LED熄灭时间
	ExtinguishedTime = g_sMpuCommonVar.arrReceivedBuf[4];

	if((LightingTime == 0 && ExtinguishedTime == 0) || (LightingTime == 0 && ExtinguishedTime != 0))
	{	
		if(Led_position == All_Led)
		{
			Drv_AllLedControl((ELedMapNum)Led_status);
			for(i = 0; i < LED_TOTAL_NUM; i++)
			{
				arrFlickerFlag[i] = LedFlickerOff;
			}

		}
		else
		{
			Drv_Led_CmdWrite((ELedId)(Led_position), (ELedMapNum)Led_status);

			arrFlickerFlag[Led_position] = LedFlickerOff;//关闭某一个灯的闪烁标志
		}
	}

	else if(LightingTime != 0 && ExtinguishedTime == 0)
	{
		Drv_LedTimingLight(Led_position, Led_status, LightingTime);
	}
	else if(LightingTime != 0 && ExtinguishedTime != 0)
	{
		Drv_LedFlicker(Led_position, Led_status, LightingTime, ExtinguishedTime);
	}
    
    return;
	
}

/*!*****************************************************************************
\brief  	点亮一定时间后熄灭（开始点亮和装填数据）

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/10 10:32
*******************************************************************************/
void Drv_LedTimingLight(UINT16 Led_position, UINT16 Led_status, UINT16 LightingTime)
{
	UINT32	i;

	if(Led_position == All_Led)
	{
		Drv_AllLedControl((ELedMapNum)Led_status);

		for(i = 0; i < LED_TOTAL_NUM; i++)
		{
			arrLightingTimeCount[i] = LightingTime;
			arrLightingTimeSave[i] = LightingTime;

			arrLastLedStatus[i] = Led_status;

			arrFlickerFlag[i] = LedFlickerOff;
		}
	}
	else
	{
		Drv_Led_CmdWrite((ELedId)Led_position, (ELedMapNum)Led_status);

		arrLightingTimeCount[Led_position] = LightingTime;
		arrLightingTimeSave[Led_position] = LightingTime;

		arrLastLedStatus[Led_position] = Led_status;

		arrFlickerFlag[Led_position] = LedFlickerOff;
	}
	
    return;

}

/*!*****************************************************************************
\brief  	循环闪烁（开始点亮和装填数据）

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/10 10:32
*******************************************************************************/
void Drv_LedFlicker(UINT16 Led_position, UINT16 Led_status, UINT16 LightingTime, UINT16 ExtinguishedTime)
{
	UINT32	i;

	if(Led_position == All_Led)
	{
		Drv_AllLedControl((ELedMapNum)Led_status);

		for(i = 0; i < LED_TOTAL_NUM; i++)
		{
			arrLightingTimeCount[i] = LightingTime;
			arrLightingTimeSave[i] = LightingTime;

			arrExtinguishedTimeCount[i] = ExtinguishedTime;
			arrExtinguishedTimeSave[i] = ExtinguishedTime;

			arrLastLedStatus[i] = Led_status;

			arrFlickerFlag[i] = LedFlickerOn;
		}
	}
	else
	{
		Drv_Led_CmdWrite((ELedId)Led_position, (ELedMapNum)Led_status);

		arrLightingTimeCount[Led_position] = (LightingTime);
		arrLightingTimeSave[Led_position] = (LightingTime);

		arrExtinguishedTimeCount[Led_position] = (ExtinguishedTime);
		arrExtinguishedTimeSave[Led_position] = (ExtinguishedTime);

		arrLastLedStatus[Led_position] = Led_status;

		arrFlickerFlag[Led_position] = LedFlickerOn;
	}
    
    return;
}

/*!*****************************************************************************
\brief  	LED亮灭定时进程

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks	放在10ms定时处理处调用
	\li

\author		Yjh 2018/12/10 10:32
*******************************************************************************/
void Drv_LedTimeProcess(void)
{
	UINT32 i;
	for(i = 0; i < LED_TOTAL_NUM; i++)
	{
		if(arrFlickerFlag[i] == LedFlickerOff)
		{
			if(arrLightingTimeCount[i] != 0)
			{
				arrLightingTimeCount[i] = (arrLightingTimeCount[i] - 1);
				if(arrLightingTimeCount[i] == 0)
				{
					Drv_Led_CmdWrite((ELedId)(i), Encode_Tab_0Level);
				}
			}
		}
		else if(arrFlickerFlag[i] == LedFlickerOn)
		{
			if(arrLightingTimeCount[i] != 0)
			{
				arrLightingTimeCount[i] = (arrLightingTimeCount[i] - 1);
				if(arrLightingTimeCount[i] == 0)
				{
					Drv_Led_CmdWrite((ELedId)(i), Encode_Tab_0Level);
					arrExtinguishedTimeCount[i] = arrExtinguishedTimeSave[i];
				}
			}
			if(arrLightingTimeCount[i] == 0)
			{
				if(arrExtinguishedTimeCount[i] != 0)
				{
					arrExtinguishedTimeCount[i] = (arrExtinguishedTimeCount[i] -1);
					if(arrExtinguishedTimeCount[i] == 0)
					{
						Drv_Led_CmdWrite((ELedId)(i), (ELedMapNum)arrLastLedStatus[i]);
						arrLightingTimeCount[i] = arrLightingTimeSave[i];
					}
				}
			}
		}
	}
    
    return;
}

/*!*****************************************************************************
\brief  	踏板状态返回A2s

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/08 11:38
*******************************************************************************/
void Drv_MpuPedalAnswer(UINT32 PedelStatus)
{
    UINT32	i,j;
	UINT32	Pedel_Single_Status;

	//踏板状态应答(初始化)
	UINT8	arrPedalReply[6]        = {0xE0, 0xE7, 0x03, 0x11};

	for(i = 0; i < PEDAL_TOTAL_NUM; i++)
	{
		Pedel_Single_Status = (PedelStatus >> i);

		if((Pedel_Single_Status & 0x01) == 0x00)
		{//踏板踩下
			if(arrPedalStatusSave[i] != 0x7F)//不等即数据发生改变
			{
				arrPedalStatusSave[i] = 0x7F;
				arrPedalReply[4] = ((HALF_PEDAL + 1) + i);
				arrPedalReply[5] = Mpu_Pedal_StepOn;

				for(j = 0; j < 6; j++)
				{
					Drv_MpuOutCmdBufProc(arrPedalReply[j]);
				}
			}
		}
		else if((Pedel_Single_Status & 0x01) == 0x01)
		{//踏板没有踩下
			if(arrPedalStatusSave[i] != 0x00)//不等即数据发生改变
			{
				arrPedalStatusSave[i] = 0x00;
				arrPedalReply[4] = ((HALF_PEDAL + 1) + i);
				arrPedalReply[5] = Mpu_Pedal_Release;

				for(j = 0; j < 6; j++)
				{
					Drv_MpuOutCmdBufProc(arrPedalReply[j]);
				}
			}
		}
	}
    return;
}


/*!*****************************************************************************
\brief  	AUX IN开关函数
\param[in]	none
\param[out]	none
\return     none
\retval		none
\remarks
	\li
\author		Yjh 2019/2/18 15:25
*******************************************************************************/
void Drv_AuxInControl(void)
{
	//打开AUX IN
	UINT8   AuxInControl[18] = 
	{
	    0xb0,0x63,0x34,/*I2S 通道0 Left IN音量*/
	    0xb0,0x62,0x00,
	    0xb0,0x06,0x00,
	
	    0xb0,0x63,0x34,/*I2S 通道0 Right IN音量*/
	    0xb0,0x62,0x01,
	    0xb0,0x06,0x00,
	};

    AuxInControl[8] = g_sMpuCommonVar.arrReceivedBuf[1];
    AuxInControl[17] = g_sMpuCommonVar.arrReceivedBuf[1];
    
    MidiFifoIn(AuxInControl, sizeof(AuxInControl));//控制AUX IN

	return;
}


/*!*****************************************************************************
\brief  	A1 USB Audio Volume Control Commands
\param[in]	none
\param[out]	none
\return     none
\retval		none
\remarks
	\li
\author		Yjh 2019/04/12 16:18
*******************************************************************************/
void Drv_AudioVolumeControl(void)
{
	UINT8	CommandVolume;//A2S Send to A1 Volume Value:0~7FH

	CommandVolume = (g_sMpuCommonVar.arrReceivedBuf[1] & 0x7F);

	if(CommandVolume == 0x7F)
	{
		ControlVolume = MaxVolume;
	}
	else
	{
		ControlVolume = (CommandVolume * 32);
	}

	SetSpeakerVolume_A1(ControlVolume);/*Set Speaker Volume Function*/

	AudioVloumeReceive = TRUE;

	return;
}


/*!*****************************************************************************
\brief  	A1 USB Connect Status to A2S
\param[in]	none
\param[out]	none
\return     none
\retval		none
\remarks
	\li
\author		Yjh 2019/04/14 10:18
*******************************************************************************/
void Drv_UsbStatusToA2s(void)
{
	BOOL	UsbNowStatus;
	UINT8	arrUsbConnect[5] = {0xE0, 0xE7, 0x02, 0x05, 0x01};/*USB Connect to PC*/
	UINT8	arrUsbNotConnect[5] = {0xE0, 0xE7, 0x02, 0x05, 0x00};/*USB Not Connect to PC*/

	UsbNowStatus = GetUSBStatus();

	if(UsbNowStatus != USBLastStatus)
	{
		USBLastStatus = UsbNowStatus;
		
		if(UsbNowStatus == TRUE)
		{
			Drv_MpuOutCmdWr(arrUsbConnect, 5);
		}
		else if(UsbNowStatus == FALSE)
		{
			Drv_MpuOutCmdWr(arrUsbNotConnect, 5);
		}
	}

	return;
}

/*!*****************************************************************************
\brief  	版本号查询命令
\param[in]	none
\param[out]	none
\return     none
\retval		none
\remarks    A1 GlobalVersion to A2S
	\li
\author		Yjh 2019/04/28 10:18
*******************************************************************************/
void Drv_GlobalVersionToA2S(void)
{
	UINT8	arrGlobalVersionReply[8] = {0xE0, 0xE7, 0x05, 0x04, 0x01};/*版本号查询命令头*/

	if(g_sMpuCommonVar.arrReceivedBuf[1] == GlobalVersionl_Inquire)
	{
		arrGlobalVersionReply[5] = (g_arrGlobalVersion[1] & 0x0F);
		arrGlobalVersionReply[6] = ((g_arrGlobalVersion[0] & 0xF0) >> 4);
        arrGlobalVersionReply[7] = (g_arrGlobalVersion[0] & 0x0F);
        
        //测试代码，先固定发V1.0.0
//        arrGlobalVersionReply[5] = 0x01;
//		arrGlobalVersionReply[6] = 0x00;
//        arrGlobalVersionReply[7] = 0x00;

		Drv_MpuOutCmdWr(arrGlobalVersionReply, 8);
	}
	return;
}

/*********************************************END OF THE FILE**********************************************************/
#endif



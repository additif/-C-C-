/*!**************************************************************************************************
\copyright Copyright (C) 2019	
\defgroup       
\file           Drv_DigitalTube.cpp
\brief          
\history:
\version        1.0
\author         YJH
\date           19/11/25 12:09
\remarks        No Remarks
****************************************************************************************************/
#pragma once

/**************************************START OF THE FILE****************************************************************/

/*!*******************************************************************************************************************
\brief	共阳数码管显示字模，根据需要添加，要与显示枚举相对应 	
*********************************************************************************************************************/
const UINT8 Nixie_COMMON_H_TAB[] = {
									0xC0, 0xF9, 0xA4,
								 	0xB0, 0x99, 0x92,
									0x82, 0xF8, 0x80,
									0x90, 0x88, 0x83,
									0xC6, 0xA1, 0x86,
									0x8E, 0x8C, 0xC7,

									0x89, 0xC1, 0xff, 
									0xBF, 0xC7, 0xf0};


/*!*******************************************************************************************************************
\brief	共阴数码管显示字模，根据需要添加，要与显示枚举相对应 	
*********************************************************************************************************************/
const UINT8 Nixie_COMMON_L_TAB[] = {
									0x3F, 0x06, 0x5B,
									0x4F, 0x66, 0x6D,
									0x7D, 0x07, 0x7F,
									0x6F, 0x77, 0x7C,
									0x39, 0x5E, 0x79,
									0x71, 0x73, 0x38,

									0x76, 0x3E, 0x00,
									0x40, 0x38, 0x0F};

/*
共阳极数组0~9：display[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

共阳极数组A~F：display[]={0x88,0x83,0xC6,0xA1,0x86,0x8E}

共阴极数组0~9：display[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F},

共阴极数组A~F：display[]={0x77,0x7C,0x39,0x5E,0x79,0x71};

*/

static CDrvDigitalTube * s_pcSingletonObj = NULL;
/*!**************************************************************************************************
\brief          
\param[in]      No param_in
\param[out]     No param_out
\return         No return
****************************************************************************************************/
CDrvDigitalTube * CDrvDigitalTube::GetInstance()
{
	if (NULL == s_pcSingletonObj)
	{
		s_pcSingletonObj = new CDrvDigitalTube;
	}

	return s_pcSingletonObj;
}

/*!**************************************************************************************************
\brief          new DigitalTube
\param[in]      No param_in
\param[out]     No param_out
\return         No return
****************************************************************************************************/
CDrvDigitalTube::CDrvDigitalTube()
{
    DWORD i;

	//LB0~LB6
    for(i = GPIO0;i <= GPIO6;i++)
	{
		GpioFunSet(GPIO_PORT1, GPIO_FUN1, (eGpioNumber)i);
	}
	//ROWT5~ROWT7
	for(i = GPIO13;i <= GPIO15;i++)
	{
		GpioFunSet(GPIO_PORT2, GPIO_FUN1, (eGpioNumber)i);
	}

	//set gpio
	//HAL_GPIO_FuncSetMulti(IO_NIXIE_CTRL_PORT, HAL_GPIO_1ST, IO_NIXIE_CTRL_BITS);
	//HAL_GPIO_FuncSetMulti(IO_NIXIE_SWITCH_PORT, HAL_GPIO_1ST, IO_NIXIE_SWITCH_BITS);

	GpioOutSet(GPIO_PORT1, IO_NIXIE_CTRL_BITS);
	GpioOutSet(GPIO_PORT2, IO_NIXIE_SWITCH_BITS);

	g_sNixieVar.boSwitch = TRUE;
	g_sNixieVar.IFRefresh = TRUE;//FALSE;
	g_sNixieVar.boCommonAnode = TRUE;//共阳极
	g_sNixieVar.NixieTubeLEDStatus[0] = E_DISP_SUB;
	g_sNixieVar.NixieTubeLEDStatus[1] = E_DISP_SUB;
	g_sNixieVar.NixieTubeLEDStatus[2] = E_DISP_SUB;
}


/*!**************************************************************************************************
\brief          delete DigitalTube
\param[in]      No param_in
\param[out]     No param_out
\return         No return
****************************************************************************************************/
CDrvDigitalTube::~CDrvDigitalTube()
{

}

/*!**************************************************************************************************
\brief          数码管数据刷新接口
\param[in]      No param_in
\param[out]     No param_out
\return         No return
****************************************************************************************************/
void NixieDisplayInterface(ENixieType eType, ENixieTubeDisplay *psDisplayData, UINT8 uDataNum)
{
	s_pcSingletonObj->NixieDisplay(eType, psDisplayData, uDataNum);
}
/*!*************************************************************************************************
\brief          NixieDisplay	数码管数据刷新接口
\param[in]      eType：显示类型
\param[in]      psDisplayData：要显示的数据
\param[in]      uDataNum：显示数据的个数
\param[out]     No out param
\return         TRUE:display success FALSE:display fail
\retval         No retval
\remarks        No remarks
\warning        No warning
\par            Exmaple Code
\code           No code
\endcode        
***************************************************************************************************/
BOOL CDrvDigitalTube::NixieDisplay(ENixieType eType, ENixieTubeDisplay *psDisplayData, UINT8 uDataNum)
{
	RET_VAL_IF_true((uDataNum > 3) , FALSE);//大于3个字符则不显示

    BOOL boRet;
	DWORD i;
	ENixieTubeDisplay eTempDisplay[3];

	for(i = 0; i < 3; i++)
	{
		if(E_DISP_NUM_LETTER == eType)
		{
			eTempDisplay[i] = *psDisplayData++;
		}

		RefreshNixieDisplay((ENixieTubePosition)i, eTempDisplay[i]);

	}

	boRet = TRUE;

    return boRet;
}

/*!**************************************************************************************************
\brief          刷新每一个位的数据
\param[in]      No param_in
\param[out]     No param_out
\return         No return
****************************************************************************************************/
void CDrvDigitalTube::RefreshNixieDisplay(ENixieTubePosition ePos, ENixieTubeDisplay eDisplay)
{
	switch(ePos)
	{
	case E_POS_ONE:
		{
			g_sNixieVar.NixieTubeLEDStatus[0] = eDisplay;

			g_sNixieVar.IFRefresh = TRUE;
		}break;
	case E_POS_TWO:
		{
			g_sNixieVar.NixieTubeLEDStatus[1] = eDisplay;

			g_sNixieVar.IFRefresh = TRUE;
		}break;
	case E_POS_THR:
		{
			g_sNixieVar.NixieTubeLEDStatus[2] = eDisplay;

			g_sNixieVar.IFRefresh = TRUE;
		}break;
	}
}




/*!*************************************************************************************************
\brief          数码管开关控制
\param[in]      boSwitch -> TRUE:开启数码管  FALSE:关闭数码管
\param[out]     No out param
\return         No return
\retval         No retval
\remarks        No remarks
\warning        No warning
\par            Exmaple Code
\code           No code
\endcode        
***************************************************************************************************/
void CDrvDigitalTube::NixieLEDSwitchOff(BOOL boSwitch)
{
	g_sNixieVar.boSwitch = boSwitch;

}

/*!*************************************************************************************************
\brief          NixieLEDProcess 放在线程中或毫秒服务中调用，刷新数码管
\param[in]      No in param
\param[out]     No out param
\return         No return
\retval         No retval
\remarks        No remarks
\warning        No warning
\par            Exmaple Code
\code           No code
\endcode        
***************************************************************************************************/
void CDrvDigitalTube::NixieLEDProcess()
{
	UINT32 NixieSwitchPortSet = 0;
	UINT32 NixieDataPortSet = 0;
	static UINT8 LedSwitchOn = 1;

	LedSwitchOn = (LedSwitchOn + 1) % NIXIE_TOTAL_NUM;

	if(g_sNixieVar.boSwitch)
	{
		if(g_sNixieVar.IFRefresh)
		{
			if(g_sNixieVar.boCommonAnode)
			{//共阳极
				//设置显示数据
				NixieDataPortSet = Nixie_COMMON_H_TAB[g_sNixieVar.NixieTubeLEDStatus[LedSwitchOn]];

				//DWORD dwPortOutVal = GpioOutDataGet((eGpioPortNumber)IO_NIXIE_CTRL_PORT);
				//dwPortOutVal = (NixieDataPortSet & IO_NIXIE_CTRL_BITS) | (dwPortOutVal & (~IO_NIXIE_CTRL_BITS));

				HAL_GPIO_WrPort(IO_NIXIE_CTRL_PORT, NixieDataPortSet, IO_NIXIE_CTRL_BITS);

				//打开对应的数码管
				NixieSwitchPortSet = 1 << (13 + LedSwitchOn);
				HAL_GPIO_WrPort(IO_NIXIE_SWITCH_PORT, NixieSwitchPortSet, IO_NIXIE_SWITCH_BITS);

				//g_sNixieVar.IFRefresh = FALSE;
			}
			else
			{//共阴极
				//设置显示数据
				NixieDataPortSet = Nixie_COMMON_L_TAB[g_sNixieVar.NixieTubeLEDStatus[LedSwitchOn]];
				HAL_GPIO_WrPort(IO_NIXIE_CTRL_PORT, NixieDataPortSet, IO_NIXIE_CTRL_BITS);

				//打开对应的数码管
				NixieSwitchPortSet = ~(1 << (13 + LedSwitchOn));
                //NixieSwitchPortSet = (NixieSwitchPortSet & IO_NIXIE_SWITCH_BITS);
				HAL_GPIO_WrPort(IO_NIXIE_SWITCH_PORT, NixieSwitchPortSet, IO_NIXIE_SWITCH_BITS);

				//g_sNixieVar.IFRefresh = FALSE;
			}
		}
	}
}
/****************************************END OF THE FILE***************************************************************/




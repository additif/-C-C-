/*!**************************************************************************************************
\copyright Copyright (C) 2019		
\defgroup       
\file           Drv_DigitalTube_Public.h
\brief          
\history:
\version        1.0
\author         YJH
\date           19/11/25 12:09
\remarks        No Remarks
****************************************************************************************************/
#pragma once

/**************************************START OF THE FILE****************************************************************/

#include "../User/Driver/DigitalTube/Drv_DigitalTube_Preference.h"
#include "../User/Driver/DigitalTube/Drv_DigitalTube_Private.h"

class CDrvDigitalTube
{

public:
    static CDrvDigitalTube *GetInstance();//����ģʽ

    BOOL NixieDisplay(ENixieType eType, ENixieTubeDisplay *psDisplayData, UINT8 uDataNum);

    void NixieLEDProcess();
    
    void NixieLEDSwitchOff(BOOL boSwitch);
private:
    
    CDrvDigitalTube();
    ~CDrvDigitalTube();

    void RefreshNixieDisplay(ENixieTubePosition ePos, ENixieTubeDisplay eDisplay);

    SNixieTubeCommonVar g_sNixieVar;//��������ݽṹ��
protected:


};

/****************************************END OF THE FILE***************************************************************/




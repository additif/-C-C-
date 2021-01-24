/*!**************************************************************************************************
\copyright Copyright (C) 2019		
\defgroup       
\file           Drv_DigitalTube_Preference.h
\brief          
\history:
\version        1.0
\author         YJH
\date           19/11/25 12:09
\remarks        No Remarks
****************************************************************************************************/
#pragma once

/**************************************START OF THE FILE****************************************************************/

#define NIXIE_TOTAL_NUM				(3)         /*!< ����ܵ�λ�� */

//#define NIXIE_ROW					(NIXIE_TOTAL_NUM)         /*!< ����ܵ�λѡ */
//#define NIXIE_LINE					(NIXIE_CTRL_LED)          /*!< ����ܵĶ�ѡ */

//#define NIXIE_TOTAL_LED_NUM			(NIXIE_TOTAL_NUM * (NIXIE_CTRL_LED + 1))     /*!< �����һ�����Ƶ�LED���� */

//#define DRV_DIGITAL_MSG_OUTPUT_MAX_NUM        (128)
/*!*************************************************************************************************
\brief	����ܿ��ƹܽ�
****************************************************************************************************/
#define IO_NIXIE_CTRL_PORT          (HAL_GPIO_PORT_1)
#define IO_NIXIE_CTRL_BITS			(0x0000007F)//ʹ��BIT0 ~ BIT6

#define IO_NIXIE_SWITCH_PORT		(HAL_GPIO_PORT_2)
#define IO_NIXIE_SWITCH_BITS		(0x0000E000)//ʹ��BIT13,BIT14,BIT15

/****************************************END OF THE FILE***************************************************************/




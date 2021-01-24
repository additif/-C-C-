/*!**************************************************************************************************
\copyright Copyright (C) 2019		
\defgroup       
\file           Drv_DigitalTube_Global.h
\brief          
\history:
\version        1.0
\author         YJH
\date           19/11/25 12:09
\remarks        No Remarks
****************************************************************************************************/
#pragma once

/**************************************START OF THE FILE****************************************************************/
/*!*************************************************************************************************
\brief	数码管显示的字符
****************************************************************************************************/
enum ENixieTubeDisplay
{
    //E_DISP_NULL,            /*!< 不显示 */
    E_DISP_0,               /*!< 0 */
    E_DISP_1,               /*!< 1 */
    E_DISP_2,               /*!< 2 */
    E_DISP_3,               /*!< 3 */
    E_DISP_4,               /*!< 4 */
    E_DISP_5,               /*!< 5 */
    E_DISP_6,               /*!< 6 */
    E_DISP_7,               /*!< 7 */
    E_DISP_8,               /*!< 8 */
    E_DISP_9,               /*!< 9 */
    E_DISP_A,               /*!< A */
    E_DISP_b,               /*!< b */
    E_DISP_C,               /*!< C */
    E_DISP_d,               /*!< d */
    E_DISP_E,               /*!< E */
    E_DISP_F,               /*!< F */
    E_DISP_P,               /*!< P */
    E_DISP_L,               /*!< L */
    E_DISP_H,               /*!< H */
    E_DISP_U,               /*!< U */
    E_DISP_EMPTY,           /*!<   */
    E_DISP_SUB,             /*!< - */

    E_DISP_TESTL,
    E_DISP_TESTR,

    E_DISP_MAX,
};

/*!*************************************************************************************************
\brief	数码管显示类型
****************************************************************************************************/
enum ENixieType
{
    E_DISP_NUM_LETTER,      /*!< numeral and letter */
    E_DISP_POINT,           /*!< 显示小数点 */
    E_DISP_LABLE,           /*!< 显示特殊符号,如：#,g,L */
};

typedef enum
{
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	/*			Command 				|				Brief				| (DWORD)dwDat1		| (DWORD)dwDat2		| (PBYTE)pbyDat3	|  Return Value	*/
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	
	DRV_DIGITAL_TUBE_SWITCH,            /*!< 数码管开关				   		| TRUE/FALSE			| No Par2			| No Par3			| No Ret		*/
	
}DRV_DIGITAL_TUBE_MSG;

#ifdef __cplusplus
extern "C" {
#endif

extern void NixieDisplayInterface(ENixieType eType, ENixieTubeDisplay *psDisplayData, UINT8 uDataNum);

#ifdef __cplusplus
};      /* End of 'extern "C"' */
#endif

extern void NixieLEDProcess();

/****************************************END OF THE FILE***************************************************************/




/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_Led_Enum.h
	Date		:	2018-10-16   15:01	

	History	:	
	1:  (2018-10-16 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_LED_ENUM_H__
#define __DRV_LED_ENUM_H__

#ifdef  __cplusplus
extern  "C"{
#endif
/*********************************************START OF THE FILE********************************************************/

/*LED ID*/
typedef enum{
    LED1,       /*0x00是第1个灯的位置*/
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED7,
    LED8,
    LED9,
    LED10,
    LED11,
    LED12,
    LED13,
    LED14,
    LED15,
    LED16,
    LED17,
    LED18,
    LED19,
    LED20,
    LED21,
    LED22,
    LED23,
    LED24,
//    LED25,
//    LED26,
//    LED27,
//    LED28,
//    LED29,
//    LED30,
//    LED31,        /*SP4800少了6个LED，所以屏蔽*/
//    LED32,
//    LED33,
//    LED34,
//    LED35,
//    LED36,
//    
//    LED37,
//LED38,
//LED39,
//LED40,
//LED41,
//LED42,
//LED43,
//LED44,
//LED45,
//LED46,
//LED47,
//LED48,
//LED49,
//LED50,
//LED51,
//LED52,
//LED53,
//LED54,
//LED55,
//LED56,
//LED57,
//LED58,
//LED59,
//LED60,
//LED61,
//LED62,
//LED63,
//LED64,
//LED65,
//LED66,
//LED67,
//LED68,
//LED69,
//LED70,
//LED71,
//LED72,
//LED73,
//LED74,
//LED75,
//LED76,
//LED77,
//LED78,
//LED79,
//LED80,
//LED81,
//LED82,
//LED83,
//LED84,
//LED85,
//LED86,
//LED87,
//LED88,
//LED89,
//LED90,
//LED91,
//LED92,
//LED93,
//LED94,
//LED95,
//LED96,
//LED97,
//LED98,
//LED99,
//LED100,
//LED101,
//LED102,
//LED103,
//LED104,
//LED105,
//LED106,
//LED107,
//LED108,
//LED109,
//LED110,
//LED111,
//LED112,
//LED113,
//LED114,
//LED115,
//LED116,
//LED117,
//LED118,
//LED119,
//LED120,
//LED121,
//LED122,
//LED123,
//LED124,
//LED125,
//LED126,
//LED127,
//LED128,
//LED129,
//LED130,
//LED131,
//LED132,
//LED133,
//LED134,
//LED135,
//LED136,
//LED137,
//LED138,
//LED139,
//LED140,
//LED141,
//LED142,
//LED143,
//LED144,
//LED145,
//LED146,
//LED147,
//LED148,
//LED149,
//LED150,
//LED151,
//LED152,
//LED153,
//LED154,
//LED155,
//LED156,
//LED157,
//LED158,
//LED159,
//LED160,
//LED161,
//LED162,
//LED163,
//LED164,
//LED165,
//LED166,
//LED167,
//LED168,
//LED169,
//LED170,
//LED171,
//LED172,
//LED173,
//LED174,
//LED175,
//LED176,
//LED177,
//LED178,
//LED179,
//LED180,
//LED181,
//LED182,
//LED183,
//LED184,
//LED185,
//LED186,


    LED_TOTAL_NUM,

}ELedId;

typedef enum
{
    All_Led = 0xFF,
}ELedPosition;


//typedef enum{
//    Encode_Tab_0Level = 0x00,      /*灭*/
//    Encode_Tab_16Level = 0x7F,      /*暗*/
//    Encode_Tab_96Level = 0xFF,      /*亮*/
//}ELedMapNum;

typedef enum{
    Encode_Tab_0Level,
    Encode_Tab_8Level,
    Encode_Tab_16Level,
    Encode_Tab_24Level,

    Encode_Tab_32Level,
    Encode_Tab_40Level,
    Encode_Tab_48Level,
    Encode_Tab_56Level,

    Encode_Tab_64Level,
    Encode_Tab_72Level,
    Encode_Tab_80Level,
    Encode_Tab_88Level,

    Encode_Tab_96Level,
    Encode_Tab_104Level,
    Encode_Tab_112Level,
    Encode_Tab_120Level,

    Encode_Tab_128Level,
    Encode_Tab_136Level,
    Encode_Tab_144Level,
    Encode_Tab_152Level,

    Encode_Tab_160Level,
    Encode_Tab_168Level,
    Encode_Tab_176Level,
    Encode_Tab_184Level,

    Encode_Tab_192Level,
    Encode_Tab_200Level,
    Encode_Tab_208Level,
    Encode_Tab_216Level,

    Encode_Tab_224Level,
    Encode_Tab_232Level,
    Encode_Tab_240Level,
    Encode_Tab_248Level,

    Encode_Tab_255Level,/*最亮,但是不使用*/

}ELedMapNum;

typedef enum{
    Encode_Tab_Off_one,
    Encode_Tab_Off_two,
    Encode_Tab_Off_thr,
}ELedEncodeTabNum;



/*********************************************END OF THE FILE**********************************************************/

#ifdef  __cplusplus
}
#endif

#endif



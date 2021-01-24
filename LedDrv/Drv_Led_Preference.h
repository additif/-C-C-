/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_Led_Preference.h
	Date		:	2018-10-16   15:01	

	History	:	
	1:  (2018-10-16 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_LED_PREFERENCE_H__
#define __DRV_LED_PREFERENCE_H__

#ifdef  __cplusplus
extern  "C"{
#endif
/*********************************************START OF THE FILE********************************************************/


#define Led_SPItoTM_BitLen          (1)                                         /*使用1byte SPI数据 表示TM1812的1bit数据*/
#define LED_Status_LEN              (8)                                         /*TM1812 1个LED命令的长度8bit*/
#define LED_SPI_ENCODE_LEN          (Led_SPItoTM_BitLen*LED_Status_LEN)         /*控制1个Led的SPI数据量, byte*/

#define LED_CMD_BUF_SIZE            (LED_TOTAL_NUM*10)							/*命令buffer大小, byte，*4是增加的大小，不够的话要加大*/
#define LED_MAP_BUF_SIZE            (LED_TOTAL_NUM)                             /*Map buffer 大小, byte*/
#define LED_SPI_DATA_BUF_SIZE       (LED_TOTAL_NUM*3)                           /*SPI数据存储buffer,3位SPI数据代表1位“0”或“1”码*/
#define LED_SPI_SEND_BUF_SIZE       (LED_TOTAL_NUM*3+LED_SPI_ENCODE_RESET_LEN)  /*SPI数据发送buffer*/
#define LED_SPI_ENCODE_RESET_LEN    (27)

#define LED_SPI_TX_BYTES_PER_IRQ	(16)                                         /*每个SPI 发送中断发送多少个字节的数据,9byte->3个LED的数据*/



/*********************************************END OF THE FILE**********************************************************/

#ifdef  __cplusplus
}
#endif

#endif


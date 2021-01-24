/*******************************************************************************
	Copyright (C) 2018		
																				
	System Name	:	A1
	Filename	: 	Drv_Led_Process.c
	Date		:	2018-08-28   12:14	

	History	:	
	1:  (2018-08-28 : Yjh) 			
*******************************************************************************/
#ifndef __DRV_LED_PROCESS_C__
#define __DRV_LED_PROCESS_C__

#ifdef  __cplusplus
extern  "C"{
#endif

/*********************************************START OF THE FILE********************************************************/

/*�����ʼ��*/
    SDrv_Led_Command g_arrLed_Cmd_Buf[LED_CMD_BUF_SIZE];
    SDrv_Led_CommandBuf g_sCmdBufRW;

    BYTE g_arrLed_Map_Buf[LED_MAP_BUF_SIZE];
    BYTE g_arrbySPI_DataBuf[LED_SPI_DATA_BUF_SIZE];
    BYTE g_arrbySPI_SendBuf[LED_SPI_SEND_BUF_SIZE];
    
    SDrv_SpiTxIrqInfo g_sLedSpiTxIrqInfo;/*SPI�����ж�ʹ�õ���Ϣ*/

    UINT8   g_Led_Change_Flag;/*LED���ݷ����仯��־*/
    
    UINT8   g_Led_Timer_Flag;
    
    #define TIMER2_SET_DEFAULT              (UINT32)((TIMERClK + 500) / 100)//10ms����һ���ж�

/*!*****************************************************************************
\brief  	��ʼ��
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/10/16 11:58
*******************************************************************************/
void Drv_Led_Init(void)
{   
    /*��ʼ��SPI1*/
    SPI_CTRL_REGISTER   spiCtrlReg;
    SPI_FIFO_CTRL_REGISTER  spiFifoReg;

    //SpiDisable();

    *(UINT32 *)((UINT32)&spiFifoReg) = mwSpiPort(0x10);
    *(UINT32 *)((UINT32)&spiCtrlReg) = mwSpiPort(0x04);

    spiFifoReg.FifoOpMode = SPI_OP_MODE_W;  /*spi��������ģʽ��ֻд*/
    spiFifoReg.FifoRst = 1;                 /*���Fifo*/
    //spiFifoReg.TxTrigEn = 1;
    spiFifoReg.TxTrigNum = SPI_TX_FIFO_E0;  /*����Fifoû�����ݽ����ж�*/

    spiCtrlReg.CsSel = 0;                   //CS0
    spiCtrlReg.Csset = 1;                   //CS0 == 1  ʹ��SPI,��ֹ����
    spiCtrlReg.CphaCpol = CPHA1CPOL0;       //CPHA1CPOL0;
    spiCtrlReg.Dord = SPI_MSB2LSB;          //msb->lsb
    spiCtrlReg.Mstr = SPI_MASTER;           //master
    spiCtrlReg.BusWidth = SPI_8BIT;         //8bit
    spiCtrlReg.ByteDord = SPI_HIGHB2LOW;    //high byte to low byte
    spiCtrlReg.TransWidth = SPI_8BIT;       //8bit
    //spiCtrlReg.Mode = SPI_MODE_NO_DMA;    //û��DMA
    spiCtrlReg.ClkDiv = 24;                 //div24 from pclk = 48 000 000
    
    mwSpiPort(0x10) = *(UINT32 *)((UINT32)&spiFifoReg);
    mwSpiPort(0x04) = *(UINT32 *)((UINT32)&spiCtrlReg);


    g_Led_Change_Flag = 0;  /*LED���ݱ仯��־��0*/
    
    /*ָ�������ָ��*/
    g_sCmdBufRW.dwRd = 0;
    g_sCmdBufRW.dwWr = 0;
    g_sCmdBufRW.psCmdBuf = g_arrLed_Cmd_Buf;
    
    
    TIMER_REG_CTRL tCtl;
    
    tCtl.b.IntEn = 1;
	tCtl.b.ExtClkSel = 0;
	tCtl.b.ExtClkEn = 0;
	tCtl.b.Enable = 1;

    /* set to default value for 1ms timer */
    TimerValueSet(2, TIMER2_SET_DEFAULT);
    
    NVIC_SetPriority(TIMER2_IRQn,0x00);
	/* enable irq of timer1 */
	NVIC_EnableIRQ(TIMER2_IRQn);
	/* enable timer1 int, start timer1 */
	TimerCtrlSet(2, tCtl.w);
    

    return;
}

/*!*****************************************************************************
\brief  	����ȫ��LED
\param[in]	Led_status��LED�����ȼ���
\param[out]	none
\return     none
\retval		none
\remarks    ���⴦��SP200��������Դ�Ƶ������ƣ��ú�����������
	\li		
\author		Yjh 2019/04/16 08:25
*******************************************************************************/
void Drv_AllLedControl(ELedMapNum Led_status)
{
    DWORD i;
    for (i = 0;i <= LED20; i++)
    {
        Drv_Led_CmdWrite((ELedId)i, Led_status);
    }
    return;
}

/*!*****************************************************************************
\brief  	ȫ��LED��
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/10/16 12:02
*******************************************************************************/
void Drv_Led_AllOn(void)
{
    DWORD i;
    for (i = 0;i < LED_TOTAL_NUM; i++)
    {
        Drv_Led_CmdWrite((ELedId)i, Encode_Tab_96Level);
    }
    return;
}


/*!*****************************************************************************
\brief  	ȫ��LED��
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/10/16 12:03
*******************************************************************************/
void Drv_Led_AllOff(void)
{
    DWORD i;
    for (i = 0;i < LED_TOTAL_NUM; i++)
    {
        Drv_Led_CmdWrite((ELedId)i, Encode_Tab_0Level);
    }
    return;
}


/*!*****************************************************************************
\brief  	ȫ��LED��
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/12/8 16:02
*******************************************************************************/
void Drv_Led_AllDark(void)
{
    DWORD i;
    for (i = 0;i < LED_TOTAL_NUM; i++)
    {
        Drv_Led_CmdWrite((ELedId)i, Encode_Tab_16Level);
    }
    return;
}


/*!*****************************************************************************
\brief  	LED�������,��ȡg_arrLed_Cmd_Buf,Ȼ��ˢ��g_arrLed_Map_Buf,
            ���ݽ����g_arrbySPI_DataBuf
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/10/16 12:04
*******************************************************************************/
void Drv_Led_Com_Proc(void)
{
    SDrv_Led_Command  *psCmd;
    
    BYTE *psMap;
    psMap = &g_arrLed_Map_Buf[0];
    
    BYTE *psSpiData;
    psSpiData = &g_arrbySPI_DataBuf[0];
    
    DWORD   i = 0,j = 0;
    BYTE    bdata = 0;

    if(g_sCmdBufRW.dwRd != g_sCmdBufRW.dwWr)
    {
        g_Led_Change_Flag = 1;    
        
        while(g_sCmdBufRW.dwRd != g_sCmdBufRW.dwWr)
        {
            psCmd = &g_sCmdBufRW.psCmdBuf[g_sCmdBufRW.dwRd];
            
            *(psMap+(psCmd->dwID)) = psCmd->dwStatus;
            
            if(((g_sCmdBufRW.dwRd)+1) >= LED_CMD_BUF_SIZE)
            {
                g_sCmdBufRW.dwRd = 0;
            }
            else
            {
                (g_sCmdBufRW.dwRd)++;
            }
        }

        while(i < LED_MAP_BUF_SIZE)
        {
            bdata = (*(psMap + j) & 0x1F);/*0~31 32������*/

            *(psSpiData++) = c_LED_arrbySpiDataEncodeTab[Encode_Tab_0Level + (bdata * 3) + Encode_Tab_Off_one];
            *(psSpiData++) = c_LED_arrbySpiDataEncodeTab[Encode_Tab_0Level + (bdata * 3) + Encode_Tab_Off_two];
            *(psSpiData++) = c_LED_arrbySpiDataEncodeTab[Encode_Tab_0Level + (bdata * 3) + Encode_Tab_Off_thr];
            i++;
            j++;
        }
    }
    return;
}



/*!*****************************************************************************
\brief  	SPI���ݴ���������������SPI�����ж�

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/10/16 12:05
*******************************************************************************/
void Drv_Led_SPI_Data_Proc(void)
{
    SPI_CTRL_REGISTER   spiCtrlReg;
    SPI_FIFO_CTRL_REGISTER  spiFifoReg;
    
    BYTE *psSpiData;
    psSpiData = &g_arrbySPI_DataBuf[0];
    
    BYTE *psSpiSend;
    psSpiSend = &g_arrbySPI_SendBuf[0];
    
    UINT32 width;
    UINT32 dataNumber = LED_SPI_SEND_BUF_SIZE;
    UINT32 i = 0;
    
    /*�ж�LED�����Ƿ��иı�*/
    if(g_Led_Change_Flag)
    {
        g_Led_Change_Flag = 0;
    
        *(UINT32 *)((UINT32)&spiFifoReg) = mwSpiPort(0x10);
        *(UINT32 *)((UINT32)&spiCtrlReg) = mwSpiPort(0x04);
        spiFifoReg.FifoOpMode = SPI_OP_MODE_W;  /*spi��������ģʽ��ֻд*/
        spiFifoReg.FifoRst = 1;                 /*���Fifo*/
        spiFifoReg.TxTrigEn = 0;                /*��SPI�ж�*/
        spiFifoReg.TxTrigNum = SPI_TX_FIFO_E0;  /*����Fifoû�����ݽ����ж�*/
        spiCtrlReg.CsSel = 0;                   //CS0
        spiCtrlReg.Csset = 0;                   //CS0 == 0
        spiCtrlReg.SpiEn = 1;                   //SPI enable  
        mwSpiPort(0x10) = *(UINT32 *)((UINT32)&spiFifoReg);
        mwSpiPort(0x04) = *(UINT32 *)((UINT32)&spiCtrlReg);
    
        for(i = 0; i < LED_SPI_DATA_BUF_SIZE; i++)/*��������*/
        {
            *(psSpiSend++) = *(psSpiData++);
        }
        //memcpy(psSpiSend,psSpiData,sizeof(g_arrbySPI_DataBuf)); /*֮�����������������ַ���*/

        if (spiCtrlReg.TransWidth == SPI_32BIT)
	    {
	    	width = 4;
	    }
	    else if (spiCtrlReg.TransWidth == SPI_16BIT)
	    {
		    width = 2;
	    }
	    else
	    {
	    	width = 1;
	    }
    
        dataNumber = (dataNumber/width);
        SpiDataNumSet(dataNumber);              /*���÷������ݸ����Ĵ���*/
    
        g_sLedSpiTxIrqInfo.dwDataToSend = dataNumber;
        g_sLedSpiTxIrqInfo.dwDateAlreadySend = 0;
        
        g_Led_Timer_Flag = 1;/*�ŵ�������ö�ʱ����־*/
    
        //NVIC_SetPriority(SPITX_IRQn,0x00);                  /*����SPI�����ж����ȼ�*/
        //NVIC_EnableIRQ(SPITX_IRQn);                         /*����SPI1_TX�ж�*/

    }
    return;
}

/*!*****************************************************************************
\brief  	��ʱ�жϽ���ĺ���
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/11/22 10:07
*******************************************************************************/
void TIMER2_Handler(void)
{
    if(g_Led_Timer_Flag)
    {
        while(g_sLedSpiTxIrqInfo.dwDateAlreadySend <= LED_SPI_SEND_BUF_SIZE)
        {
            while(SpiTxFullCheck() == FALSE)
            {
                SpiDataSet(g_arrbySPI_SendBuf[g_sLedSpiTxIrqInfo.dwDateAlreadySend++]);
            }
        }
    }
    g_Led_Timer_Flag = 0;
    TimerIntClr(2);
    
    return;
}


/*!*****************************************************************************
\brief  	�ָ�SPIԭ�ȵ�����
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/10/16 12:06
*******************************************************************************/
void Drv_Led_SPI_DeInit(void)
{
    
    return;
}



/*!*****************************************************************************
\brief  	SPI1_TX�жϽ���ĺ���
	
\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/10/16 12:07
*******************************************************************************/
void  SPI_ONELINETX_HANDLER(void)
{
    if(g_sLedSpiTxIrqInfo.dwDateAlreadySend >= LED_SPI_SEND_BUF_SIZE)   /*�ر�SPI�ж�*/
    {
        SpiTxTrigDisable();
        NVIC_DisableIRQ(SPITX_IRQn);
        SpiCsSet();
        SpiOpModeSet(SPI_OP_MODE_NONE);
        
        SpiDisable();
    }
    else
    {
        DWORD dwPacketSize = g_sLedSpiTxIrqInfo.dwDataToSend - g_sLedSpiTxIrqInfo.dwDateAlreadySend;
        dwPacketSize = MIN(dwPacketSize, LED_SPI_TX_BYTES_PER_IRQ);
        
        while(dwPacketSize--)
        {
            SpiDataSet(g_arrbySPI_SendBuf[g_sLedSpiTxIrqInfo.dwDateAlreadySend++]);
        }
        
    }
    
    return;
}


/*!*****************************************************************************
\brief  	LED�ӿں���
	
\param[in]	Led_position	LED��λѡ
\param[in]	Led_status		LED��״̬
\param[out]	none

\return     none
\retval		none

\remarks
	\li		

\author		Yjh 2018/10/16 12:08
*******************************************************************************/
void Drv_Led_CmdWrite(ELedId Led_position, ELedMapNum Led_status)
{
    SDrv_Led_Command  *psCmd;
    psCmd = &g_sCmdBufRW.psCmdBuf[g_sCmdBufRW.dwWr];
    
    if(Led_position >= LED_TOTAL_NUM)
    {
        ;//LEDλѡ������Χ����������
    }
    else
    {
        psCmd->dwID = Led_position;
        psCmd->dwStatus = Led_status;

        if(((g_sCmdBufRW.dwWr)+1) >= LED_CMD_BUF_SIZE)
        {
            g_sCmdBufRW.dwWr = 0;
        }
        else
        {
            (g_sCmdBufRW.dwWr)++;
        }
    }

    return;
}

/*********************************************END OF THE FILE**********************************************************/
#ifdef  __cplusplus
}
#endif

#endif




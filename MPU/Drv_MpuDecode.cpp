/*******************************************************************************
	Copyright (C) 2018

	System Name	:	A1
	Filename	: 	Drv_MpuDecode.c
	Date		:	2018-12-18   22:30

	History	:
	1:  (2018-12-18 : Yjh)
*******************************************************************************/
#ifndef __DRV_MPUDECODE_C__
#define __DRV_MPUDECODE_C__


/*********************************************START OF THE FILE********************************************************/

    /*MPU OUT MIDI Buff*/
	BYTE g_arrMpuOutMidiBuf[MPU_OUT_MIDI_BUF_SIZE];
    SDrv_MpuOutMidiBuf g_sMpuOutMidiBuf;

    /*MPU OUT Cmd Buff*/
	BYTE g_arrMpuOutCmdBuf[MPU_OUT_CMD_BUF_SIZE];
    SDrv_MpuOutCmdBuf g_sMpuOutCmdBuf;

    /*MPU IN MIDI Buff*/
    UINT8 g_arrMpuInBuf[MPU_IN_BUF_SIZE];
    SDrv_MpuInBuf g_sMpuInBuf;
    
//    /*MPU IN Cmd Buff*/
//    BYTE g_arrMpuInCmdBuf[MPU_IN_CMD_BUF_SIZE];
//    SDrv_MpuInCmdBuf g_sMpuInCmdBuf;

	UINT8	MidiLocalFlag = 1;
	UINT8	MidiUsbOutFlag = 0;

	UINT8	OtherData;

	UINT8	Group0Flag = 1;
	UINT8	Group1Flag = 0;
	UINT8	Group2Flag = 0;
	UINT8	Group3Flag = 0;

	UINT8	EnterNormalModeFlag = 0;
	UINT8	EnterUpdateModeFlag = 0;
	UINT8	EnterSelfCheckModeFlag = 0;
	UINT8	EnterPcbaModeFlag = 0;
    
    UINT8   A2sInitialSuccessFlag = 0;

	SMpuCommonVar g_sMpuCommonVar;//MPUģ�����ݵĹ�������
    
	BOOL SaleType = TRUE;/*���������֣�TRUE������FALSE����*/
	BOOL ConfigNumReceiveFlag = FALSE;/*�Ƿ���ܵ����������ú�����*/

	BOOL AudioVloumeReceive = FALSE;/*�Ƿ���ܵ�Audio��ʱ������*/
/*!*****************************************************************************
\brief  	MPU��ʼ��

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/11/28 10:05
*******************************************************************************/
void Drv_MpuInitial(void)
{
    
    //MPU Out MIDI BUFF ��ʼ��
	g_sMpuOutMidiBuf.dwRd = 0;
	g_sMpuOutMidiBuf.dwWr = 0;
	g_sMpuOutMidiBuf.psMpuOutMidiBuf = g_arrMpuOutMidiBuf;
    //MPU Out Cmd BUFF ��ʼ��
    g_sMpuOutCmdBuf.dwRd = 0;
    g_sMpuOutCmdBuf.dwWr = 0;
    g_sMpuOutCmdBuf.psMpuOutCmdBuf = g_arrMpuOutCmdBuf;

    //MPU In MIDI BUFF ��ʼ��
    g_sMpuInBuf.dwRd = 0;
    g_sMpuInBuf.dwWr = 0;
    g_sMpuInBuf.psMpuInBuf = g_arrMpuInBuf;
    //MPU In Cmd BUFF ��ʼ��
//    g_sMpuInCmdBuf.dwRd = 0;
//    g_sMpuInCmdBuf.dwWr = 0;
//    g_sMpuInCmdBuf.psMpuInCmdBuf = g_arrMpuInCmdBuf;

	g_sMpuCommonVar.CommandHeadFlag = 0;
	g_sMpuCommonVar.CmdDecodeFlag = 0;
	g_sMpuCommonVar.MidiSendFlag = 0;
	memset(g_sMpuCommonVar.arrReceivedBuf, 0, sizeof(g_sMpuCommonVar.arrReceivedBuf));
	memset(g_sMpuCommonVar.arrMidiReceiveBuf, 0, sizeof(g_sMpuCommonVar.arrMidiReceiveBuf));
    g_sMpuCommonVar.nReceivedDataLen = 0;
	g_sMpuCommonVar.nCmdLen = 0;
	g_sMpuCommonVar.nMidiReceivedLen = 0;
	g_sMpuCommonVar.nMidiLen = 0;
	g_sMpuCommonVar.eDecodeStatus = MPU_MCD_TYPE_MIDI;//Ĭ�Ͻ���״̬�ǽ�MIDI��״̬
	g_sMpuCommonVar.eMidiStatus = MPU_MIDI_NONE;//Ĭ��MIDI����״̬Ϊ��״̬

    //��ʼ��MPU�ܽ�
    //GpioFunSet(GPIO_PORT2, GPIO_FUN1, (eGpioNumber)(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8|BIT9|BIT10|BIT11));
    
    //����MPU�˿�,����A2S���յ���������
    GpioOutDataSet(GPIO_PORT2, 0x0000FFFF);

    return;
}



/********************************************* A1 to be Slave Start********************************************************/

/*!*****************************************************************************
\brief  	��Ϊ�ӻ�������������

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks    �����̽��յ����֣�˵��A2S�򾲵縴λ�ˣ�A1��Ҫ���������λ
	\li

\author		Yjh 2018/11/28 10:05
*******************************************************************************/
void Drv_McuSlaveHandshake(void)
{
	if(A2sInitialSuccessFlag)
	{/*A2S��ʼ�����*/
		if(g_sMpuCommonVar.arrReceivedBuf[1] == Handshake_Ask)
		{
			//NVIC_SystemReset();//��λ
		}
	}
	else
	{/*A2S��ʼ��δ���*/
		if(g_sMpuCommonVar.arrReceivedBuf[1] == Handshake_Ask)
		{
			Drv_McuHandshakeReply();//��������Ӧ��
		}
		
	}

    return;
}


/*!*****************************************************************************
\brief  	�ȴ�ģʽѡ������

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks	�ȴ�ģʽѡ��������Bootloader��ѭ���ȴ�
	\li

\author		Yjh 2018/12/11 12:05
*******************************************************************************/
void Drv_McuStartupModeWait(void)
{
//    UINT16	CmdDataOne;
//	UINT16	CmdDataTwo;
//	UINT16	CmdSize;
//	UINT16	ReceiveData;

//	//�ȴ�ģʽѡ������
//	while(g_sMpuInBuf.dwRd != g_sMpuInBuf.dwWr)
//	{
//		CmdDataOne = (g_sMpuInBuf.psMpuInBuf[g_sMpuInBuf.dwRd] & 0xFF);

//		if(((g_sMpuInBuf.dwRd) + 1) >= MPU_IN_BUF_SIZE)
//		{
//			CmdDataTwo = (g_sMpuInBuf.psMpuInBuf[0] & 0xFF);
//			CmdSize    = (g_sMpuInBuf.psMpuInBuf[1] & 0xFF);
//		}
//		else
//		{
//			CmdDataTwo = (g_sMpuInBuf.psMpuInBuf[(g_sMpuInBuf.dwRd) + 1] & 0xFF);

//			if(((g_sMpuInBuf.dwRd) + 2) >= MPU_IN_BUF_SIZE)
//			{
//				CmdSize    = (g_sMpuInBuf.psMpuInBuf[0] & 0xFF);
//			}
//			else
//			{
//				CmdSize    = (g_sMpuInBuf.psMpuInBuf[(g_sMpuInBuf.dwRd) + 2] & 0xFF);
//			}
//			
//		}

//		if(CmdDataOne == CmdHeadOne && CmdDataTwo == CmdHeadTwo && CmdSize >= 0x03)
//		{
//			//E7
//			MpuInRdPointerPlus();
//			ReceiveData = (g_sMpuInBuf.psMpuInBuf[g_sMpuInBuf.dwRd] & 0xFF);

//			//���������
//			MpuInRdPointerPlus();

//			//����ID
//			MpuInRdPointerPlus();
//			ReceiveData = (g_sMpuInBuf.psMpuInBuf[g_sMpuInBuf.dwRd] & 0xFF);

//			if(ReceiveData == Start_Mode_Cmd)
//			{
//				if(((g_sMpuInBuf.dwRd)+1) >= MPU_IN_BUF_SIZE)
//                {g_sMpuInBuf.dwRd = 0;}
//                else{(g_sMpuInBuf.dwRd)++;}
//				ReceiveData = (g_sMpuInBuf.psMpuInBuf[g_sMpuInBuf.dwRd] & 0xFF);

//				switch (ReceiveData)
//				{
//					case NormalMode:
//									EnterNormalModeFlag     = 1;
//									EnterUpdateModeFlag     = 0;
//									EnterSelfCheckModeFlag	= 0;
//									EnterPcbaModeFlag       = 0;
//									break;
//					case UpdateMode:
//									EnterNormalModeFlag     = 0;
//									EnterUpdateModeFlag		= 1;
//									EnterSelfCheckModeFlag	= 0;
//									EnterPcbaModeFlag       = 0;
//									break;
//					case SelfCheckMode:
//									EnterNormalModeFlag     = 0;
//									EnterUpdateModeFlag		= 0;
//									EnterSelfCheckModeFlag	= 1;
//									EnterPcbaModeFlag       = 0;
//									break;
//					case PcbaMode:
//									EnterNormalModeFlag     = 0;
//									EnterUpdateModeFlag		= 0;
//									EnterSelfCheckModeFlag	= 0;
//									EnterPcbaModeFlag       = 1;
//									break;
//					default:		break;
//				}
//                Drv_McuStartupModeReply();//����ģʽӦ��
//                //BootUndoneFlag = 0;     //��ֹ����ѭ����׼��������Ӧ����
//                
//			}
//		}
//		else
//		{
//			//�������ָ���1
//			MpuInRdPointerPlus();
//		}
//		
//	}
    return;
}


/*!*****************************************************************************
\brief  	����ģʽӦ��

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks	����Ӧ���̣������̡����߸��¹��̡����Թ��̣����������������Ӧ������
	\li

\author		Yjh 2018/12/11 12:05
*******************************************************************************/
void Drv_McuStartupModeReply(void)
{
	UINT32	i;
	//����ģʽӦ������
	UINT8	arrNormalModeReply[6] 		= {0xE0, 0xE7, 0x03, 0x01, 0x00, 0x01};
	UINT8	arrUpdateModeReply[6] 		= {0xE0, 0xE7, 0x03, 0x01, 0x01, 0x01};
	UINT8	arrSelfCheckModeReply[6]	= {0xE0, 0xE7, 0x03, 0x01, 0x02, 0x01};
	UINT8	arrPcbaModeReply[6]			= {0xE0, 0xE7, 0x03, 0x01, 0x03, 0x01};
    
    EnterNormalModeFlag = 1;//����������

	if(EnterNormalModeFlag)
	{
		for(i = 0; i < 6; i++)
		{
			Drv_MpuOutCmdBufProc(arrNormalModeReply[i]);
		}
	}
	else if(EnterUpdateModeFlag)
	{
		for(i = 0; i < 6; i++)
		{
			Drv_MpuOutCmdBufProc(arrUpdateModeReply[i]);
		}
	}
	else if(EnterSelfCheckModeFlag)
	{
		for(i = 0; i < 6; i++)
		{
			Drv_MpuOutCmdBufProc(arrSelfCheckModeReply[i]);
		}
	}
	else if(EnterPcbaModeFlag)
	{
		for(i = 0; i < 6; i++)
		{
			Drv_MpuOutCmdBufProc(arrPcbaModeReply[i]);
		}
	}

    return;
}


/*!*****************************************************************************
\brief  	������ѯ�ӻ�оƬ��Ծ״̬(A1���ӻ�)

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/11 11:05
*******************************************************************************/
void Drv_McuQueryActive(void)
{
	UINT32 i;

	//�ظ�������ѯ�����Ӧ������
	UINT8	arrQueryActiveReply[5] = {0xE0, 0xE7, 0x02, 0x02, 0x01};

	if(g_sMpuCommonVar.arrReceivedBuf[1] == QueryActive_Ask)
	{
		for(i = 0; i < 5; i++)
		{
			Drv_MpuOutCmdBufProc(arrQueryActiveReply[i]);
		}
	}

    return;
}


/*!*****************************************************************************
\brief  	A2sоƬ��ʼ�����

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2019/2/18 15:25
*******************************************************************************/
void Drv_A2sInitialSuccess(void)
{
    if(g_sMpuCommonVar.arrReceivedBuf[1] == Initial_Data)
	{
		A2sInitialSuccessFlag = 1;//A2s��ʼ�����
	}

    return;
}


/********************************************* A1 to be Slave End ********************************************************/





/*!*****************************************************************************
\brief  	��MPU Out MIDI Buffд�����ݣ���byteд�룩

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2019/01/11 12:05
*******************************************************************************/
UINT32 Drv_MpuOutMidiWr(UINT8 *datapt, UINT32 dataNumber)
{
	while(dataNumber)
	{
		Drv_MpuOutMidiBufProc(*datapt++);

		dataNumber--;
	}

    return dataNumber;
}


/*!*****************************************************************************
\brief  	дMPU Out MIDI Buff��byteд�룩

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/04 10:05
*******************************************************************************/
void Drv_MpuOutMidiBufProc(BYTE OutMidiData)
{
    BYTE *psMpuMidiOut;/*BYTE ������Ҫ�޸ĳ�UINT16*/
    psMpuMidiOut = &g_sMpuOutMidiBuf.psMpuOutMidiBuf[g_sMpuOutMidiBuf.dwWr];

    *psMpuMidiOut = OutMidiData;
    
    if(((g_sMpuOutMidiBuf.dwWr)+1) >= MPU_OUT_MIDI_BUF_SIZE)
	{
		g_sMpuOutMidiBuf.dwWr = 0;
	}
    else
    {
        (g_sMpuOutMidiBuf.dwWr)++;
    }

    return;
}


/*!*****************************************************************************
\brief  	��MPU Out Cmd Buffд�����ݣ���byteд�룩

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2019/01/11 12:05
*******************************************************************************/
UINT32 Drv_MpuOutCmdWr(UINT8 *datapt, UINT32 dataNumber)
{
	while(dataNumber)
	{
		Drv_MpuOutCmdBufProc(*datapt++);

		dataNumber--;
	}

    return dataNumber;
}


/*!*****************************************************************************
\brief  	дMPU Out Cmd Buff��byteд�룩

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/04 10:05
*******************************************************************************/
void Drv_MpuOutCmdBufProc(BYTE OutCmdData)
{
    BYTE *psMpuCmdOut;/*BYTE ������Ҫ�޸ĳ�UINT16*/
    psMpuCmdOut = &g_sMpuOutCmdBuf.psMpuOutCmdBuf[g_sMpuOutCmdBuf.dwWr];
    
    *psMpuCmdOut = OutCmdData;

    if(((g_sMpuOutCmdBuf.dwWr)+1) >= MPU_OUT_CMD_BUF_SIZE)
	{
		g_sMpuOutCmdBuf.dwWr = 0;
	}
    else
    {
        (g_sMpuOutCmdBuf.dwWr)++;
    }


    return;
}


/*!*****************************************************************************
\brief  	��MPU IN Buffд�����ݣ���byteд�룩
\param[in]	none
\param[out]	none
\return     none
\retval		none
\remarks
	\li
\author		Yjh 2018/12/04 10:05
*******************************************************************************/
UINT32 Drv_MpuInWr(UINT8 *datapt, UINT32 dataNumber)
{
	while(dataNumber)
	{
		Drv_MpuInBufProc(*datapt++);

		dataNumber--;
	}

    return dataNumber;
}
/*!*****************************************************************************
\brief  	дMPU In Buff

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/04 10:05
*******************************************************************************/
void Drv_MpuInBufProc(UINT8 InData)
{
	UINT8 *psMpuInWr;
	psMpuInWr = &g_sMpuInBuf.psMpuInBuf[g_sMpuInBuf.dwWr];

//	//�ݴ���
//	if(InData == 0xF400)
//	{
//		//��������
//	}
//	else
//	{
		(*psMpuInWr) = InData;

		if(((g_sMpuInBuf.dwWr)+1) >= MPU_IN_BUF_SIZE)//buff������0~1023,1024��Ҫ���ؿ�ͷ 
		{
			g_sMpuInBuf.dwWr = 0;
		}
		else
		{
			(g_sMpuInBuf.dwWr)++;
		}
//	}

    return;
}


/*!*****************************************************************************
\brief  	��MPU In Buff����

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2019/03/19 10:05
*******************************************************************************/
BOOL MpuInBufRd(UINT8 *pBuf, UINT32 uSize)
{
	UINT32 nDataRemain = uSize;//��Ҫ��ȡ�����ݸ���
	UINT32 nBufSize;//��ǰBUFF�ڵ�������

	if(g_sMpuInBuf.dwWr >= g_sMpuInBuf.dwRd)
	{
		nBufSize = (g_sMpuInBuf.dwWr - g_sMpuInBuf.dwRd);
	}
	else if(g_sMpuInBuf.dwWr < g_sMpuInBuf.dwRd)
	{
		nBufSize = ((MPU_IN_BUF_SIZE - g_sMpuInBuf.dwRd) + g_sMpuInBuf.dwWr);
	}
	
	//�����Ҫ��ȡ�����ݳ�����BUFF�ڵ���������ֱ�ӷ���FALSE
	if(uSize > nBufSize)
	{
		return FALSE;
	}

	while(nDataRemain != 0)
	{
		*pBuf++ = g_sMpuInBuf.psMpuInBuf[g_sMpuInBuf.dwRd];//��ȡһ��ֵ
        
        //��ָ�����
		if(((g_sMpuInBuf.dwRd)+1) >= MPU_IN_BUF_SIZE)
    	{g_sMpuInBuf.dwRd = 0;}
    	else{(g_sMpuInBuf.dwRd)++;}

		--nDataRemain;//��ȡ���������ݼ�
	}

	return TRUE;

}


/*!*****************************************************************************
\brief  	MPU In Cmd Buff����

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/04 10:05
*******************************************************************************/
//void Drv_MpuInCmdBufProc(BYTE InCmdData)
//{
//	BYTE *psMpuCmdIn;
//	psMpuCmdIn = &g_sMpuInCmdBuf.psMpuInCmdBuf[g_sMpuInCmdBuf.dwWr];

//	if(++(g_sMpuInCmdBuf.dwWr) >= MPU_IN_CMD_BUF_SIZE)
//	{
//		g_sMpuInCmdBuf.dwWr = 0;
//	}

//	*psMpuCmdIn = InCmdData;
//	psMpuCmdIn++;

//    return;
//}


/*!*****************************************************************************
\brief  	��MPUд��׼MIDI����

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/4 15:40
*******************************************************************************/
void Drv_MpuWriteMidiData(void)
{
	UINT8	*psMpuOutMidiRd;
    UINT32	num;
	UINT32	i;
    
	if(g_sMpuOutMidiBuf.dwWr >= g_sMpuOutMidiBuf.dwRd)
	{
		num = (g_sMpuOutMidiBuf.dwWr - g_sMpuOutMidiBuf.dwRd);
	}
	else
	{
		num = ((MPU_OUT_MIDI_BUF_SIZE - g_sMpuOutMidiBuf.dwRd) + g_sMpuOutMidiBuf.dwWr);
	}
	

	if((0x80 & asm_Mpu_ReadStatus()) == 0x80)
	{
		if(num != 0)
		{
			if(num > 64)//һ�����ֻ�ܷ���64���ֽ�
			{
				num = 64;
			}
			for(i = 0; i < num; ++i)
			{
				psMpuOutMidiRd = &g_sMpuOutMidiBuf.psMpuOutMidiBuf[g_sMpuOutMidiBuf.dwRd];
				asm_Mpu_WriteData((*psMpuOutMidiRd));

				if(((g_sMpuOutMidiBuf.dwRd)+1) >= MPU_OUT_MIDI_BUF_SIZE)
				{g_sMpuOutMidiBuf.dwRd = 0;}
				else{(g_sMpuOutMidiBuf.dwRd)++;}

			}
		}
	}
    
    return;
}


/*!*****************************************************************************
\brief  	��MPUд��������

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/4 15:40
*******************************************************************************/
void Drv_MpuWriteCmdData(void)
{
	UINT8	*psMpuOutCmdRd;
    UINT32	num;
	UINT32	i;

	if(g_sMpuOutCmdBuf.dwWr >= g_sMpuOutCmdBuf.dwRd)
	{
		num = (g_sMpuOutCmdBuf.dwWr - g_sMpuOutCmdBuf.dwRd);
	}
	else
	{
		num = ((MPU_OUT_CMD_BUF_SIZE - g_sMpuOutCmdBuf.dwRd) + g_sMpuOutCmdBuf.dwWr);
	}
	

	if((0x80 & asm_Mpu_ReadStatus()) == 0x80)
	{
		if(num != 0)
		{
			if(num > 64)
			{
				num = 64;
			}
			for(i = 0; i < num; ++i)
			{
				psMpuOutCmdRd = &g_sMpuOutCmdBuf.psMpuOutCmdBuf[g_sMpuOutCmdBuf.dwRd];
				asm_Mpu_WriteCmd((*psMpuOutCmdRd));

				if(((g_sMpuOutCmdBuf.dwRd)+1) >= MPU_OUT_CMD_BUF_SIZE)
				{g_sMpuOutCmdBuf.dwRd = 0;}
				else{(g_sMpuOutCmdBuf.dwRd)++;}
			}
		}
	}
    
    return;
}


/*!*****************************************************************************
\brief  	����MPU���ݣ�ʹ�ö�ʱ��ѯ����A2s�����ݣ�

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/12/4 15:40
*******************************************************************************/
void Drv_ReceiveMpuData(void)
{
    UINT8 Mpu_Read_Status;//��ȡA2sMPU״̬�Ĵ���
	BYTE i;
	BYTE Mpu_SendCnt;//A2s MPU�������ݵĸ���

	Mpu_Read_Status = (UINT8)asm_Mpu_ReadStatus();

	if((Mpu_Read_Status & 0x7f) <= 0x3f)
	{
		if(Mpu_Read_Status & 0x3f)
		{
			Mpu_SendCnt = Mpu_Read_Status & 0x3f;
		}
		else
		{
			Mpu_SendCnt = 64;
		}

		for(i = 0; i < Mpu_SendCnt; i++)
		{
			Drv_MpuInBufProc((UINT8)asm_Mpu_ReadData());
		}
	}

    return;
}


/*********************************************END OF THE FILE**********************************************************/
#endif



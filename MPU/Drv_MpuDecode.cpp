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

	SMpuCommonVar g_sMpuCommonVar;//MPU模块数据的公用数据
    
	BOOL SaleType = TRUE;/*内外销区分，TRUE内销，FALSE外销*/
	BOOL ConfigNumReceiveFlag = FALSE;/*是否接受到内外销配置号命令*/

	BOOL AudioVloumeReceive = FALSE;/*是否接受到Audio此时的音量*/
/*!*****************************************************************************
\brief  	MPU初始化

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
    
    //MPU Out MIDI BUFF 初始化
	g_sMpuOutMidiBuf.dwRd = 0;
	g_sMpuOutMidiBuf.dwWr = 0;
	g_sMpuOutMidiBuf.psMpuOutMidiBuf = g_arrMpuOutMidiBuf;
    //MPU Out Cmd BUFF 初始化
    g_sMpuOutCmdBuf.dwRd = 0;
    g_sMpuOutCmdBuf.dwWr = 0;
    g_sMpuOutCmdBuf.psMpuOutCmdBuf = g_arrMpuOutCmdBuf;

    //MPU In MIDI BUFF 初始化
    g_sMpuInBuf.dwRd = 0;
    g_sMpuInBuf.dwWr = 0;
    g_sMpuInBuf.psMpuInBuf = g_arrMpuInBuf;
    //MPU In Cmd BUFF 初始化
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
	g_sMpuCommonVar.eDecodeStatus = MPU_MCD_TYPE_MIDI;//默认解码状态是解MIDI码状态
	g_sMpuCommonVar.eMidiStatus = MPU_MIDI_NONE;//默认MIDI解码状态为无状态

    //初始化MPU管脚
    //GpioFunSet(GPIO_PORT2, GPIO_FUN1, (eGpioNumber)(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8|BIT9|BIT10|BIT11));
    
    //拉高MPU端口,避免A2S接收到错误数据
    GpioOutDataSet(GPIO_PORT2, 0x0000FFFF);

    return;
}



/********************************************* A1 to be Slave Start********************************************************/

/*!*****************************************************************************
\brief  	作为从机接收握手命令

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks    主工程接收到握手，说明A2S打静电复位了，A1需要重新软件复位
	\li

\author		Yjh 2018/11/28 10:05
*******************************************************************************/
void Drv_McuSlaveHandshake(void)
{
	if(A2sInitialSuccessFlag)
	{/*A2S初始化完成*/
		if(g_sMpuCommonVar.arrReceivedBuf[1] == Handshake_Ask)
		{
			//NVIC_SystemReset();//软复位
		}
	}
	else
	{/*A2S初始化未完成*/
		if(g_sMpuCommonVar.arrReceivedBuf[1] == Handshake_Ask)
		{
			Drv_McuHandshakeReply();//发送握手应答
		}
		
	}

    return;
}


/*!*****************************************************************************
\brief  	等待模式选择命令

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks	等待模式选择命令在Bootloader里循环等待
	\li

\author		Yjh 2018/12/11 12:05
*******************************************************************************/
void Drv_McuStartupModeWait(void)
{
//    UINT16	CmdDataOne;
//	UINT16	CmdDataTwo;
//	UINT16	CmdSize;
//	UINT16	ReceiveData;

//	//等待模式选择命令
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

//			//数据命令长度
//			MpuInRdPointerPlus();

//			//命令ID
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
//                Drv_McuStartupModeReply();//启动模式应答
//                //BootUndoneFlag = 0;     //终止数据循环，准备拷贝对应程序
//                
//			}
//		}
//		else
//		{
//			//错误命令，指针加1
//			MpuInRdPointerPlus();
//		}
//		
//	}
    return;
}


/*!*****************************************************************************
\brief  	启动模式应答

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks	在相应工程（主工程、在线更新工程、测试工程）进入这个函数用来应答主机
	\li

\author		Yjh 2018/12/11 12:05
*******************************************************************************/
void Drv_McuStartupModeReply(void)
{
	UINT32	i;
	//进入模式应答命令
	UINT8	arrNormalModeReply[6] 		= {0xE0, 0xE7, 0x03, 0x01, 0x00, 0x01};
	UINT8	arrUpdateModeReply[6] 		= {0xE0, 0xE7, 0x03, 0x01, 0x01, 0x01};
	UINT8	arrSelfCheckModeReply[6]	= {0xE0, 0xE7, 0x03, 0x01, 0x02, 0x01};
	UINT8	arrPcbaModeReply[6]			= {0xE0, 0xE7, 0x03, 0x01, 0x03, 0x01};
    
    EnterNormalModeFlag = 1;//处于主工程

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
\brief  	主机查询从机芯片活跃状态(A1作从机)

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

	//回复主机查询命令的应答命令
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
\brief  	A2s芯片初始化完成

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
		A2sInitialSuccessFlag = 1;//A2s初始化完成
	}

    return;
}


/********************************************* A1 to be Slave End ********************************************************/





/*!*****************************************************************************
\brief  	向MPU Out MIDI Buff写入数据（多byte写入）

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
\brief  	写MPU Out MIDI Buff（byte写入）

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
    BYTE *psMpuMidiOut;/*BYTE 可能需要修改成UINT16*/
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
\brief  	向MPU Out Cmd Buff写入数据（多byte写入）

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
\brief  	写MPU Out Cmd Buff（byte写入）

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
    BYTE *psMpuCmdOut;/*BYTE 可能需要修改成UINT16*/
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
\brief  	向MPU IN Buff写入数据（多byte写入）
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
\brief  	写MPU In Buff

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

//	//容错处理
//	if(InData == 0xF400)
//	{
//		//丢弃数据
//	}
//	else
//	{
		(*psMpuInWr) = InData;

		if(((g_sMpuInBuf.dwWr)+1) >= MPU_IN_BUF_SIZE)//buff区间是0~1023,1024就要跳回开头 
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
\brief  	读MPU In Buff操作

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
	UINT32 nDataRemain = uSize;//需要读取的数据个数
	UINT32 nBufSize;//当前BUFF内的数据量

	if(g_sMpuInBuf.dwWr >= g_sMpuInBuf.dwRd)
	{
		nBufSize = (g_sMpuInBuf.dwWr - g_sMpuInBuf.dwRd);
	}
	else if(g_sMpuInBuf.dwWr < g_sMpuInBuf.dwRd)
	{
		nBufSize = ((MPU_IN_BUF_SIZE - g_sMpuInBuf.dwRd) + g_sMpuInBuf.dwWr);
	}
	
	//如果需要读取的数据超过了BUFF内的数据量，直接返回FALSE
	if(uSize > nBufSize)
	{
		return FALSE;
	}

	while(nDataRemain != 0)
	{
		*pBuf++ = g_sMpuInBuf.psMpuInBuf[g_sMpuInBuf.dwRd];//读取一个值
        
        //读指针后移
		if(((g_sMpuInBuf.dwRd)+1) >= MPU_IN_BUF_SIZE)
    	{g_sMpuInBuf.dwRd = 0;}
    	else{(g_sMpuInBuf.dwRd)++;}

		--nDataRemain;//读取的数据量递减
	}

	return TRUE;

}


/*!*****************************************************************************
\brief  	MPU In Cmd Buff处理

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
\brief  	往MPU写标准MIDI数据

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
			if(num > 64)//一次最多只能发送64个字节
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
\brief  	往MPU写控制命令

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
\brief  	接收MPU数据（使用定时查询接收A2s的数据）

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
    UINT8 Mpu_Read_Status;//读取A2sMPU状态寄存器
	BYTE i;
	BYTE Mpu_SendCnt;//A2s MPU发送数据的个数

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



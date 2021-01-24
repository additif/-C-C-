/*******************************************************************************
	Copyright (C) 2018 

	System Name	:	A1
	Filename	: 	Drv_MpuDataAnalysis.c
	Date		:	2018-12-18   22:30

	History	:
	1:  (2018-12-18 : Yjh)
*******************************************************************************/
#ifndef __DRV_MPUDATAANALYSIS_C__
#define __DRV_MPUDATAANALYSIS_C__

/*********************************************START OF THE FILE********************************************************/

/*!*****************************************************************************
\brief  	往USB端口的发MIDI数据

\param[in]	*pdata:数据指针     num暂时只能为1（一个一个的解码）
\param[out]	none

\return     none
\retval		none

\remarks    MPU接收的MIDI数据需要经过解码成完整的MIDI事件才能发往USB端口
	\li

\author		Yjh 2019/4/1 14:29
*******************************************************************************/
void Drv_MidiToUsbDecode(UINT8 *pdata, UINT16 num)
{
    UINT8 byReceived = *pdata;
    
    if(g_sMpuCommonVar.eMidiStatus == MPU_MIDI_NONE)//MIDI解码无状态
    {
        g_sMpuCommonVar.nMidiReceivedLen = 0;
        switch(byReceived & 0x00F0)
        {
            /* 1 Byte */
            case (0x050):
            {
                g_sMpuCommonVar.nMidiLen = 1;
                g_sMpuCommonVar.eMidiStatus = MPU_MIDI_BYTE;
            }
            /* 2 Byte */
            //case (0x020):
            //case (0x060):
            case (0x0C0):
            case (0x0D0):
            {
                g_sMpuCommonVar.nMidiLen = 2;
                g_sMpuCommonVar.eMidiStatus = MPU_MIDI_BYTE;
            }break;
            /* 3 Byte */
            //case (0x030):
            //case (0x040):
            //case (0x070):
            case (0x080):
            case (0x090):
            case (0x0A0):
            case (0x0B0):
            case (0x0E0):
            {
                g_sMpuCommonVar.nMidiLen = 3;
                g_sMpuCommonVar.eMidiStatus = MPU_MIDI_BYTE;
            }break;
            case (0x0F0):
            {
                switch(byReceived & 0x0F)
                {
                    case(0x00):
                    {/* F0 */
                        g_sMpuCommonVar.nMidiLen = 0xFF;//可能有1K数据，所以长度没有作用
                        g_sMpuCommonVar.eMidiStatus = MPU_MIDI_F0F7;
                    }break;
                    case(0x01):
                    case(0x03):
                    {/* 2 Bytes */
                        g_sMpuCommonVar.nMidiLen = 2;
                        g_sMpuCommonVar.eMidiStatus = MPU_MIDI_BYTE;
                    }break;
                    case(0x02):
                    //case(0x04):
                    //case(0x05):
                    {/* 3 Bytes */
                        g_sMpuCommonVar.nMidiLen = 3;
                        g_sMpuCommonVar.eMidiStatus = MPU_MIDI_BYTE;
                    }break;
		            case(0x0F):
                    {/* 1 BYTE *//* 复位信息 */
                        ClearUSBMidiInputPacketBuf();//复位sound srv
                        ClearUSBMidiOutputPacketBuf();//复位sound srv

                        g_sMpuCommonVar.nMidiLen = 1;
                        g_sMpuCommonVar.eMidiStatus = MPU_MIDI_BYTE;
                    }break;
                    case(0x08):
		            case(0x09):
		            case(0x0A):
		            case(0x0B):
		            case(0x0C):
		            case(0x0E):
                    // {
                    //     m_boFESensitive = TRUE;
                    // }
                    case(0x06):
		            case(0x07):
                    {/* 1 BYTE */
                        g_sMpuCommonVar.nMidiLen = 1;
                        g_sMpuCommonVar.eMidiStatus = MPU_MIDI_BYTE;
                    }break;
                }
            }
            default:
                break;  
        }
    }
    
    if(g_sMpuCommonVar.eMidiStatus == MPU_MIDI_BYTE)//9n,8n,Cn,FFh,Bn
    {
        if(!g_sMpuCommonVar.MidiSendFlag)
        {//MIDI事件未完整
            if(g_sMpuCommonVar.nMidiLen > g_sMpuCommonVar.nMidiReceivedLen)
            {
                g_sMpuCommonVar.arrMidiReceiveBuf[g_sMpuCommonVar.nMidiReceivedLen++] = byReceived;//放MIDI数据
            }
            if(g_sMpuCommonVar.nMidiLen == g_sMpuCommonVar.nMidiReceivedLen)
            {
                g_sMpuCommonVar.MidiSendFlag = 1;//置起Midi码可以发送标志
            }
        }
        if(g_sMpuCommonVar.MidiSendFlag)
        {//MIDI事件完整了可以发送到端口
        
            PacketToUSBMidiEvent(g_sMpuCommonVar.arrMidiReceiveBuf, g_sMpuCommonVar.nMidiLen);
            
            g_sMpuCommonVar.MidiSendFlag = 0;//清空Midi码可以发送标志
            g_sMpuCommonVar.nMidiLen = 0;//清空MIDI数据长度
            g_sMpuCommonVar.eMidiStatus = MPU_MIDI_NONE;//MIDI解码重新设为MIDI无状态
        }
    }
    else if(g_sMpuCommonVar.eMidiStatus == MPU_MIDI_F0F7)
    {
        if(byReceived != 0xF7)
        {
            g_sMpuCommonVar.arrMidiReceiveBuf[g_sMpuCommonVar.nMidiReceivedLen++] = byReceived;//放MIDI数据
            
            if(g_sMpuCommonVar.nMidiReceivedLen >= IO_MIDI_FROM_A2S_SYSEX_MAX_LENGTH)
            {/*大于256字节的F0码丢弃*/
                g_sMpuCommonVar.nMidiLen = 0;//清空MIDI数据长度
                g_sMpuCommonVar.eMidiStatus = MPU_MIDI_NONE;//MIDI解码重新设为MIDI无状态
            }
        }
        else
        {
            g_sMpuCommonVar.arrMidiReceiveBuf[g_sMpuCommonVar.nMidiReceivedLen++] = byReceived;//放F7
            
            PacketToUSBMidiEvent(g_sMpuCommonVar.arrMidiReceiveBuf, g_sMpuCommonVar.nMidiReceivedLen);
            
            g_sMpuCommonVar.nMidiLen = 0;//清空MIDI数据长度
            g_sMpuCommonVar.eMidiStatus = MPU_MIDI_NONE;//MIDI解码重新设为MIDI无状态
        }
    }
    
    return;
}

/*!*****************************************************************************
\brief  	转发MIDI数据

\param[in]	num = 1，底层MIDI解码接受一个一个byte解码
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/11/28 08:23
*******************************************************************************/
void Drv_MidiDataTransmit(UINT8 *pdata, UINT16 num)
{
	if(MidiLocalFlag)		//Midi数据发送到本地
	{
		if(Group0Flag == 1)
		{
            MidiFifoIn(pdata,num);
		}
		if(Group1Flag == 1)
		{
            MidiGroup1In(pdata, num);
		}
		if(Group2Flag == 1)
		{
            MidiGroup2In(pdata, num);
		}
    	if(Group3Flag == 1)
		{
            MidiGroup3In(pdata, num);
		}
	}

	if(MidiUsbOutFlag)		//Midi数据发送到MIDI/USB OUT
	{
        //写到MIDI OUT BUFF
		MidiSendOut(pdata, num);
        //写到USB OUT
		Drv_MidiToUsbDecode(pdata, num);
	}

    return;
}

/*!*****************************************************************************
\brief  	命令数据解码

\param[in]	none
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/11/28 08:23
*******************************************************************************/
void Drv_MpuInDataDecode(void)
{
	UINT8	byReceived;//当前读取到的数据
    BOOL    DecodeCmdSuccess = FALSE;//解码成功标志
    UINT8   CountNum = 0;
    UINT8   byCmdHeadOne = 0xE0;

	while(MpuInBufRd(&byReceived,1))//读取数据成功,读取失败则退出
	{
        CountNum++;/*每次进来最大解码不超过64个字节*/

        if(byReceived == CmdHeadOne)
        {
            g_sMpuCommonVar.CommandHeadFlag = 1;//命令判别标志,遇到E0则置起
            g_sMpuCommonVar.eDecodeStatus = MPU_CMD_TYPE_NONE;//将状态清空，去判断是否遇到E0+E7
        }
        else//没遇到E0,再判断当前是处在解命令还是解MIDI状态.或是处于无状态即前一次遇到E0,这次需要判断是否E7
        {
            switch(g_sMpuCommonVar.eDecodeStatus)
            {
            case MPU_CMD_TYPE_NONE://当前还没有状态，正在判断是否遇到命令头
                {
                    if(g_sMpuCommonVar.CommandHeadFlag)
                    {
                        if(byReceived == CmdHeadTwo)//前一次遇到E0，这次遇到E7，则进入E0+E7解码
                        {
                            g_sMpuCommonVar.eDecodeStatus = MPU_CMD_TYPE_E0E7;
                            g_sMpuCommonVar.CommandHeadFlag = 0;//清空命令头标志位
                        }
                        else//前一次遇到E0，这次遇到的不是E7
                        {
                            Drv_MidiDataTransmit(&byCmdHeadOne, 1);//发送上一次遇到的E0
                            Drv_MidiDataTransmit(&byReceived, 1);//发送这次遇到的数据
                            g_sMpuCommonVar.eDecodeStatus = MPU_MCD_TYPE_MIDI;
                            g_sMpuCommonVar.CommandHeadFlag = 0;//清空命令头标志位
                        }
                    }
                }break;
            case MPU_CMD_TYPE_E0E7://当前进入命令解码
                {
                    //数据长度
                    if(g_sMpuCommonVar.nCmdLen == 0)
                    {
                        g_sMpuCommonVar.nCmdLen = byReceived;
                    }
                    //存放MPU命令的数据包
                    else if(g_sMpuCommonVar.nCmdLen > g_sMpuCommonVar.nReceivedDataLen)
                    {
                        g_sMpuCommonVar.arrReceivedBuf[g_sMpuCommonVar.nReceivedDataLen++] = byReceived;//第一个先放数据长度，然后是命令ID,之后是数据包
                        if(g_sMpuCommonVar.nReceivedDataLen == sizeof(g_sMpuCommonVar.arrReceivedBuf))//最长不会有那么长的命令，所以是错误的
                        {
                            g_sMpuCommonVar.nCmdLen = 0;
                            g_sMpuCommonVar.nReceivedDataLen = 0;
                            g_sMpuCommonVar.CmdDecodeFlag = 0;
                            g_sMpuCommonVar.eDecodeStatus = MPU_CMD_TYPE_NONE;//数据错误，将解码状态置为无状态，重新查找E0+E7头
                        }
                    }
                    //存放完所有的数据后
                    if(g_sMpuCommonVar.nCmdLen == g_sMpuCommonVar.nReceivedDataLen)
                    {
                        g_sMpuCommonVar.CmdDecodeFlag = 1;//置起可以进行解码标志
                    }
                    //进行命令解码
                    if(g_sMpuCommonVar.CmdDecodeFlag)
                    {
                        switch (g_sMpuCommonVar.arrReceivedBuf[0])//判断命令ID
                        {
                            case Midi_Group0:
                                {
                                    Group0Flag = 1;
                                    Group1Flag = 0;
                                    Group2Flag = 0;
                                    Group3Flag = 0;
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Midi_Group1:
                                {
                                    Group0Flag = 0;
                                    Group1Flag = 1;
                                    Group2Flag = 0;
                                    Group3Flag = 0;
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Midi_Group2:
                                {
                                    Group0Flag = 0;
                                    Group1Flag = 0;
                                    Group2Flag = 1;
                                    Group3Flag = 0;
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Midi_Group3:
                                {
                                    Group0Flag = 0;
                                    Group1Flag = 0;
                                    Group2Flag = 0;
                                    Group3Flag = 1;
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Midi_GroupAll:
                                {
                                    Group0Flag = 1;
                                    Group1Flag = 1;
                                    Group2Flag = 1;
                                    Group3Flag = 1;
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Midi_Local:
                                {
                                    MidiLocalFlag = 1;
                                    MidiUsbOutFlag = 0;
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Midi_Usb_Out:
                                {
                                    MidiLocalFlag = 0;
                                    MidiUsbOutFlag = 1;
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Midi_Usb_Local_Out:
                                {
                                    MidiLocalFlag = 1;
                                    MidiUsbOutFlag = 1;
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case HandShake_Cmd:
                                {
                                    //再收到一条握手命令，说明A2S打静电复位了，A1需要重新软件复位
                                    Drv_McuSlaveHandshake();
                                    //NVIC_SystemReset();
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Start_Mode_Cmd:
                                {
                                    //主工程不响应此命令
                                    Drv_McuStartupModeReply();//如果在收到启动模式命令，再应答一次
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Query_Active_Cmd:
                                {
                                    Drv_McuQueryActive();//回复主机查询活跃应答
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case A2sInitial_Cmd:
                                {
                                    Drv_A2sInitialSuccess();//接收主机初始化成功命令

                                    if(A2sInitialSuccessFlag)
                                    {//A2s初始化完成，发送配置号查询命令
                                        BYTE arrConfigNumCmd[] = {CmdHeadOne, CmdHeadTwo, 0x06, ConfigNum_Cmd, CMD_OUT_ASK,0x00,0x00,0x00,0x00};

                                        Drv_MpuOutCmdWr(arrConfigNumCmd, sizeof(arrConfigNumCmd));
                                    }

                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Led_SingleCommand:
                                {
                                    Drv_MpuSingleLedDecode();//单功能LED命令
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case AuxIn_Command:
                                {
                                    Drv_AuxInControl();//AUX IN 由A2S控制
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Led_MultiCommand:
                                {
                                    Drv_MpuMultiLedDecode();//多功能LED命令
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case AudioVolumeCommand:
                                {
                                    Drv_AudioVolumeControl();//A1 USB Audio音量控制命令
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case GlobalVersion_Cmd:
                                {
                                    Drv_GlobalVersionToA2S();//版本号查询命令
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case ConfigNum_Cmd:
                                {
                                    if(CMD_IN_REPLY == g_sMpuCommonVar.arrReceivedBuf[1])
                                    {
                                        if(DOMESTIC_SALE == g_sMpuCommonVar.arrReceivedBuf[5])
                                        {//内销
                                            SaleType = TRUE;

                                            ConfigNumReceiveFlag = TRUE;
                                        }
                                        else if(EXPORT_SALE == g_sMpuCommonVar.arrReceivedBuf[5])
                                        {//外销
                                            SaleType = FALSE;

                                            ConfigNumReceiveFlag = TRUE;
                                        }
                                    }
                                }break;
                            default:
                                {//遇到超出命令范围的命令
                                    DecodeCmdSuccess = FALSE;
                                }break;
                        }
                        
                        if(DecodeCmdSuccess == TRUE)
                        {
                            //解码完成
                            DecodeCmdSuccess = FALSE;
                            g_sMpuCommonVar.nCmdLen = 0;
                            g_sMpuCommonVar.nReceivedDataLen = 0;
                            g_sMpuCommonVar.CmdDecodeFlag = 0;
                            g_sMpuCommonVar.eDecodeStatus = MPU_MCD_TYPE_MIDI;//解码完成后，将解码状态置为解MIDI码
                        }
                        else
                        {
                            //解码未完成或命令超范围
                            DecodeCmdSuccess = FALSE;
                            g_sMpuCommonVar.nCmdLen = 0;
                            g_sMpuCommonVar.nReceivedDataLen = 0;
                            g_sMpuCommonVar.CmdDecodeFlag = 0;
                            g_sMpuCommonVar.eDecodeStatus = MPU_MCD_TYPE_MIDI;
                        }
                    }
                }break;
            case MPU_MCD_TYPE_MIDI://当前进入解Midi码
                {

                    Drv_MidiDataTransmit(&byReceived, 1);
                    
                }break;
            }
        }
    }

    return;
}


/*********************************************END OF THE FILE**********************************************************/
#endif



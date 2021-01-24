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
\brief  	��USB�˿ڵķ�MIDI����

\param[in]	*pdata:����ָ��     num��ʱֻ��Ϊ1��һ��һ���Ľ��룩
\param[out]	none

\return     none
\retval		none

\remarks    MPU���յ�MIDI������Ҫ���������������MIDI�¼����ܷ���USB�˿�
	\li

\author		Yjh 2019/4/1 14:29
*******************************************************************************/
void Drv_MidiToUsbDecode(UINT8 *pdata, UINT16 num)
{
    UINT8 byReceived = *pdata;
    
    if(g_sMpuCommonVar.eMidiStatus == MPU_MIDI_NONE)//MIDI������״̬
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
                        g_sMpuCommonVar.nMidiLen = 0xFF;//������1K���ݣ����Գ���û������
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
                    {/* 1 BYTE *//* ��λ��Ϣ */
                        ClearUSBMidiInputPacketBuf();//��λsound srv
                        ClearUSBMidiOutputPacketBuf();//��λsound srv

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
        {//MIDI�¼�δ����
            if(g_sMpuCommonVar.nMidiLen > g_sMpuCommonVar.nMidiReceivedLen)
            {
                g_sMpuCommonVar.arrMidiReceiveBuf[g_sMpuCommonVar.nMidiReceivedLen++] = byReceived;//��MIDI����
            }
            if(g_sMpuCommonVar.nMidiLen == g_sMpuCommonVar.nMidiReceivedLen)
            {
                g_sMpuCommonVar.MidiSendFlag = 1;//����Midi����Է��ͱ�־
            }
        }
        if(g_sMpuCommonVar.MidiSendFlag)
        {//MIDI�¼������˿��Է��͵��˿�
        
            PacketToUSBMidiEvent(g_sMpuCommonVar.arrMidiReceiveBuf, g_sMpuCommonVar.nMidiLen);
            
            g_sMpuCommonVar.MidiSendFlag = 0;//���Midi����Է��ͱ�־
            g_sMpuCommonVar.nMidiLen = 0;//���MIDI���ݳ���
            g_sMpuCommonVar.eMidiStatus = MPU_MIDI_NONE;//MIDI����������ΪMIDI��״̬
        }
    }
    else if(g_sMpuCommonVar.eMidiStatus == MPU_MIDI_F0F7)
    {
        if(byReceived != 0xF7)
        {
            g_sMpuCommonVar.arrMidiReceiveBuf[g_sMpuCommonVar.nMidiReceivedLen++] = byReceived;//��MIDI����
            
            if(g_sMpuCommonVar.nMidiReceivedLen >= IO_MIDI_FROM_A2S_SYSEX_MAX_LENGTH)
            {/*����256�ֽڵ�F0�붪��*/
                g_sMpuCommonVar.nMidiLen = 0;//���MIDI���ݳ���
                g_sMpuCommonVar.eMidiStatus = MPU_MIDI_NONE;//MIDI����������ΪMIDI��״̬
            }
        }
        else
        {
            g_sMpuCommonVar.arrMidiReceiveBuf[g_sMpuCommonVar.nMidiReceivedLen++] = byReceived;//��F7
            
            PacketToUSBMidiEvent(g_sMpuCommonVar.arrMidiReceiveBuf, g_sMpuCommonVar.nMidiReceivedLen);
            
            g_sMpuCommonVar.nMidiLen = 0;//���MIDI���ݳ���
            g_sMpuCommonVar.eMidiStatus = MPU_MIDI_NONE;//MIDI����������ΪMIDI��״̬
        }
    }
    
    return;
}

/*!*****************************************************************************
\brief  	ת��MIDI����

\param[in]	num = 1���ײ�MIDI�������һ��һ��byte����
\param[out]	none

\return     none
\retval		none

\remarks
	\li

\author		Yjh 2018/11/28 08:23
*******************************************************************************/
void Drv_MidiDataTransmit(UINT8 *pdata, UINT16 num)
{
	if(MidiLocalFlag)		//Midi���ݷ��͵�����
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

	if(MidiUsbOutFlag)		//Midi���ݷ��͵�MIDI/USB OUT
	{
        //д��MIDI OUT BUFF
		MidiSendOut(pdata, num);
        //д��USB OUT
		Drv_MidiToUsbDecode(pdata, num);
	}

    return;
}

/*!*****************************************************************************
\brief  	�������ݽ���

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
	UINT8	byReceived;//��ǰ��ȡ��������
    BOOL    DecodeCmdSuccess = FALSE;//����ɹ���־
    UINT8   CountNum = 0;
    UINT8   byCmdHeadOne = 0xE0;

	while(MpuInBufRd(&byReceived,1))//��ȡ���ݳɹ�,��ȡʧ�����˳�
	{
        CountNum++;/*ÿ�ν��������벻����64���ֽ�*/

        if(byReceived == CmdHeadOne)
        {
            g_sMpuCommonVar.CommandHeadFlag = 1;//�����б��־,����E0������
            g_sMpuCommonVar.eDecodeStatus = MPU_CMD_TYPE_NONE;//��״̬��գ�ȥ�ж��Ƿ�����E0+E7
        }
        else//û����E0,���жϵ�ǰ�Ǵ��ڽ�����ǽ�MIDI״̬.���Ǵ�����״̬��ǰһ������E0,�����Ҫ�ж��Ƿ�E7
        {
            switch(g_sMpuCommonVar.eDecodeStatus)
            {
            case MPU_CMD_TYPE_NONE://��ǰ��û��״̬�������ж��Ƿ���������ͷ
                {
                    if(g_sMpuCommonVar.CommandHeadFlag)
                    {
                        if(byReceived == CmdHeadTwo)//ǰһ������E0���������E7�������E0+E7����
                        {
                            g_sMpuCommonVar.eDecodeStatus = MPU_CMD_TYPE_E0E7;
                            g_sMpuCommonVar.CommandHeadFlag = 0;//�������ͷ��־λ
                        }
                        else//ǰһ������E0����������Ĳ���E7
                        {
                            Drv_MidiDataTransmit(&byCmdHeadOne, 1);//������һ��������E0
                            Drv_MidiDataTransmit(&byReceived, 1);//�����������������
                            g_sMpuCommonVar.eDecodeStatus = MPU_MCD_TYPE_MIDI;
                            g_sMpuCommonVar.CommandHeadFlag = 0;//�������ͷ��־λ
                        }
                    }
                }break;
            case MPU_CMD_TYPE_E0E7://��ǰ�����������
                {
                    //���ݳ���
                    if(g_sMpuCommonVar.nCmdLen == 0)
                    {
                        g_sMpuCommonVar.nCmdLen = byReceived;
                    }
                    //���MPU��������ݰ�
                    else if(g_sMpuCommonVar.nCmdLen > g_sMpuCommonVar.nReceivedDataLen)
                    {
                        g_sMpuCommonVar.arrReceivedBuf[g_sMpuCommonVar.nReceivedDataLen++] = byReceived;//��һ���ȷ����ݳ��ȣ�Ȼ��������ID,֮�������ݰ�
                        if(g_sMpuCommonVar.nReceivedDataLen == sizeof(g_sMpuCommonVar.arrReceivedBuf))//���������ô������������Ǵ����
                        {
                            g_sMpuCommonVar.nCmdLen = 0;
                            g_sMpuCommonVar.nReceivedDataLen = 0;
                            g_sMpuCommonVar.CmdDecodeFlag = 0;
                            g_sMpuCommonVar.eDecodeStatus = MPU_CMD_TYPE_NONE;//���ݴ��󣬽�����״̬��Ϊ��״̬�����²���E0+E7ͷ
                        }
                    }
                    //��������е����ݺ�
                    if(g_sMpuCommonVar.nCmdLen == g_sMpuCommonVar.nReceivedDataLen)
                    {
                        g_sMpuCommonVar.CmdDecodeFlag = 1;//������Խ��н����־
                    }
                    //�����������
                    if(g_sMpuCommonVar.CmdDecodeFlag)
                    {
                        switch (g_sMpuCommonVar.arrReceivedBuf[0])//�ж�����ID
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
                                    //���յ�һ���������˵��A2S�򾲵縴λ�ˣ�A1��Ҫ���������λ
                                    Drv_McuSlaveHandshake();
                                    //NVIC_SystemReset();
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Start_Mode_Cmd:
                                {
                                    //�����̲���Ӧ������
                                    Drv_McuStartupModeReply();//������յ�����ģʽ�����Ӧ��һ��
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Query_Active_Cmd:
                                {
                                    Drv_McuQueryActive();//�ظ�������ѯ��ԾӦ��
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case A2sInitial_Cmd:
                                {
                                    Drv_A2sInitialSuccess();//����������ʼ���ɹ�����

                                    if(A2sInitialSuccessFlag)
                                    {//A2s��ʼ����ɣ��������úŲ�ѯ����
                                        BYTE arrConfigNumCmd[] = {CmdHeadOne, CmdHeadTwo, 0x06, ConfigNum_Cmd, CMD_OUT_ASK,0x00,0x00,0x00,0x00};

                                        Drv_MpuOutCmdWr(arrConfigNumCmd, sizeof(arrConfigNumCmd));
                                    }

                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Led_SingleCommand:
                                {
                                    Drv_MpuSingleLedDecode();//������LED����
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case AuxIn_Command:
                                {
                                    Drv_AuxInControl();//AUX IN ��A2S����
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case Led_MultiCommand:
                                {
                                    Drv_MpuMultiLedDecode();//�๦��LED����
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case AudioVolumeCommand:
                                {
                                    Drv_AudioVolumeControl();//A1 USB Audio������������
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case GlobalVersion_Cmd:
                                {
                                    Drv_GlobalVersionToA2S();//�汾�Ų�ѯ����
                                    DecodeCmdSuccess = TRUE;
                                }break;
                            case ConfigNum_Cmd:
                                {
                                    if(CMD_IN_REPLY == g_sMpuCommonVar.arrReceivedBuf[1])
                                    {
                                        if(DOMESTIC_SALE == g_sMpuCommonVar.arrReceivedBuf[5])
                                        {//����
                                            SaleType = TRUE;

                                            ConfigNumReceiveFlag = TRUE;
                                        }
                                        else if(EXPORT_SALE == g_sMpuCommonVar.arrReceivedBuf[5])
                                        {//����
                                            SaleType = FALSE;

                                            ConfigNumReceiveFlag = TRUE;
                                        }
                                    }
                                }break;
                            default:
                                {//�����������Χ������
                                    DecodeCmdSuccess = FALSE;
                                }break;
                        }
                        
                        if(DecodeCmdSuccess == TRUE)
                        {
                            //�������
                            DecodeCmdSuccess = FALSE;
                            g_sMpuCommonVar.nCmdLen = 0;
                            g_sMpuCommonVar.nReceivedDataLen = 0;
                            g_sMpuCommonVar.CmdDecodeFlag = 0;
                            g_sMpuCommonVar.eDecodeStatus = MPU_MCD_TYPE_MIDI;//������ɺ󣬽�����״̬��Ϊ��MIDI��
                        }
                        else
                        {
                            //����δ��ɻ������Χ
                            DecodeCmdSuccess = FALSE;
                            g_sMpuCommonVar.nCmdLen = 0;
                            g_sMpuCommonVar.nReceivedDataLen = 0;
                            g_sMpuCommonVar.CmdDecodeFlag = 0;
                            g_sMpuCommonVar.eDecodeStatus = MPU_MCD_TYPE_MIDI;
                        }
                    }
                }break;
            case MPU_MCD_TYPE_MIDI://��ǰ�����Midi��
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



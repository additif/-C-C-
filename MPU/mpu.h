/*******************************************************************************
	Copyright (C) 2018

	System Name	:	A1
	Filename	: 	mpu.h V1.0.0
	Date		:	2018-12-18   22:30

	History	:   	Initial version
	1:  (2018-12-18 : Yjh)
*******************************************************************************/

#ifndef		__MPU_H__
#define		__MPU_H__

#ifdef  __cplusplus
extern  "C"{
#endif
/*********************************************START OF THE FILE********************************************************/
#include "main.h"
#include "global.h"

//#define	MPU_SEND_TEST				//���Ͳ���ʹ��
//#define	MPU_SENDDATA_TEST           //����Ϊ�������ݣ�û����Ϊ��������

//#define	MPU_RECEIVE_TEST		//���ղ���ʹ��
//#define MPU_STABILITY_TEST        //�ȶ��Բ���

#define MPU_R_TEST_FALSE    0       //���ղ��Դ���
#define MPU_R_TEST_LOOP     1       //���ղ�����
#define MPU_R_TEST_OVER     2       //���ղ��Խ���

extern void MpuSendData_loop(void);	
extern UINT8 MpuReceiveData_loop(void);

extern void asm_Mpu_WriteData(int R0);
extern void asm_Mpu_WriteCmd(int R0);
extern UINT8 asm_Mpu_ReadStatus(void);
extern UINT8 asm_Mpu_ReadData(void);
/*********************************************END OF THE FILE**********************************************************/
#ifdef  __cplusplus
}
#endif

#endif





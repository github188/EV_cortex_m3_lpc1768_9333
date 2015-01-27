#ifndef _PUBLIC_H
#define _PUBLIC_H

#include "../API/BillDevControl.h"
#include "../API/ChannelHandle.h"
#include "../API/GoodsAndKey.h"
#include "../API/MemoryHandle.h"
#include "../API/ZikuList.h"
#include "../config.h"
#include "../API/UI.h"


//�������������豸����֮��ͨ�ŵĽṹ��
typedef struct
{
    uint8_t   SendType;				//��������
	uint8_t   SubSendType;		   //�Ӽ���������

	unsigned char  value_8;
	unsigned short value_16;
	unsigned int   value_32;
	
} MAIN_DEV_TASK_MSG;	




/*---------------------------------------------------------
�����ͨ���豸������ܵĲ���ָ���
-----------------------------------------------------------*/


#define TASK_DISABLE_REQ           1 //�������ͽ��� ֽӲ��������
#define TASK_DISABLE_RPT		   2

#define TASK_ENABLE_REQ            3//��������ʹ�� ֽӲ����
#define TASK_ENABLE_RPT            4

#define TASK_TRADE_REQ			5 //�������ͳ�����������
#define TASK_TRADE_RPT			6 //

#define TASK_HOPPER_REQ        7
#define TASK_HOPPER_RPT        7


#if 0
//��ȡ����
#define TASKCMD_GETKEY					(0x10)
//Hopper��������
#define TASKCMD_HOPPER					(0x11)
//ֽ�����յ�ֽ�ҷ���ͨ������ֵ
#define TASKCMD_BILLIN					(0x12)
//����ֽ��������
#define TASKCMD_DISABLEDEV				(0x13)
//ʹ��ֽ��������
#define TASKCMD_ENABLEDEV				(0x14)
//ά��״̬�������н����豸����
#define TASKCMD_VINDICATE				(0x15)
//������������
#define TASKCMD_CHANNELHANDLE			(0x16)
//����Ӳ����
#define TASKCMD_DISABLECOIN				(0x17)
//ʹ��Ӳ����
#define TASKCMD_ENABLECOIN				(0x18)
#endif

//����ͨ������ĶԷ�������
extern OS_EVENT *g_msg_main_to_dev;
extern OS_EVENT *g_msg_dev_to_main;
extern MAIN_DEV_TASK_MSG task_msg_main_to_dev;
extern MAIN_DEV_TASK_MSG task_msg_dev_to_main;


//ֽ��������
extern OS_EVENT *g_MoneyIN;
typedef struct _money_recv_msg_{
	unsigned char billType;
	unsigned int billValue;
	
}MONEY_RECV_MSG;
extern MONEY_RECV_MSG money_recv_msg;  


//��ʼ�������ź���
extern OS_EVENT *g_InitSem;
//Ӳ����ͨ����Ϣ����
extern OS_EVENT *g_CoinIn;
#define G_COIN_IN_SIZE    50


//�����豸��ͨ����ֵ���ݽṹ
extern struct STDevValue stDevValue;

/*********************************************************************************************
**��   ��   ��:
**��������˵����
**�������������
**��������������
**�� �� ����ֵ��
**********************************************************************************************/
/*-------------------------------------------------------------------------------------------
                                    �ײ��豸���ƽӿ�API
---------------------------------------------------------------------------------------------*/

/*********************************************************************************************
**��   ��   ��: CreateCommonMBox
**��������˵������������ͨ������
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
void CreateCommonMBox(void);

/*********************************************************************************************
**��   ��   ��: TASK_Device
**��������˵�����豸��������
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
void TASK_Device(void *pdata);

/*********************************************************************************************************
** Function name:       GetKeyInValue
** Descriptions:        �������İ�����ֵ
** input parameters:    ��
** output parameters:   ��
** Returned value:      �а���������Ӧ�ļ�ֵ���ް�������0xff
*********************************************************************************************************/
uint8_t GetKeyInValue(void);

/*********************************************************************************************************
** Function name:       SystemInit
** Descriptions:        ϵͳ��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SystemInit(void);

unsigned char mbox_post_main_to_dev(unsigned char type);
unsigned char mbox_post_dev_to_main(unsigned char type);
unsigned char checkAllDevice(void);

#endif

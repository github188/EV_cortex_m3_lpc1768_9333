/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           MDB.h
** Last modified Date:  2013-01-08
** Last Version:         
** Descriptions:        MDBͨѶЭ��                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-08
** Version:             V0.1
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"

volatile unsigned int MDBCONVERSATIONWAITACKTIMEOUT;//MDBͨѶ��ʱʱ��


/*********************************************************************************************************
** Function name:     	MdbBusHardwareReset
** Descriptions:	    MDB���߸�λ����MDB����TXD������200ms;ע��ú���ΪӲ����λ�������������豸����λ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
/*void MdbBusHardwareReset(void)
{
	PINSEL0 &=(~(0x03<<20));	//P0.10��ΪGPIO
	FIO0DIR |= (1ul<<10);		//����Ϊ���
	FIO0CLR |= (1ul<<10);		//���0
	OSTimeDly(40);				//��ʱ200ms
	PINSEL0 |= (0x03<<20);		//����ΪTXD����
	InitUart2();
	return;
}*/


/*********************************************************************************************************
** Function name:     	MdbBusHardwareReset
** Descriptions:	    MDB���߸�λ����MDB����TXD������200ms;ע��ú���ΪӲ����λ�������������豸����λ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void MdbBusHardwareReset(void)
{
	SetUart2MdbMode();
	PINSEL0 &= (~(0x03<<20));	//P0.10��ΪGPIOģʽ
	FIO0DIR |= (1ul<<10);		//����Ϊ���
	FIO0CLR |= (1ul<<10);		//���0
	OSTimeDly(40);				//��ʱ200ms
	PINSEL0 |= (0x01<<20);		//����ΪTXD����
	InitUart2();
	return;
}


/*********************************************************************************************************
** Function name:     	MdbConversation
** Descriptions:	    MDB�豸ͨѶ
** input parameters:    Dev:�豸��(��5λ��ַ������λ����)��
**						*SeDat:Ҫ���͵����ݣ�
**						SenDatLen��Ҫ�������ݵĳ���
** output parameters:   *RevDat:���յ������ݣ�RevLen:���յ����ݵĳ��ȣ�
** Returned value:      1��ͨѶ�ɹ���0��ͨѶʧ�ܣ�
*********************************************************************************************************/
uint8_t MdbConversation(uint8_t Dev,uint8_t *SeDat,uint8_t SenDatLen,uint8_t *RevDat,uint8_t *RevLen)
{
	unsigned char i,check = 0,err = 0;
	unsigned char Temp;

	ClrUart2Buff();
	//OSSchedLock();
	MdbPutChr(Dev,MDB_ADD);					//�ͳ�MDB��ַ������
	check = Dev;
	for(i=0;i<SenDatLen;i++)				//�ͳ�����
	{
		MdbPutChr(SeDat[i],MDB_DAT);
		check += SeDat[i];	
	}
	MdbPutChr(check,MDB_DAT);				//�ͳ�У���
	//OSSchedUnlock();
	OSTimeDly(3);
	MDBCONVERSATIONWAITACKTIMEOUT = 200;	//����ACK�ȴ�����ʱ200ms
	while(MDBCONVERSATIONWAITACKTIMEOUT)
	{
		Temp = GetMdbStatus();
		if(Temp > MDB_RCV_DATA)
		{
			switch(Temp)
			{
				case MDB_RCV_OK		: *RevLen = Uart2GetStr(RevDat,MDB_MAX_BLOCK_SIZE);
									  err = 1;
									  break;	
				case MDB_OUT_RANGE	: err = 2;
									  break;
				case MDB_CHKSUM_ERR	: err = 3;
									  break;
				default:break;
			}
			break;	
		}	
	}
	return err;	
}
/**************************************End Of File*******************************************************/

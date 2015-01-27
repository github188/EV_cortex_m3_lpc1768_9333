#include "..\config.h"
#include "GoodsAndKey.h"
#include "ChannelHandle.h"


/*********************************************************************************************
**��   ��   ��: delay
**��������˵����us��ʱ����
**�������������max����ʱ��ʱ��
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
void delay(uint32_t max)
{
	while(max)
		max--;
}


/*********************************************************************************************
**��   ��   ��:	SelectKeyInit
**��������˵����ѡ��������ʼ��
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
void SelectKeyInit(void)
{
	//���������ó�����
	FIO1DIR &= ~(1ul<<22);
	FIO1DIR &= ~(1ul<<27);
	FIO1DIR &= ~(1ul<<21);
	FIO1DIR &= ~(1ul<<23);
	FIO1DIR |= (1ul<<31);
	FIO1DIR |= (1ul<<19);
	FIO1DIR |= (1ul<<18);
	FIO1DIR |= (1ul<<20);
	FIO1SET |= (1ul<<18)|(1ul<<19)|(1ul<<20)|(1ul<<31);
	FIO1CLR |= (1ul<<21)|(1ul<<22)|(1ul<<23)|(1ul<<27);
}

/*********************************************************************************************
**��   ��   ��: IsSelectKeyIn
**��������˵��������Ƿ���ѡ����������
**���������������
**����������������
**�� �� ����ֵ�����ݲ�ͬ�ļ�������Ӧ�ļ�ֵ��û�з���0
**********************************************************************************************/
uint8_t IsSelectKeyIn(void)
{
	static uint8_t keyInFlag[4]={0};

	if((FIO1PIN&(1ul<<27))==0x00000000)
	{
		keyInFlag[0]=1;
	}
	else
	{
		if(keyInFlag[0]==1)
		{
			keyInFlag[0]=0;
			Buzzer();
			return 0x04;	
		}
	}
	if((FIO1PIN&(1ul<<23))==0x00000000)
	{
		keyInFlag[1]=1;
	}
	else
	{
		if(keyInFlag[1]==1)
		{
			keyInFlag[1]=0;
			Buzzer();
			return 0x03;	
		}
	}
	if((FIO1PIN&(1ul<<22))==0x00000000)
	{
		keyInFlag[2]=1;
	}
	else
	{
		if(keyInFlag[2]==1)
		{
			keyInFlag[2]=0;
			Buzzer();
			return 0x02;	
		}
	}
	if((FIO1PIN&(1ul<<21))==0x00000000)
	{
		keyInFlag[3]=1;
	}
	else
	{
		if(keyInFlag[3]==1)
		{
			keyInFlag[3]=0;
			Buzzer();
			return 0x01;	
		}
	}
	return 0;
}


unsigned char UserSelectKey()
{
	unsigned char *Ukey,err;

	Ukey = (unsigned char *)OSQPend(g_KeyMsg,15,&err);
	//Trace("Ukey==%d\r\n",*Ukey);
	if(err==OS_NO_ERR)
	{
		return *Ukey;
	}
	else
		return 0;
}


unsigned char ReturnKey()
{
	static uint8_t ReKeyFlag = 0;
	FIO4DIR &= ~(1ul<<28);
	//FIO4SET |= (1ul<<28);

	if(FIO4PIN&(1ul<<28))
	{
		ReKeyFlag = 1;
	}
	else
	{
		if(ReKeyFlag==1)
		{
			ReKeyFlag=0;
			Buzzer();
			return 0x01;	
		}
	}
	return 0;
}








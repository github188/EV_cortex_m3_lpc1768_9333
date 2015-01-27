#include "..\config.h"
#include "BillDevControl.h"
#include "Public.h"

extern volatile uint8_t CoinIn[G_COIN_IN_SIZE];

//����ֽ����ͨ��Ӧ�����ݰ���ʱ����
extern unsigned int MDBCONVERSATIONWAITACKTIMEOUT1;
extern volatile uint8_t RecvBillType;
extern uint8_t BillColumnSet;

extern void *coinInNum[G_COIN_IN_SIZE];

static unsigned char coinIndex = 0;




/*********************************************************************************************************
** Function name:       BillDevSellect
** Descriptions:       	ֽ�����豸ѡ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
#if 0
void BillDevSellect()
{
	unsigned char BillRdBuff[36],BillRdLen,Error;
	unsigned char i;

	//����Ƿ���GBAֽ����
	for(i=0;i<3;i++)
	{
		Error = GbaControl_BillAccept();
		if(Error != 2)
		{
			NowBillDev = BILLDEV_RS232;
			return;
		}
	}
	for(i=0;i<3;i++)
	{
		//����Ƿ���MDBֽ����
		//Reset
		MdbConversation(0x30,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		//Setup
		Error = MdbConversation(0x31,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		if(Error == 1)
		{
			NowBillDev = BILLDEV_MDB;
			return;
		}
	}

	
	
	
}
#endif

/*********************************************************************************************************
** Function name:       BillDevInit
** Descriptions:        ֽ�����豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevInit()
{
	unsigned char BillRdBuff[36],BillRdLen,i,j,ComStatus;	

	if(SYSPara.BillType == VMC_BILL_MDB)
	{
		//Reset
		for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
		{
			ComStatus =MdbConversation(0x30,NULL,0x00,&BillRdBuff[0],&BillRdLen);
			OSTimeDly(OS_TICKS_PER_SEC);
		}
		
		//poll
		for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
		{
			ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		}
		//Setup
		for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
		{
			ComStatus = MdbConversation(0x31,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		}

		if(ComStatus == 1)
		{
			stDevValue.BillLevel = BillRdBuff[0];
			stDevValue.BillCode = ((((uint16_t)BillRdBuff[1]) << 8) | BillRdBuff[2]);
			stDevValue.BillDecimal = 100;
		    for(i = 0; i < BillRdBuff[5]; i++) 
		    {
			   stDevValue.BillDecimal /= 10;
		    }
			stDevValue.BillScale = ((((uint16_t)BillRdBuff[3]) << 8) | BillRdBuff[4]) * stDevValue.BillDecimal;
			//stDevValue.Decimal = BillRdBuff[5];
			stDevValue.BillStkCapacity = ((((uint16_t)BillRdBuff[6]) << 8) | BillRdBuff[7]);
			stDevValue.BillSecurity = ((((uint16_t)BillRdBuff[8]) << 8) | BillRdBuff[9]);
			if(BillRdBuff[10] == 0) 
		    {
		  	  stDevValue.BillEscrowFun = 0;//���ݴ�
		    }
		    else 
		    {
		  	  stDevValue.BillEscrowFun = 1; //���ݴ�
		    }			
			for(i=0;i<8;i++)
			{
				if(BillRdBuff[11+i] == 0xFF) 
				{
				     break;
				}
				
				stDevValue.BillValue[i] = (uint32_t)BillRdBuff[i+11] * stDevValue.BillScale;				
			}
			
		}
		else //��ʼ��ʧ��
		{
			stDevState.billState = 1;
		}
		OSTimeDly(40);
		BillDevEnable();
	}
	else if(SYSPara.BillType == VMC_BILL_RS232)
	{
		BillDevEnable();	
	}
	
}


/*********************************************************************************************************
** Function name:       BillDevDisable
** Descriptions:        ����ֽ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevDisable(void)
{
	uint8_t BillValue[4]={0},BillRdBuff[36],BillRdLen;

	if(SYSPara.BillType == VMC_BILL_RS232)
	{
		GbaControl_BillDisable();
	}
	else
	if(SYSPara.BillType == VMC_BILL_MDB || SYSPara.BillType == VMC_BILL_MDB_ITL)
	{
		BillValue[0] = 0x00;
		BillValue[1] = 0x00;
		BillValue[2] = 0xff;
		BillValue[3] = 0xff;
	   	MdbConversation(0x34,BillValue,4,&BillRdBuff[0],&BillRdLen);	
	}
}


void BillDevReject(void)
{
	uint8_t BillValue[4]={0},BillRdBuff[36],BillRdLen;

	if(SYSPara.BillType == VMC_BILL_RS232)
	{
		GbaControl_BillReject();
	}
	else
	if(SYSPara.BillType == VMC_BILL_MDB || SYSPara.BillType == VMC_BILL_MDB_ITL)
	{
		BillValue[0] = 0x00;
	   	MdbConversation(0x35,BillValue,1,&BillRdBuff[0],&BillRdLen);	
	}	
}



/*********************************************************************************************************
** Function name:       BillDevEnable
** Descriptions:        ʹ��ֽ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevEnable(void)
{
	uint8_t BillWrBuff[4]={0},BillRdBuff[36],BillRdLen;
	
	if(SYSPara.BillType == VMC_BILL_RS232)
	{
		if(stDevValue.BillValue[0] != 0)
			BillColumnSet |= 0x01;
		if(stDevValue.BillValue[1]!=0)
			BillColumnSet |= 0x02;
		if(stDevValue.BillValue[2]!=0)
			BillColumnSet |= 0x04;
		if(stDevValue.BillValue[3]!=0)
			BillColumnSet |= 0x08;
		if(stDevValue.BillValue[4]!=0)
			BillColumnSet |= 0x10;
		if(stDevValue.BillValue[5]!=0)
			BillColumnSet |= 0x20;
		if(stDevValue.BillValue[6]!=0)
			BillColumnSet |= 0x40;
		if(stDevValue.BillValue[7]!=0)
			BillColumnSet |= 0x80;
	}
	else
	if(SYSPara.BillType == VMC_BILL_MDB || SYSPara.BillType == VMC_BILL_MDB_ITL)
	{
		BillWrBuff[0] = 0x00;
		if(stDevValue.BillValue[0])
			BillWrBuff[1] |= 0x01;
		if(stDevValue.BillValue[1])
			BillWrBuff[1] |= 0x02;
		if(stDevValue.BillValue[2])
			BillWrBuff[1] |= 0x04;
		if(stDevValue.BillValue[3])
			BillWrBuff[1] |= 0x08;
		if(stDevValue.BillValue[4])
			BillWrBuff[1] |= 0x10;
		if(stDevValue.BillValue[5])
			BillWrBuff[1] |= 0x20;
		if(stDevValue.BillValue[6])
			BillWrBuff[1] |= 0x40;
		if(stDevValue.BillValue[7])
			BillWrBuff[1] |= 0x80;
		BillWrBuff[2] = 0x00;
		BillWrBuff[3] = 0x0F;
		//Enable
		MdbConversation(0x34,BillWrBuff,4,&BillRdBuff[0],&BillRdLen);
	}
}


/*********************************************************************************************************
** Function name:       GetBillDevState
** Descriptions:        ���ֽ�����豸�ĵ�ǰ״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void GetBillDevState()
{
	unsigned char BillRdBuff[36],BillRdLen,Error,i;

	for(i=0;i<8;i++)
	{
		if(stDevValue.BillValue[i]!=0)
			break;
	}
	if(i>=8)
	{
		stDevState.billState = 1;
		return;
	}
	if(SYSPara.BillType == VMC_BILL_RS232)
	{
		Error = GbaControl_BillAccept();
		if(Error != 2)
		{
			stDevState.billState = 0;		
		}	
		else
			stDevState.billState = 1;
	}
	else
	if(SYSPara.BillType == VMC_BILL_MDB || SYSPara.BillType == VMC_BILL_MDB_ITL)
	{
		Error = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		if(Error == 1)
		{
			BillDevReject();
			stDevState.billState = 0;		
		}	
		else
		{
			BillDevReject();
			stDevState.billState = 1;
		}
	}
}


/*********************************************************************************************************
** Function name:       BillDevProcess
** Descriptions:        ֽ�����豸���Ʋ���
** input parameters:    ��
** output parameters:   RecvMoney���������ֽ�ҽ��
						BillType���������ֽ��ͨ��
** Returned value:      ��ֽ�����뷵��1���޷���0
*********************************************************************************************************/
uint8_t BillDevProcess(unsigned int *RecvMoney,unsigned char *BillType)
{
	unsigned char BillRdBuff[36],BillRdLen,Error,BillWrBuff[1];
	uint8_t type=0;

	BillWrBuff[0] = 0x01;
	if(SYSPara.BillType == VMC_BILL_MDB || SYSPara.BillType == VMC_BILL_MDB_ITL)
	{
		//��ѯֽ�����Ƿ����յ�ֽ�ң��з���1
		Error = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		if(Error == 1)
		{
			stDevState.billState = 0;
			if((BillRdBuff[0]&0xf0)==0x90)
			{
				type = BillRdBuff[0]&0x0f;
				//����ѹ��ָ��
				MdbConversation(0x35,BillWrBuff,0x01,&BillRdBuff[0],&BillRdLen);
				Timer.bill_comunication_timeout = 3000;
				while(Timer.bill_comunication_timeout)
				{
					//��ѯ����Ƿ�ѹ�ֳɹ�
					Error = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
					if(Error == 1)
					{
						if((BillRdBuff[0]&0xf0)==0x80)
						{
							*RecvMoney = stDevValue.BillValue[type];
							*BillType = type;

							money_recv_msg.billType = type ;
							money_recv_msg.billValue = stDevValue.BillValue[type];
							OSQPost(g_MoneyIN,&money_recv_msg);
							Trace("money_recv_msg.moneyback = %d\r\n",money_recv_msg.billValue);
							return 1;	
						}
					}	
				}
			}
		}
		else
		{
			BillDevReject();
			stDevState.billState = 1;
		}
	}
	else
	if(SYSPara.BillType == VMC_BILL_RS232)
	{
		Error = GbaControl_BillAccept();	
		if(Error == 1)
		{
			Error = GbaControl_BillStack();
			Timer.bill_comunication_timeout = 800;
			while(Timer.bill_comunication_timeout)
			{
				Error = GbaControl_BillStacked();
				if(Error == 1)
				{
					type = RecvBillType/8-1;
					*RecvMoney = stDevValue.BillValue[type];
					*BillType = type;

					money_recv_msg.billType = type ;
					money_recv_msg.billValue = stDevValue.BillValue[type];
					OSQPost(g_MoneyIN,&money_recv_msg);
					return 1;	
				}
				OSTimeDly(5);	
			}
		}
		//BillDevReject();
	}
	return 0;
}


unsigned char BillDevIsBillIn(unsigned char *InBillType,unsigned int *InBillValue)
{
	uint8_t err;
	MONEY_RECV_MSG  *BillBackMsg;

	BillBackMsg = OSQPend(g_MoneyIN,10,&err);
	if(err == OS_NO_ERR)
	{
		*InBillType = BillBackMsg->billType;	
		*InBillValue = BillBackMsg->billValue;
		Trace("billType = %d,billValue= %d\r\n",*InBillType,*InBillValue);
		
		return (BillBackMsg->billValue) ? 1 : 0;
	}
	else
		return 0;
}




unsigned char CoinDevProcess()
{
	if(SYSPara.CoinType == VMC_COIN_PARALLEL)
	{
		CoinIn[coinIndex] = ReadParallelCoinAcceptor();
		if(CoinIn[coinIndex])
		{
			OSQPost(g_CoinIn,(void *)&CoinIn[coinIndex]);	
			coinIndex = ++coinIndex % G_COIN_IN_SIZE;
			return 1;
		}

	}
	else if(SYSPara.CoinType == VMC_COIN_SERIAL)
	{	
		CoinIn[coinIndex] = ReadSerialPluseCoinAcceptor();
		if(CoinIn[coinIndex])
		{
			OSQPost(g_CoinIn,(void *)&CoinIn[coinIndex]);
			coinIndex = ++coinIndex % G_COIN_IN_SIZE;
			return 1;
		}	
	}

	return 0;
}


unsigned int getCoinMoney(void)
{
	unsigned int money = 0;
	unsigned char count = 0,rcx= 10;
	if(SYSPara.CoinType == VMC_COIN_PARALLEL)
	{
		count = ReadParallelCoinAcceptor();
		if(count)
		{
			money += stDevValue.CoinValue[count-1];
		}
	}
	else if(SYSPara.CoinType == VMC_COIN_SERIAL)
	{		
		while(rcx--)
		{
			count = ReadSerialPluseCoinAcceptor();
			if(count)
				money += stDevValue.CoinValue[count-1];	
		}		
	}
	
	return money;
}


unsigned int CoinDevGetIn()
{
	unsigned char err,*count,rcx = 10;
	unsigned int money = 0;
	
	while(rcx--)
	{
		count = (unsigned char *)OSQPend(g_CoinIn,5,&err);
		if(err == OS_NO_ERR && *count)
		{
			money += stDevValue.CoinValue[*count-1];
			print_log("coin_recv = %d\r\n",*count);
		}
	}
	return money;

	
#if 0
	while(1)
	{
		count = (unsigned char *)OSQPend(g_CoinIn,5,&err);
		if(*count > 0)
		{
			Trace("Coin_2_%d\r\n",*count);
			if(*count == 1)
				iNum += stDevValue.CoinValue[0];
			else
				iNum += stDevValue.CoinValue[*cOinCount-1];//stDevValue.CoinValue[*cOinCount-1];
			Trace("iNum=%d\r\n",iNum);	
		}
		else
		{
			flag++;
		}
		if(flag>=10)
			break;
	}
	if(iNum>0)
	{
		return iNum;
	}
	return 0;	
#endif

}



/*********************************************************************************************************
** Function name:       deviceEnable
** Descriptions:        �豸 ʹ�� �����
** Returned value:      ��ֽ�����뷵��1���޷���0
*********************************************************************************************************/

void deviceEnable(unsigned char flag)
{
	unsigned char type;
	type = (flag) ? TASK_ENABLE_REQ: TASK_DISABLE_REQ;
	mbox_post_main_to_dev(type);

	msleep(500);
}









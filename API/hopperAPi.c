/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           hopper.h
** Last modified Date:  2014-02-28
** Last Version:        
** Descriptions:        hopper�����㷨+����                    
**------------------------------------------------------------------------------------------------------
** Created by:          yanbo
** Created date:        2014-02-28
** Version:             V1.0
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\Config.h"
#include "..\API\MemoryHandle.h"
#include "..\API\Public.h"
ST_HOPPER sysHopper[3];//��ʼ����ȡflash����

ST_HOPPER stHopper[3];//��������hopper�� �ṹ������
static unsigned char  sn=0;


//����hopper���� �ṹ������ ���������λ��ּ��� 1 > 2 > 3
//���� hopper1 hopper2 ͨ����ֵ 1Ԫ hopper3 ͨ����ֵΪ 5ë �� hopper1 hopper2
//�����ڼ���1��,hopper3 �����ڼ���2��  ����3Ϊ��  ��
ST_HOPPER_LEVEL stHopperLevel[3];



/*********************************************************************************************************
** Function name:     	hopperInit
** Descriptions:	    hopper�豸��ʼ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void hopperInit(void)
{
	unsigned char i,j,levelNum = 0;
	unsigned int value;

	unsigned char ratio[HOPPER_NUMS] = HOPPER_LEVEL_RATIO;
	unsigned char address[HOPPER_NUMS] = HOPPER_ADDR;
	for(i = 0; i < HOPPER_NUMS;i++)
	{
		memset((void *)&stHopper[i],0,sizeof(ST_HOPPER));
		memset((void *)&stHopperLevel[i],0,sizeof(ST_HOPPER_LEVEL));
	}
	
	//��flash��ȡͨ����ֵ����ʼ��hopper����ַ
	for(i = 0;i < HOPPER_NUMS;i++)
	{
		stHopper[i].channelValue= stDevValue.HpValue[i];
		stHopper[i].addr = address[i];
	}

	//�������
	for(i = 0;i < HOPPER_NUMS;i++)
	{
		value = 0;
		//��ѯδ���伶���е����
		for(j = 0;j < HOPPER_NUMS;j++)
		{
			if(!stHopper[j].levelFlag && value < stHopper[j].channelValue)
			{
				value = stHopper[j].channelValue;
			}
		}
		stHopperLevel[i].channelValue = value;
		stHopperLevel[i].ratio = ratio[i];
		if(value)
		{
			for(j = 0;j < HOPPER_NUMS;j++)//��ѯ��ͬ ��ֵ�Ķ� ���з���
			{
				if(value == stHopper[j].channelValue)
				{
					levelNum = stHopperLevel[i].num ++;
					stHopperLevel[i].hopper[levelNum] = &stHopper[j];
					stHopper[j].levelFlag = 1;//��������־
					
				}
			}
		}
		
		Trace("levelNo=%d num =%d value = %d\r\n",
			i,stHopperLevel[i].num,stHopperLevel[i].channelValue);
		
		
	}

	Trace("hopperInit over..\r\n");
}

/*********************************************************************************************************
** Function name:     	getHopperIndex
** Descriptions:	    ����hopper�����ַ�Ż�� ����
** input parameters:    
						addr:�豸��Ӳ�������ַ
						
** output parameters:   ��
** Returned value:      0ʧ��  ���� ���� ������
*********************************************************************************************************/
unsigned char getHopperIndex(unsigned char addr)
{
	unsigned char i;
	for(i = 0;i < HOPPER_NUMS;i++)
	{
		if(stHopper[i].addr == addr)
			return (i + 1);
	}

	return 0;
}


unsigned char getMinHopperPriceState(void)
{
	unsigned char i,j;
	for(i = HOPPER_NUMS;i > 0;i++)
	{
		if(stHopperLevel[i - 1].channelValue)
		{
			break;
		}
			
	}
	if(i)
	{
		for(j = 0;j < stHopperLevel[i - 1].num;j++)
		{
			if(stHopperLevel[i - 1].hopper[j]->state == 0)
				return 1;
		}	
	}
	
	return 0;
}

/*********************************************************************************************************
** Function name:     	evb_hopper_handle
** Descriptions:	    EVBhopper�豸�������ƺ���
** input parameters:    cmdType:��Ҫ�������Ƶ��豸����
						address:�豸��Ӳ�������ַ
						cmdVlaue:�������ݣ���Hopper���Ҳ����ĳ�������
** output parameters:   ��
** Returned value:      1���յ�Ӧ��0��δ�յ�Ӧ��ͨѶʧ��
*********************************************************************************************************/
uint8_t evb_hopper_check(uint8_t address)
{
	uint8_t EvbAckMsg[36] = {0};
	uint8_t res,hopperNo=0;
	res = EvbConversation(0,0xED,sn,EVB_HANDLE_HP_QUERY,address,0,EvbAckMsg);
	if(res == 1)
	{
		if(EvbAckMsg[1] == 0x08)//��ѯ���ذ�
		{
			hopperNo = getHopperIndex(EvbAckMsg[4]);	
	
			EvbAckMsg[3] &= ~(0x01 | (0x01 << 2) | (0x01 << 1) | (0x01 << 7) );//���ηǹ���λ
			if(EvbAckMsg[3]==0x00)//����
				stHopper[hopperNo -1 ].state= 0x00;
			else if((EvbAckMsg[3] & (0x01 << 5)) || (EvbAckMsg[3] & (0x01 << 3)))//ȱ��
			{
				if((EvbAckMsg[3] & (0x01 << 4)) || (EvbAckMsg[3] & (0x01 << 6)))//�й���
					stHopper[hopperNo -1 ].state = 0x02;
				else
					stHopper[hopperNo -1 ].state = 0x01;
			}
				
			else
				stHopper[hopperNo -1 ].state = 0x02;	//����
				
			stHopper[hopperNo -1 ].LastOutPutCount =  INTEG16(EvbAckMsg[6], EvbAckMsg[5]);

			
			return 1;
		}
					
	}	

	return 0;
		
}



/*********************************************************************************************************
** Function name:     	evb_hopper_handle
** Descriptions:	    EVBhopper�豸�������ƺ���
** input parameters:    cmdType:��Ҫ�������Ƶ��豸����
						address:�豸��Ӳ�������ַ
						cmdVlaue:�������ݣ���Hopper���Ҳ����ĳ�������
** output parameters:   ��
** Returned value:      1���յ�Ӧ��0��δ�յ�Ӧ��ͨѶʧ��
*********************************************************************************************************/
uint8_t evb_hopper_handle(uint8_t cmdType,uint8_t address,unsigned short cmdVlaue)
{
	uint8_t EvbAckMsg[36] = {0},j;
	uint8_t MsgHead,MsgType,MsgAdd;
	uint16_t MsgDate;
	uint8_t res,hopperNo=0;

	if(cmdType == EVB_HANDLE_HP_QUERY)//Hopper��ѯ����ָ��	
	{
		MsgHead = 0xED;
		MsgType = EVB_HANDLE_HP_QUERY;
		MsgAdd = address;
		MsgDate = 0;	
	}
	else if(cmdType == EVB_HANDLE_HP_OUTPUT)//Hopper���Ҳ���ָ��
	{
		MsgHead = 0xED;
		MsgType = EVB_HANDLE_HP_OUTPUT;
		MsgAdd = address;
		MsgDate =  cmdVlaue;
		sn++;
	}
	else
	{
		Trace("EvbDevice_Handle the cmdType is err!\r\n");	
		return 0;
	}
	
	
	for(j=0;j<5;j++)
	{
		//���Ͳ����������ж�Ӧ���
		res = EvbConversation(0,MsgHead,sn,MsgType,MsgAdd,MsgDate,EvbAckMsg);
		if(res == 1)
		{
			//���һ�Ӧ��
			if(EvbAckMsg[1] == 0x06 && MsgType == EVB_HANDLE_HP_OUTPUT)
			{
				print_log("hopper%d output cmd...ok\r\n",address + 1);
				return 1;
			}
			else if(EvbAckMsg[1] == 0x08 && MsgType == EVB_HANDLE_HP_QUERY)//��ѯ���ذ�
			{
				
				hopperNo = getHopperIndex(EvbAckMsg[4]);
				print_log("hopper%d checked state = %x lastCOunt= %d\r\n",
					hopperNo,EvbAckMsg[3],stHopper[hopperNo -1 ].LastOutPutCount);
				EvbAckMsg[3] &= ~((0x01 << 2) | (0x01 << 1) | (0x01 << 7) );//���ηǹ���λ
				if(EvbAckMsg[3]==0x00)//����
					stHopper[hopperNo -1 ].state= 0x00;
				else if(EvbAckMsg[3] & 0x01)//����ʧ��
				{
					stHopper[hopperNo -1 ].state = 0x02;
				}
				else if((EvbAckMsg[3] & (0x01 << 5)) || (EvbAckMsg[3] & (0x01 << 3)))//ȱ��
				{
					if((EvbAckMsg[3] & (0x01 << 4)) || (EvbAckMsg[3] & (0x01 << 6)))//�й���
						stHopper[hopperNo -1 ].state = 0x02;
					else
						stHopper[hopperNo -1 ].state = 0x01;
				}
				else
					stHopper[hopperNo -1 ].state = 0x02;	//����

					
				stHopper[hopperNo -1 ].LastOutPutCount =  INTEG16(EvbAckMsg[6], EvbAckMsg[5]);

				
				return 1;
			}			
			
		}	
		
	}
	return 0;
}

/*********************************************************************************************************
** Function name:     	getHopperHandleTime
** Descriptions:	    ������������� ��ʱ�ȴ�ʱ��
** input parameters:    
						
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned short getHopperHandleTime(unsigned int value)
{
	unsigned char i;
	for(i = HOPPER_NUMS;i > 0;i--)
	{
		if(stHopperLevel[i - 1].channelValue)
		{
			return (((value/stHopperLevel[i - 1].channelValue) * 20) + 12000);
		}
	}
	return 12000;
}



/*********************************************************************************************************
** Function name:     	hopperTestOutput
** Descriptions:	    hopper���Ҳ��Բ�������
** input parameters:    Count����������
						addr:��ַ���
** output parameters:   ��
** Returned value:      1���ɹ����ң�0������ʧ��
*********************************************************************************************************/
uint8_t hopperTestOutput(unsigned short  count,unsigned char addr)
{
	uint8_t res;
	unsigned char hpIndex;
	if(count<=0)
	{
		Trace("count is 0 ......\r\n");
		return 0;	
	}
	//���ͳ���ָ��
	res = evb_hopper_handle(EVB_HANDLE_HP_OUTPUT,addr,count);
	if(res==1)
	{
		OSTimeDly(100);
		//�ȴ�������ɷ���Ӧ���
		Timer.HpHandleTimer = 5000 + count * 250;
		while(Timer.HpHandleTimer)
		{
			Trace("HpHandleTimer=%d\r\n",Timer.HpHandleTimer);
			//��ѯ�Ƿ�������
			res = evb_hopper_handle(EVB_HANDLE_HP_QUERY,addr,0);
			//Trace("evb_hopper_handle res = %d addr = %d lastOutput = %d\r\n",res,addr,stHopper[getHopperIndex(addr) - 1].LastOutPutCount);
			if(res==1)
			{
				hpIndex = getHopperIndex(addr);
				if(hpIndex)
				{	
					Trace("LastOutPutCount = %d  count = %d\r\n",stHopper[hpIndex - 1].LastOutPutCount,count);
					if(stHopper[hpIndex - 1].LastOutPutCount != count)//����ʧ��
					{
						
						return 0;
					}	
					else
						return 1;			
				}					
			}
			OSTimeDly(2);
		}
		return 0;	
	}
	return 0;
}





/*********************************************************************************************************
** Function name:     	hopperOutput
** Descriptions:	    hopper���Ҳ�������
** input parameters:    Count����������
						addr:��ַ���
** output parameters:   ��
** Returned value:      1���ɹ����ң�0������ʧ��
*********************************************************************************************************/
uint8_t hopperOutput(unsigned short  count,unsigned char addr)
{
	uint8_t res,i;
	unsigned char hpIndex;
	if(count<=0)
	{
		Trace("count is 0 ......\r\n");
		return 1;	
	}
	//���ͳ���ָ��
	for(i = 0;i < 3;i++)
	{
		res = evb_hopper_handle(EVB_HANDLE_HP_OUTPUT,addr,count);
		if(res == 1)
			break;
		print_log("hopper output cmd fail num = %d\r\n",i);
		OSTimeDly(100/5);
	}
	if(res==1)
	{
		OSTimeDly(100);
		//�ȴ�������ɷ���Ӧ���
		Timer.HpHandleTimer = 5000 + count * 250;
		while(Timer.HpHandleTimer)
		{
			Trace("HpHandleTimer=%d\r\n",Timer.HpHandleTimer);
			//��ѯ�Ƿ�������
			res = evb_hopper_handle(EVB_HANDLE_HP_QUERY,addr,0);
			//Trace("evb_hopper_handle res = %d addr = %d lastOutput = %d\r\n",res,addr,stHopper[getHopperIndex(addr) - 1].LastOutPutCount);
			if(res==1)
			{
				hpIndex = getHopperIndex(addr);
				Trace("hopper%d : LastOutPutCount = %d  count = %d\r\n",hpIndex,stHopper[hpIndex - 1].LastOutPutCount,count);
				if(hpIndex)
				{	
					if(hpIndex == 1)
						stTotalTrade.Hopper1DispSum += stHopper[hpIndex - 1].LastOutPutCount;
					else if(hpIndex == 2)
						stTotalTrade.Hopper2DispSum += stHopper[hpIndex - 1].LastOutPutCount;
					else if(hpIndex == 3)
						stTotalTrade.Hopper3DispSum += stHopper[hpIndex - 1].LastOutPutCount;

					print_log("output:hopper1 =%d hopper2 = %d hopper3 = %d\r\n",
						stTotalTrade.Hopper1DispSum,stTotalTrade.Hopper2DispSum,stTotalTrade.Hopper3DispSum);
					if(stHopper[hpIndex - 1].LastOutPutCount != count)//����ʧ��
					{
						//stHopper[hpIndex - 1].lastPayoutFail = 1;
						return 0;
					}	
					else
					{
						//stHopper[hpIndex - 1].lastPayoutFail = 0;
						return 1;	
					}			
				}					
			}
			OSTimeDly(2);
		}
		return 0;	
	}
	return 0;
}



/*********************************************************************************************************
** Function name:     	checkHopperAndUpdate
** Descriptions:	    ���hopper������״̬�� �ṹ��
** input parameters:    
						
** output parameters:   
** Returned value:      
*********************************************************************************************************/

void checkHopperAndUpdate(void)
{
	unsigned char i;
	for(i = 0;i < HOPPER_NUMS;i++)
	{
		if(stHopper[i].channelValue)
		{
			evb_hopper_check(stHopper[i].addr);	
		}
		else
			 stHopper[i].state = 3;

	} 
	
}



/*********************************************************************************************************
** Function name:     	hopperOutputByLevel
** Descriptions:	    hopper�豸�����������
** input parameters:    changeCount������Ҫ���������
** output parameters:   remainCount��������ʧ�ܺ�ʣ�������
						levelNo :ѡ���ĸ����� ����
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/
unsigned char  hopperOutputByLevel(unsigned int changeCount,unsigned int *remainCount,
				unsigned char levelNo)
{
	unsigned char i,addr,cycleFlag = 0,res,rcx;
	unsigned int changedCount = 0;//��������
	if(changeCount == 0)
	{
		*remainCount = 0;
		return 1;
	}
	//���� hopper�����Ƿ����
	if(levelNo > HOPPER_NUMS)
	{
		*remainCount = changeCount;
		Trace("the levelNo = %d > the best no %d \r\n",levelNo,HOPPER_NUMS);
		return 0;
	}
	if(!stHopperLevel[levelNo - 1].num )//û�п��õĶ�
	{
		Trace("the levelNo = %d useAbleNum = is 0 \r\n",levelNo);
		*remainCount = changeCount;
		return 0;
	}
	//��ѯ�Ƿ���ѭ����
	for(i = 0; i < stHopperLevel[levelNo - 1].num;i++)
	{
		if(stHopperLevel[levelNo - 1].hopper[i]->isCycle && stHopperLevel[levelNo - 1].hopper[i]->state == 0)
		{
			cycleFlag = 1;
			break;
		}		
	}
	if(cycleFlag)//��ѭ���� ������ѭ��������
	{
		addr = stHopperLevel[levelNo - 1].hopper[i]->addr;
		res = hopperOutput(changeCount,addr);
		if(res)
		{
			*remainCount  = 0;
			return 1;
		}
		else
		{
			changedCount += stHopperLevel[levelNo - 1].hopper[i]->LastOutPutCount;//ѭ��������ʧ�� ����������	
		}
	}
	//û��ѭ���� �� ���ѡ��һ�����õĶ� ����
	for(i = 0; i < stHopperLevel[levelNo - 1].num;i++)
	{	
		if(stHopperLevel[levelNo - 1].hopper[i]->specialFault >= HOPPER_FAULT_NUM)//����豸����������
			continue;
		addr = stHopperLevel[levelNo - 1].hopper[i]->addr;	
		for(rcx = 0;rcx < 2;rcx++)
		{
			res = hopperOutput(changeCount - changedCount,addr);
			Trace("selcet-- levelNo = %d addr= %d res =%d\r\n",levelNo,addr,res);
			if(res)
			{	
				*remainCount  = 0;
				stHopperLevel[levelNo - 1].hopper[i]->specialFault =  0;//�������
				return 1;
			}
			else //ʧ���� ��������һ�� ͬ����Ķ� ����
			{
				changedCount += stHopperLevel[levelNo - 1].hopper[i]->LastOutPutCount;
				Trace("changeCount = %d,hopper[%d]->LastOutPutCount =%d,changedCount =%d\r\n",
					changeCount,i,stHopperLevel[levelNo - 1].hopper[i]->LastOutPutCount,changedCount);				
			}
		}		
		
		//����ѭ�������ұң���������Ҳ������Ļ�
		//�������Ϊ hopper ������ϲ����			
		stHopperLevel[levelNo - 1].hopper[i]->specialFault ++;//����+1
						
	}
	//��ѯ���������п��õĶ� ���Һ�����յ�ʧ�����
	*remainCount = changeCount - changedCount;
	return 0;
		
	
	
}


/*********************************************************************************************************
** Function name:     	hopperChangerHandle
** Descriptions:	    hopper�豸�����������
** input parameters:    changeCount������Ҫ����Ľ��
** output parameters:   remainCount��������ʧ�ܺ�ʣ��Ľ��
						
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/
unsigned char  hopperChangerHandle(unsigned int changeMoney,unsigned int *remainMoney)
{
	unsigned int changedCount,remainCount;
	unsigned char res,i;
	//Trace("hopperChangerHandle:changeMoney = %d  \r\n",changeMoney);
	//������ �Ӵ�С ��ʼ����
	for(i = 0; i < HOPPER_NUMS;i++)
	{
		//Trace("stHopperLevel[%d].channelValue = %d\r\n",i,stHopperLevel[i].channelValue);
		if(!stHopperLevel[i].channelValue)//�˼��𲻴���
			continue;
		changedCount = changeMoney / stHopperLevel[i].channelValue; //������ ��Ҫ������
		changeMoney  = changeMoney - (changedCount * stHopperLevel[i].channelValue);
		res = hopperOutputByLevel(changedCount,&remainCount,(i + 1));
		if(res)//����ɹ�
		{
			//Trace("changeMoney .... %d %d %d\r\n",changeMoney,changedCount,stHopperLevel[i].channelValue);
			if(changeMoney == 0)//������ɹ�
			{
				*remainMoney = 0;
				return 1;
			}
		}
		else
		{
			changeMoney  = changeMoney + remainCount * stHopperLevel[i].channelValue;	
			//Trace("changeMoney remain .... %d  %d %d\r\n",changeMoney,(changedCount - remainCount),stHopperLevel[i].channelValue);
		}
		
		
	}
	//���м��𶷶������� ֱ�ӷ���ʣ����
	*remainMoney = changeMoney;
	Trace("all level is checked....\r\n");
	return 0;
		
		
	
	
}

/*********************************************************************************************************
** Function name:     	evb_device_hp_dispence
** Descriptions:	    EVB_Hp�豸�����������
** input parameters:    ChangeMoney������Ҫ����Ľ��
** output parameters:   SurplusMoney��������ʧ�ܺ�ʣ��Ľ��
						Hp1OutNum����00��ַHopper�ĳ��Ҹ���
						Hp2OutNum����01��ַHopper�ĳ��Ҹ���
						Hp3OutNum����10��ַHopper�ĳ��Ҹ���
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/
uint32_t evb_device_hp_dispence(uint32_t ChangeMoney,uint32_t *SurplusMoney,uint8_t *Hp1OutNum,uint8_t *Hp2OutNum,uint8_t *Hp3OutNum)
{
	unsigned char mErrFlag = 3,err;
	MAIN_DEV_TASK_MSG *HpBackMsg;
	unsigned int remainVlaue;
	unsigned short timeOut;
	while(mErrFlag)
	{
		
		task_msg_main_to_dev.value_32 = ChangeMoney;
		//���������DevieTask�������Hopper�����Ҳ���
		mbox_post_main_to_dev(TASK_HOPPER_REQ);
		//�ȴ�DeviceTask���񷵻�Ӧ�����䣬�õ�ָ��Hopper�ĳ������
		
		timeOut = getHopperHandleTime(ChangeMoney);//������С����������㳬ʱʱ��

		HpBackMsg = OSMboxPend(g_msg_dev_to_main,timeOut,&err);
		if(err == OS_NO_ERR)
		{
			remainVlaue = HpBackMsg->value_32;
			//Trace("hopper mboxBack remainVlaue=%d\r\n",remainVlaue);
			*SurplusMoney = remainVlaue;
			*Hp1OutNum = stHopper[0].LastOutPutCount;
			*Hp2OutNum = stHopper[1].LastOutPutCount;
			*Hp3OutNum = stHopper[2].LastOutPutCount;
			
			return (remainVlaue) ? 0 : 1;
			
		}
		else
		{
			mErrFlag--;
			continue;
		}
	}

	return 0;
}




/*********************************************************************************************************
//�㷨Ԥ����hopper �������� ��ֵ 2Ԫ ����ֵ 1Ԫ
1.ֽ�Ҷ��㰴�� 5Ԫ(2*2 + 1*1)�ķ�ʽ���ж���
ֽ��Ͷһ���������

Ӳ��Ͷ���һö��ֵ���ڵ���  hopper�����ֵ��Ϊ�Ƕ�С
��ʱͶһö��������

Ӳ��Ͷ���һö��ֵС��hopper�����ֵ�����ǶҴ� Ĭ�ϰ���һ������ֵ����

2.Ӳ�Ҷ�����С 2Ԫ Ӳ�� �һ� 2*1Ԫ
3.Ӳ�Ҷ��� С�Ҵ� һ���������Ҫ�� �����һ�Ӳ��
	���� Ͷ 50�� 20��20�� 10�ֺ� �������1Ԫ*1
	����Ͷ 1Ԫ1Ԫ �������� 1*2Ԫ  

*********************************************************************************************************/


/*********************************************************************************************************
** Function name:     	vmcBillChanger
** Descriptions:	     ֽ�Ҷ����㷨
** input parameters:   
** output parameters:   
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/
unsigned char  vmcBillChanger(unsigned int changeAmount,unsigned int *remainAmount)
{
	char i,j,res;
	unsigned int remainCount = 0,changeCount = 0;	
	//��ֽ�������㷨
	if(!stDevValue.hpBaseChange)
	{
		print_log("stDevValue.hpBaseChange is 0 can't change.....\r\n");
		return 0;
	}
	for(i = 0;i < HOPPER_NUMS;i++)
	{		
		if(!stHopperLevel[i].channelValue ||  !stHopperLevel[i].ratio)
		{
			break;
		}
	}	
	
	*remainAmount = changeAmount % HOPPER_BASE_CHANGED;

	if(changeAmount >= HOPPER_BASE_CHANGED)
	{
		//�������Ƚ���һ��һ��ѭ������
		for(j = 0;j < i;j++)
		{
			changeCount = (changeAmount / HOPPER_BASE_CHANGED) * stHopperLevel[j].ratio;
			if(!hopperOutputByLevel(changeCount, &remainCount, j+1))
			{
				*remainAmount += remainCount * stHopperLevel[j].channelValue;//���ɹ��ۼ�ʣ����
				print_log("vbillChanged[%d] *remainAmount = %d\r\n",j,*remainAmount);
			}

		}
	}
	
	//����δ����Ľ��
	if(*remainAmount)
	{
		changeAmount = *remainAmount;
		//���԰����ջ������㷽ʽ��������
		print_log("remain.....changeAmount = %d\r\n",changeAmount);
		res = hopperChangerHandle(changeAmount,remainAmount);
		
		if(res)
		{
			return 1;
		}
		else
			return 0;
	}
	
	return 1;
}



/*********************************************************************************************************
** Function name:     	vmcCheckBillChanger
** Descriptions:	     ���ֽ���Ƿ���Զ���
** input parameters:   
** output parameters:   
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/

unsigned char vmcCheckBillChanger(unsigned short changeAmount)
{
	unsigned char i,j;
	for(i= 0;i < HOPPER_NUMS;i++)
	{
		if(changeAmount >= stHopperLevel[i].channelValue)
			return 1;
	}
	if((i < HOPPER_NUMS) && stHopperLevel[i].channelValue)
	{
		for(;i < HOPPER_NUMS;i++)
		{
			if(!stHopperLevel[i].channelValue)
				break;
			for(j =0;j < stHopperLevel[i].num;j++)
			{
				if(!stHopperLevel[i].hopper[j]->state)
					return 1;
			}
		}
	}
	return 0;
	
}


/*********************************************************************************************************
** Function name:     	vmcCheckCoinChanger
** Descriptions:	     ���Ӳ���Ƿ���Զ���
** input parameters:   
** output parameters:   
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/

unsigned char vmcCheckCoinChanger(unsigned short changeAmount)
{
	unsigned char i,j;
	for(i= 0;i < HOPPER_NUMS;i++)
	{
		if(stHopperLevel[i].channelValue)
		{
			if(changeAmount >= stHopperLevel[i].channelValue)
			{	
				return 1;
			}
		}
		else
			break;
	}

	for(;i < HOPPER_NUMS;i++)
	{
		if(!stHopperLevel[i].channelValue)
			break;
		for(j =0;j < stHopperLevel[i].num;j++)
		{
			if(!stHopperLevel[i].hopper[j]->state)
				return 1;
		}
	}
	return 0;

	
}



/*********************************************************************************************************
** Function name:     	vmcCoinChanger
** Descriptions:	    Ӳ�Ҷ����㷨
** input parameters:   
** output parameters:   
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/

unsigned char vmcCoinChanger(unsigned short changeAmount,unsigned int *remainAmount)
{
	unsigned char i;
	unsigned int remainCount = 0,changeCount = 0;

	*remainAmount = changeAmount;
	//Ӳ���������㷨
	for(i = HOPPER_NUMS;i > 0;i--)
	{
		if(stHopperLevel[i - 1].channelValue)
		{
			*remainAmount  = changeAmount % stHopperLevel[i - 1].channelValue;
			 changeCount = changeAmount / stHopperLevel[i - 1].channelValue;
			 print_log("coin level[%d].changeCount = %d \r\n",i-1,changeCount);
			
			 if(hopperOutputByLevel(changeCount, &remainCount,i))
			 {
				if(*remainAmount == 0)
					return 1;		
			 }
			 changeAmount = *remainAmount  + remainCount*stHopperLevel[i - 1].channelValue;
			 *remainAmount = changeAmount;
		}
		
	}
	return (*remainAmount == 0) ? 1 : 0;
	
 
	
}


/*********************************************************************************************************
** Function name:     	hopperChanger
** Descriptions:	    ��������
** input parameters:   
** output parameters:   
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/

uint32_t hopperChanger(uint32_t changedAmount,uint32_t *remainAmount,unsigned char type)
{
	unsigned char mErrFlag = 3,err;
	MAIN_DEV_TASK_MSG *HpBackMsg;
	unsigned int remainVlaue;
	unsigned short timeOut;
	while(mErrFlag)
	{
		task_msg_main_to_dev.value_32 = changedAmount;
		task_msg_main_to_dev.SubSendType = type;
		//���������DevieTask�������Hopper�����Ҳ���
		mbox_post_main_to_dev(TASK_HOPPER_REQ);
		//�ȴ�DeviceTask���񷵻�Ӧ�����䣬�õ�ָ��Hopper�ĳ������
		
		timeOut = getHopperHandleTime(changedAmount);//������С����������㳬ʱʱ��
		HpBackMsg = OSMboxPend(g_msg_dev_to_main,timeOut,&err);
		if(err == OS_NO_ERR)
		{
			
			remainVlaue = HpBackMsg->value_32;
			*remainAmount = remainVlaue;
			return (remainVlaue) ? 0 : 1;
			
		}
		else
		{
			mErrFlag--;
			continue;
		}
	}

	return 0;
}




/*********************************************************************************************************
** Function name:     	hopperChangerFailCheck
** Descriptions:	    ��ѯhopper����ʧ��ԭ��
** input parameters:   
** output parameters:   
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/
unsigned char hopperChangerFailCheck(const unsigned int remainAmount)
{
	unsigned char i;

	for(i = HOPPER_NUMS;i > 0;i--)
	{
		if(stHopperLevel[i - 1].channelValue)
			break;
	}

	print_log("check the mix hopper calue is %d remainAmount is %d\r\n",
		stHopperLevel[i - 1].channelValue,remainAmount);

	if(i && (remainAmount < stHopperLevel[i - 1].channelValue))//������С����С��ֵ��
		return 1;
	else
		return 0;
}


 

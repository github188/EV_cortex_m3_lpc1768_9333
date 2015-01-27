#include "ChannelHandle.h"
#include "..\config.h"
#include "Public.h"


//ͨ�Ű����к�
uint8_t chSn = 1;


//�����������ַ�б�
unsigned char ColumnAdd[8]={0x01,0x04,0x05,0x06,0x02,0x07,0x08,0x09};

const unsigned char ColumnAddr[2][10] = {
	{0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A},
	{0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A}
	};


/*********************************************************************************************
**��   ��   ��: hd_logic_init
**��������˵���������߼��ų�ʼ��
**�������������cabinetNo ���
**����������������
**�� �� ����ֵ����
**********************************************************************************************/

void hd_logic_init(unsigned char cabinetNo)
{
	unsigned char logocNoIndex = 1,i,j,r = 0,c = 0;		
	for(i = 0;i < COLUMN_LEVEL_NUM;i++)
	{
		if(!stHuodao[i].isOpen)
			continue;
		for(j = 0,c = 0;j < COLUMN_ONE_LEVEL_NUM;j++)
		{
			if(!stHuodao[i].stColumn[j].isOpen)
			{
				stHuodao[i].stColumn[j].selectNo = 0;
				stHuodao[i].stColumn[j].logicNo = 0;
				stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;
			}
			else
			{
				if(c >= 9)
					stHuodao[i].stColumn[j].logicNo = (r + 1) * 10;	
				else
					stHuodao[i].stColumn[j].logicNo = (r + 1) * 10 + 1 + c;
				c++;
				stHuodao[i].stColumn[j].selectNo = logocNoIndex++;
				stHuodao[i].stColumn[j].state = HUODAO_STATE_NORMAL;						
			}
		}
		
		if(stHuodao[i].isOpen && c)
			r++;
					
	}
}


/*********************************************************************************************
**��   ��   ��: hd_init
**��������˵�����»�����ʼ������
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
void hd_init(void)
{
	unsigned char i,j,res;

	readHuodaoInfo(TYPE_HUODAO_ALL);
	for(i = 0; i < COLUMN_LEVEL_NUM;i++)
	{
		if(!stHuodao[i].isOpen)  
			continue;
		for(j = 0; j < COLUMN_ONE_LEVEL_NUM;j++)
		{
			if(!stHuodao[i].stColumn[j].isOpen || !stHuodao[i].stColumn[j].logicNo) 
				continue;
			
			res = ChannelHandle(COLUMN_CHECK_EMPTY,stHuodao[i].stColumn[j].logicNo);
			if(res == COLUMN_RST_OK)
			{
				if(stHuodao[i].stColumn[j].remainCount)
					stHuodao[i].stColumn[j].state = HUODAO_STATE_NORMAL;
				else
					stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
			}
			else if(res == COLUMN_RST_EMPTY)
				stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
			else
				stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;

			
				
		}
	}
	
}


/*********************************************************************************************
**��   ��   ��: columnInit
**��������˵�����»�����ʼ������
**�������������flag 0 ��ʼ��  1 ��ȡflash
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
void columnInit(const unsigned char flag)
{
	unsigned char i,j;
	unsigned char level[COLUMN_LEVEL_NUM] = VMC_HUODAO_LEVEL_OPEN;
	unsigned char huodao[COLUMN_LEVEL_NUM][COLUMN_ONE_LEVEL_NUM] = VMC_HUODAO_COLUMN_OPEN;
	//ȫ������ʼ��
	for(i = 0; i < COLUMN_LEVEL_NUM;i++)
	{
#if VMC_HUODAO_OPEN_CONFIG == 1
		stHuodao[i].isOpen = level[i];
#else
		stHuodao[i].isOpen = 1;
#endif	
		
		for(j = 0;j < COLUMN_ONE_LEVEL_NUM;j++)
		{			
			stHuodao[i].stColumn[j].addr = ColumnAddr[i][j];
			stHuodao[i].stColumn[j].physicNo = (i + 1) * 10 + j + 1;	
			stHuodao[i].stColumn[j].logicNo = 0;
			stHuodao[i].stColumn[j].remainCount = 10;
			stHuodao[i].stColumn[j].maxCount = 10;
			stHuodao[i].stColumn[j].tradeCount = 0;
			stHuodao[i].stColumn[j].state = HUODAO_STATE_NORMAL;
			stHuodao[i].stColumn[j].price = 100;
#if VMC_HUODAO_OPEN_CONFIG == 1
			stHuodao[i].stColumn[j].isOpen = huodao[i][j];
#else
			stHuodao[i].stColumn[j].isOpen = 1;
#endif			
			
			Trace("column init..physicNo =%d\r\n",stHuodao[i].stColumn[j].physicNo);
		}
	}
		
	hd_logic_init(1);
	saveHuodaoInfo(TYPE_HUODAO_ALL);
	

	Trace("column init ..over..flag = %d\r\n",flag);

}



/*********************************************************************************************
**��   ��   ��: checkAllColumn
**��������˵��������type���ȫ����
**�������������type ���� 1 ����״̬  2ʣ����  3 ����
**              
**����������������
**�� �� ����ֵ��1 �ɹ�     0ʧ��
**********************************************************************************************/
unsigned char checkAllColumn(unsigned char type)
{
	unsigned char i,j;
	unsigned int value;
	for(i = 0;i < COLUMN_LEVEL_NUM;i++)
	{
		if(!stHuodao[i].isOpen)
			continue;
		for(j = 0;j < COLUMN_ONE_LEVEL_NUM;j++)
		{
			if(!stHuodao[i].stColumn[j].isOpen)
				continue;
			
			if(type == 1)
				value = ((stHuodao[i].stColumn[j].state == HUODAO_STATE_NORMAL) || 
							(stHuodao[i].stColumn[j].state == HUODAO_STATE_EMPTY)) ? 1 : 0 ;	
			else if(type == 2)
				value = ((stHuodao[i].stColumn[j].remainCount != 0) &&
						(stHuodao[i].stColumn[j].state == HUODAO_STATE_NORMAL)) ? 1 : 0;
			else if(type == 3)	
				value = stHuodao[i].stColumn[j].price;
			else
				return 0;
			//Trace("type = %d stHuodao[%d][%d] vlaue = %d\r\n",type,i,j,value);
			if(value)
				return 1;
			
		}
	}

	return 0;
}




/*********************************************************************************************
**��   ��   ��: getColumnIndex
**��������˵��������type ��û����ṹ�������
**�������������type ���� 1 �������������� ��������  2�߼���  3������
**              levelIndex �������  columnIndex ����������
**����������������
**�� �� ����ֵ��1 �ɹ�  0ʧ��
**********************************************************************************************/
unsigned char getColumnIndex(unsigned char *levelIndex,unsigned char *columnIndex,
						unsigned char type,unsigned char no)
{	

	unsigned char i,j,tempNo = 0;
	if(!no)
	{
		Trace("getColumnIndex no is 0...\r\n");
		*levelIndex = 0;
		*columnIndex = 0;
		return 0;
	}
		
	
	for(i =0; i < COLUMN_LEVEL_NUM ;i++)
	{
		for(j = 0;j < COLUMN_ONE_LEVEL_NUM;j++)
		{
			
			if(type == 1)
				tempNo = stHuodao[i].stColumn[j].physicNo;
			else if(type == 2)
				tempNo = stHuodao[i].stColumn[j].logicNo;
			else if(type == 3)
				tempNo = stHuodao[i].stColumn[j].selectNo;
			else 
				tempNo = stHuodao[i].stColumn[j].physicNo;

			//print_huodao("tempNo[%d][%d] = %d  no = %d \r\n",i,j,tempNo,no);
			if(tempNo == no)
			{			
				*levelIndex = i;
				*columnIndex = j;
				return 1;
			}
		}
		
	}

	
	*levelIndex = 0;
	*columnIndex = 0;
	return 0;

	
	
}


/*********************************************************************************************
**��   ��   ��: getColumnInfoByIndex
**��������˵�������������Ż�û����ṹ����Ϣ
**�������������i �������  j ����������
**����������������
**�� �� ����ֵ��1 �ɹ�  0ʧ��
**********************************************************************************************/
unsigned int getColumnInfoByIndex(unsigned char i,unsigned char j,unsigned char type)
{
	unsigned int value;
	switch(type)
	{
		case HUODAO_TYPE_ISOPEN:
			value = stHuodao[i].stColumn[j].isOpen;
			break;
		case HUODAO_TYPE_STATE:
			value = stHuodao[i].stColumn[j].state;
		break;
		case HUODAO_TYPE_REMAIN:
			value = stHuodao[i].stColumn[j].remainCount;
		break;
		case HUODAO_TYPE_PRICE:
			value = stHuodao[i].stColumn[j].price;
		break;
		case HUODAO_TYPE_TRADE_NUM:
			value = stHuodao[i].stColumn[j].tradeCount;
		break;
		
		case HUODAO_TYPE_TRADE_MAXCOUNT:
			value = stHuodao[i].stColumn[j].maxCount;
		break;
		default:
			value  = 0;
			break;
	}


	return value;
	
}

/*********************************************************************************************
**��   ��   ��: setColumnInfoByIndex
**��������˵����ͨ�����������û�����Ϣ
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned char setColumnInfoByIndex(unsigned char i,unsigned char j,
						unsigned char type,unsigned int value)
{
	
	switch(type)
	{
		case HUODAO_TYPE_ISOPEN:
			stHuodao[i].stColumn[j].isOpen = (unsigned char)value;
			break;
		case HUODAO_TYPE_STATE:
			stHuodao[i].stColumn[j].state = (unsigned char)value;
		break;
		case HUODAO_TYPE_REMAIN:
			stHuodao[i].stColumn[j].remainCount = (unsigned char)value;;
		break;
		case HUODAO_TYPE_PRICE:
			stHuodao[i].stColumn[j].price  = (value);
		break;
		case HUODAO_TYPE_TRADE_NUM:
			stHuodao[i].stColumn[j].tradeCount = (unsigned char)value;;
		break;
		case HUODAO_TYPE_TRADE_MAXCOUNT:
			stHuodao[i].stColumn[j].maxCount= (unsigned char)value;;
		break;
		case HUODAO_TYPE_TRADE_LOGICNO:
			stHuodao[i].stColumn[j].logicNo = (unsigned char)value;
		break;
		default:
			
			break;
	}


	return 1;
}




/*********************************************************************************************
**��   ��   ��: fromPhysicGetLogicNum
**��������˵�����»������������ת�߼�������
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned char fromPhysicGetLogicNum(unsigned char cabinetNo,unsigned char physicNo)
{
	unsigned char i,j;
	cabinetNo = cabinetNo;
	if(physicNo && getColumnIndex(&i, &j,1,physicNo))
	{
		return stHuodao[i].stColumn[j].logicNo;
	}
	
	return 0;
}

/*********************************************************************************************
**��   ��   ��: fromLogicNoGetSelectNo
**��������˵�����»������������ת�߼�������
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned char fromLogicNoGetSelectNo(unsigned char cabinetNo,unsigned char logicNo)
{
	unsigned char i,j;
	cabinetNo = cabinetNo;
	if(logicNo && getColumnIndex(&i, &j,2,logicNo))
	{
		return stHuodao[i].stColumn[j].selectNo;
	}
	
	return 0;
}





/*********************************************************************************************
**��   ��   ��: fromSelectNoGetLogicNo
**��������˵����ѡ��������ת�߼�������
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned char fromSelectNoGetLogicNo(unsigned char cabinetNo,unsigned char selectNo)
{
	unsigned char i,j;
	cabinetNo = cabinetNo;
	if(getColumnIndex(&i, &j,3,selectNo))
	{
		return stHuodao[i].stColumn[j].logicNo;
	}
	return 0;
}



/*********************************************************************************************
**��   ��   ��: getLevelOrColumnState
**��������˵����ͨ����������Ż�û�����Ϣ
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned int getColumnInfoByPhysic(unsigned char cabinetNo,unsigned char physicNo,unsigned char type)
{

	unsigned char i,j;

	cabinetNo = cabinetNo;
	if(getColumnIndex(&i, &j,1,physicNo))
	{
		//Trace("getIndex i = %d,j =%d physicNo %d,type = %d\r\n",i,j,physicNo,type);
		return getColumnInfoByIndex(i,j,type);
	}
	return 0;
	
}



/*********************************************************************************************
**��   ��   ��: setColumnInfoByPhysic
**��������˵����ͨ��������������û�����Ϣ
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned char setColumnInfoByPhysic(unsigned char cabinetNo,
						unsigned char physicNo,unsigned int value,unsigned char type)
{

	unsigned char i,j,err;

	cabinetNo = cabinetNo;
	err = getColumnIndex(&i, &j,1,physicNo);
	//Trace("setIndex i = %d,j =%d physicNo %d,type = %d,value =%d\r\n",i,j,physicNo,type,value);	
	if(err)
	{
		return setColumnInfoByIndex(i,j,type,value);
	}
	return 0;

	
}


/*********************************************************************************************
**��   ��   ��: getColumnInfoByLogic
**��������˵����ͨ���߼������Ż�û�����Ϣ  �߼�����ʮ���� 11
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned int getColumnInfoByLogic(unsigned char cabinetNo,unsigned char logicNo,unsigned char type)
{

	
	unsigned char i,j;

	cabinetNo = cabinetNo;
	if(getColumnIndex(&i, &j,2,logicNo))
	{
		return getColumnInfoByIndex(i,j,type);
	}
	return 0;
}



/*********************************************************************************************
**��   ��   ��: setColumnInfoByLogic
**��������˵����ͨ���߼����������û�����Ϣ
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned char setColumnInfoByLogic(unsigned char cabinetNo,
						unsigned char logicNo,unsigned int value,unsigned char type)
{
	unsigned char i,j;

	cabinetNo = cabinetNo;
	if(getColumnIndex(&i, &j,2,logicNo))
	{
		return setColumnInfoByIndex(i,j,type,value);
	}
	return 0;
}

/*********************************************************************************************
**��   ��   ��: getColumnInfoBySelectNo
**��������˵����ͨ���߼������Ż�û�����Ϣ  
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned int getColumnInfoBySelectNo(unsigned char cabinetNo,unsigned char selectNo,unsigned char type)
{

	unsigned char i,j;

	cabinetNo = cabinetNo;
	if(getColumnIndex(&i, &j,3,selectNo))
	{
		return getColumnInfoByIndex(i,j,type);
	}
	return 0;
}



/*********************************************************************************************
**��   ��   ��: setColumnInfoBySelectNo
**��������˵����ͨ���߼����������û�����Ϣ
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned char setColumnInfoBySelectNo(unsigned char cabinetNo,
						unsigned char selectNo,unsigned int value,unsigned char type)
{
	unsigned char i,j;

	cabinetNo = cabinetNo;
	if(getColumnIndex(&i, &j,3,selectNo))
	{
		return setColumnInfoByIndex(i,j,type,value);
	}
	return 0;
}


/*****************************************************************************
** Function name:	hd_open_handle	
** Descriptions:	      �������غ���
**                            ����������¶��������������߼��źţ�
					������������,���¶Ըò�Ļ��������߼��ź�
** parameters:		cabinetNo�������
					no:     ����������/��� 					
					type��1����ĳ����������2����ĳ�����
					flag:	1����������0�����ر�														
** Returned value:	1 �ɹ�  0ʧ��
*******************************************************************************/
unsigned char hd_open_handle(unsigned char cabinetNo,unsigned char no,
								unsigned char type,unsigned char flag)
{
	unsigned char i,j,err,temp;
	cabinetNo = cabinetNo;//Ԥ��
	
	if(type == 2)//�����
	{
		if(no < 1 || no > COLUMN_LEVEL_NUM)
			return 0;
		
		temp = (flag == 1) ? 1 : 0;
		if(stHuodao[no - 1].isOpen == temp)//�˴β������ϼ��㿪���ַ������俪��ʲô�಻��ֱ�ӷ���1
			return 1;
		stHuodao[no - 1].isOpen = temp;
		for(j = 0;j < COLUMN_ONE_LEVEL_NUM;j++)//�ò�����л���ִ����Ӧ�Ŀ��ز���
		{
			if(!stHuodao[no - 1].stColumn[j].addr)
				stHuodao[no - 1].stColumn[j].isOpen = 0;
			else
				stHuodao[no - 1].stColumn[j].isOpen = temp;
		}
		hd_logic_init(cabinetNo);//���������߼���
		return 1;			
	}
	else//Ī��������
	{
		err = getColumnIndex(&i,&j,1,no);
		if(err)
		{
			if(!stHuodao[i].isOpen)//��ر��˲����������������
				return 0;
			if(!stHuodao[i].stColumn[j].addr)//�����ַ�����ڲ��������
			{
				stHuodao[i].stColumn[j].isOpen = 0;
				return 0;
			}
			temp = (flag == 1) ? 1 : 0;
			if(stHuodao[i].stColumn[j].isOpen == temp)
				return 1;
			
			stHuodao[i].stColumn[j].isOpen = temp;
			hd_logic_init(cabinetNo);//���������߼���
			return 1;	
			
			
		}
	}
	
	return 0;
	
}



/*********************************************************************************************
**��   ��   ��:	ChannelHandle
**��������˵����������������
**�������������Handletype���������ͣ�logicNo���ܿػ����߼�������
**����������������
**�� �� ����ֵ��1���ɹ���0������ʧ�ܣ�2 �������� 3�����޻� 4����������5ͨѶʧ��
**********************************************************************************************/
uint8_t ChannelHandle(unsigned char Handletype,unsigned char logicNo)
{
	uint8_t ackdata[36],temp,rcx = 3;
	uint8_t res = 0,i,j;

	Trace("logicNo:%d\r\n",logicNo);
	if(!getColumnIndex(&i, &j, 2,logicNo))
	{
		return COLUMN_RST_N_A;
	}
	while(rcx--)
	{	
		res = EvbConversation(0,0xED,chSn,Handletype,stHuodao[i].stColumn[j].addr,0x00,ackdata);
		if(res == 1)
			break;
		msleep(500);
	}
	
	if(res == 1)
	{
		chSn++;
		temp = ackdata[1];//״̬�ֽ�
		if(Handletype == COLUMN_OUTGOODS || Handletype == COLUMN_CHECK_RST)
		{	
			if(temp & 0x01)
			{
				Trace("COLUMN err status == %x\r\n",temp);
				if(temp & (0x03 << 4))//��������
				{
					return COLUMN_RST_FAULT;
				}
				if(temp & (0x01 << 3))//�����޻�
				{
					return COLUMN_RST_EMPTY;
				}
				return COLUMN_RST_FAULT;
			}
			return COLUMN_RST_OK;
		}
		else if(Handletype == COLUMN_CHECK_EMPTY)
		{
			Trace("COLUMN check status == %x\r\n",temp);
			if(temp)//�޻�
			{
				return COLUMN_RST_EMPTY;
			}
			else  //�л�
			{
				return COLUMN_RST_OK;
			}
		}
		else
			return COLUMN_RST_N_A;
		
	}

	return COLUMN_RST_COM;
	

}


/*********************************************************************************************
**��   ��   ��:	hd_outgoods
**��������˵������������
**�������������logicNo���ܿػ����߼�������
**����������������
**�� �� ����ֵ��1���ɹ���0������ʧ�ܣ�2 �������� 3�����޻� 4����������5ͨѶʧ��
**********************************************************************************************/
uint8_t hd_outgoods(unsigned char logicNo)
{
	unsigned char res,rCheck,i,j;
	if(!getColumnIndex(&i,&j, 2, logicNo))
	{
		return 0;
	}

	//ִ�г�������
	res = ChannelHandle(COLUMN_OUTGOODS,logicNo);
	if(res == COLUMN_RST_OK)
	{
		//ִ�г��������ɹ����ѯ�������
		rCheck = ChannelHandle(COLUMN_CHECK_RST,logicNo);
		if(rCheck == COLUMN_RST_OK)
		{
			if(stHuodao[i].stColumn[j].remainCount)
					stHuodao[i].stColumn[j].remainCount --;
			stHuodao[i].stColumn[j].tradeCount++;	
			
			//�����ɹ���������
			res = ChannelHandle(COLUMN_CHECK_EMPTY,logicNo);
			if(res == COLUMN_RST_OK)
			{
				if(stHuodao[i].stColumn[j].remainCount == 0)
					stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
				else
					stHuodao[i].stColumn[j].state = HUODAO_STATE_NORMAL;
			}
			else if(res == COLUMN_RST_EMPTY)//ȱ��
			{
				stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
			}
			else
				stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;	
			
			return COLUMN_RST_OK;
			
		}
		else//�������ɹ���������
		{
			if(rCheck == COLUMN_RST_EMPTY)
				stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;	
			else
				stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;	
			return rCheck;
		}
			
	}
	else //�������ɹ���������
	{
		if(res == COLUMN_RST_EMPTY)
			stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;	
		else
			stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;	
		return res;
	}
		

}


/*********************************************************************************************
**��   ��   ��:	hd_outgoods_test
**��������˵���������������
**�������������logicNo���ܿػ����߼�������
**����������������
**�� �� ����ֵ��1���ɹ���0������ʧ�ܣ�2 �������� 3�����޻� 4����������5ͨѶʧ��
**********************************************************************************************/
uint8_t hd_outgoods_test(unsigned char logicNo)
{
	unsigned char res,rCheck,i,j;
	if(!getColumnIndex(&i,&j, 2, logicNo))
	{
		return 0;
	}
	//ִ�г�������
	res = ChannelHandle(COLUMN_OUTGOODS,logicNo);
	if(res == COLUMN_RST_OK)
	{
		//ִ�г��������ɹ����ѯ�������
		rCheck = ChannelHandle(COLUMN_CHECK_RST,logicNo);
		if(rCheck == COLUMN_RST_OK)
		{
			//�����ɹ���������
			res = ChannelHandle(COLUMN_CHECK_EMPTY,logicNo);
			if(res == COLUMN_RST_OK)
			{
				if(stHuodao[i].stColumn[j].remainCount == 0)
					stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
				else
					stHuodao[i].stColumn[j].state = HUODAO_STATE_NORMAL;
			}
			else if(res == COLUMN_RST_EMPTY)//ȱ��
			{
				stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
			}
			else
				stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;	
			
			return COLUMN_RST_OK;
		}
		else
		{
			if(rCheck == COLUMN_RST_EMPTY)
			stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;	
			else
				stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;	
			return rCheck;
		}
		

		
	}
	else //�������ɹ���������
	{
		if(res == COLUMN_RST_EMPTY)
			stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;	
		else
			stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;	
		return res;
	}
		

}


/*********************************************************************************************
**��   ��   ��:	hd_check_state
**��������˵��������״̬���
**�������������logicNo���ܿػ����߼�������
**����������������
**�� �� ����ֵ��1���ɹ���0������ʧ�ܣ�2 �������� 3�����޻� 4����������5ͨѶʧ��
**********************************************************************************************/
uint8_t hd_check_state(unsigned char logicNo)
{
	unsigned char res,i,j;
	if(!getColumnIndex(&i,&j, 2, logicNo))
	{
		return COLUMN_RST_N_A;
	}
	//�����ɹ���������
	res = ChannelHandle(COLUMN_CHECK_EMPTY,logicNo);
	if(res == COLUMN_RST_OK)
	{
		if(stHuodao[i].stColumn[j].remainCount == 0)
			stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
		else
			stHuodao[i].stColumn[j].state = HUODAO_STATE_NORMAL;
	}
	else if(res == COLUMN_RST_EMPTY)//ȱ��
	{
		stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
	}
	else
		stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;	
	
	return COLUMN_RST_OK;

	

}




/*********************************************************************************************
**��   ��   ��: hd_outgoods_send
**��������˵�������ͻ���������������
**�������������logicNO�������߼���ַ
**��������������chlstate������״̬
**�� �� ����ֵ��0��ʧ�ܣ�1���ɹ�
**********************************************************************************************/
unsigned char hd_outgoods_send(unsigned char logicNO,unsigned char *chlstate)
{
	uint8_t err,temp;
	MAIN_DEV_TASK_MSG *nMsgPack;

	//���豸�����ͳ�������
	task_msg_main_to_dev.SubSendType = COLUMN_OUTGOODS;
	task_msg_main_to_dev.value_8 = logicNO;
	mbox_post_main_to_dev(TASK_TRADE_REQ);
	//�ȴ������������
	nMsgPack = OSMboxPend(g_msg_dev_to_main,3000,&err);//12s

	Trace("rst = %d sendType = %d\r\n",nMsgPack->value_8,nMsgPack->SendType);
	if(err == OS_NO_ERR && nMsgPack->SendType == TASK_TRADE_RPT)
	{
		temp = nMsgPack->value_8;
		*chlstate = nMsgPack->value_8;	
		return ((temp == COLUMN_RST_OK) ?  1 : 0);
	}
	*chlstate = 0;
	return 0;


		
		
}


#if 0
/*********************************************************************************************
**��   ��   ��: ChannelhandleProcess
**��������˵���������������ƺ���
**�������������handletype���������ͣ�logicNO�������߼���ַ
**��������������chlstate������״̬
**�� �� ����ֵ��0��ʧ�ܣ�1���ɹ�
**********************************************************************************************/
unsigned char ChannelhandleProcess(unsigned char handletype,unsigned char logicNO,unsigned char *chlstate)
{
	uint8_t err,i,j,temp;
	MAIN_DEV_TASK_MSG *nMsgPack;

	if(!getColumnIndex(&i,&j, 2, logicNO))
	{
		*chlstate = COLUMN_RST_N_A;
		return 0;
	}
	
	
	//���豸�����ͳ�������
	task_msg_main_to_dev.SubSendType = handletype;
	task_msg_main_to_dev.value_8 = logicNO;
	mbox_post_main_to_dev(TASK_TRADE_REQ);
	//�ȴ������������
	nMsgPack = OSMboxPend(g_msg_dev_to_main,2500,&err);//12s

	Trace("HandleType==%d rst = %d sendType = %d\r\n",handletype,nMsgPack->value_8,nMsgPack->SendType);
	if(err == OS_NO_ERR && nMsgPack->SendType == TASK_TRADE_RPT)
	{
		temp = nMsgPack->value_8;
		*chlstate = nMsgPack->value_8;	
		if(temp == COLUMN_RST_OK)
		{
			if(handletype == COLUMN_OUTGOODS)
			{
				if(stHuodao[i].stColumn[j].remainCount)
					stHuodao[i].stColumn[j].remainCount --;
				stHuodao[i].stColumn[j].tradeCount++;	
			}
			
			if(stHuodao[i].stColumn[j].remainCount == 0)
				stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
			else
				stHuodao[i].stColumn[j].state = HUODAO_STATE_NORMAL;
			return 1;
		}
		else if(temp == COLUMN_RST_EMPTY)//ȱ��
		{
			stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
		}
		else
			stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;

	}

	return 0;


		
		
}
#endif

/*********************************************************************************************
**��   ��   ��: CheckColumnIsOk
**��������˵�����������Ƿ����  
**�������������handletype���������ͣ�logicNO�������߼���ַ
**��������������chlstate������״̬
**�� �� ����ֵ��0��δ֪ ��1���ɳ��� 2 �������� 3 ����ȱ�� 4 ���������� 5 ���� 6����Ϊ0
**********************************************************************************************/
unsigned char CheckColumnIsOk(unsigned char selectNo,unsigned int remainAmount,unsigned int *price)
{
	unsigned char i,j;
	*price = 0;
	if(!getColumnIndex(&i,&j,3,selectNo))
		return 4;

	*price = stHuodao[i].stColumn[j].price;
	if(!stHuodao[i].stColumn[j].price)
		return 6;

	if(remainAmount < stHuodao[i].stColumn[j].price)	
		return 5;

	if(!stHuodao[i].stColumn[j].remainCount)
		return 3;

	if(stHuodao[i].stColumn[j].state == HUODAO_STATE_EMPTY)
		return 3;
	
	if(stHuodao[i].stColumn[j].state != HUODAO_STATE_NORMAL)
		return 2;
	
	if(!stHuodao[i].stColumn[j].canShip)
		return 0;

	*price = stHuodao[i].stColumn[j].price;
	return 1;
	
	
}


//��ȡ�ĸ����û�����󵥼�
unsigned int getColumnsMaxPrice(void)
{
	unsigned char i,j;
	unsigned int value = 0;
	for(i = 0;i < COLUMN_LEVEL_NUM;i ++)
	{
		if(!stHuodao[i].isOpen)
			continue;
		
		for(j = 0; j < COLUMN_ONE_LEVEL_NUM;j++)
		{
			if(stHuodao[i].stColumn[j].isOpen && (stHuodao[i].stColumn[j].state == HUODAO_STATE_NORMAL))
				value = (value > stHuodao[i].stColumn[j].price)  ? value : stHuodao[i].stColumn[j].price;
		}
	}
	
	return value;
}


/*********************************************************************************************
**��   ��   ��:	ChannelIsEmptyLed
**��������˵����
**�������������
**����������������
**�� �� ����ֵ��
**********************************************************************************************/
void ChannelIsEmptyLed()
{
	if(stHuodao[0].stColumn[1].state==2)
		FIO1CLR |= (1ul<<19); //��1
	else
		FIO1SET |= (1ul<<19);
	if(stHuodao[0].stColumn[0].state==2)
		FIO1CLR |= (1ul<<18); //��2
	else
		FIO1SET |= (1ul<<18);
	if(stHuodao[0].stColumn[3].state==2)
		FIO1CLR |= (1ul<<31);//��3
	else
		FIO1SET |= (1ul<<31);
	if(stHuodao[0].stColumn[2].state==2)
		FIO1CLR |= (1ul<<20);//��4
	else
		FIO1SET |= (1ul<<20);
}




/*********************************************************************************************
**��   ��   ��:	ChannelHandleTest
**��������˵�����������Ժ���
**�������������Handletype���������ͣ�columnNo���ܿػ�����
**����������������
**�� �� ����ֵ��1���ɹ���0������ʧ�ܣ�
**********************************************************************************************/
uint8_t ChannelHandleTest(unsigned char columnNo)
{
	uint8_t ackdata[36];
	uint8_t res = 0,i,j,ClAdd = 0;

	if(!getColumnIndex(&i,&j,3,columnNo))
		return 0;
	ClAdd = stHuodao[i].stColumn[j].addr;
	
	res = EvbConversation(0,0xED,chSn,COLUMN_OUTGOODS,ClAdd,0x00,ackdata);
	if(res == 1)
	{
		chSn++;
		//OSTimeDly(350);
		//EvbConversation(0,0xED,chSn,CHECKCHANNEL,ClAdd,0x00,ackdata);
		if(ackdata[1]&0x01)
		{
			Trace("AckData==%x\r\n",ackdata[1]);
			//����ȱ��
			if(ackdata[1]&0x08)
				stHuodao[i].stColumn[j].state = HUODAO_STATE_EMPTY;
			//��������
			if(ackdata[1]&0x10)
				stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;
			//����ʧ��
			return 0;
		}
		else
		{
			stHuodao[i].stColumn[j].state = HUODAO_STATE_NORMAL;
			//�����ɹ�
			return 1;
		}
		
	}	
	else //û�з���Ӧ���
	{
		stHuodao[i].stColumn[j].state = HUODAO_STATE_FAULT;//ͨ�Ź���
		
	}
	return 0;

}

/*********************************************************************************************
**��   ��   ��: ColumnTestByPcPost
**��������˵�������Ի�����������
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned char ColumnTestByPcPost(unsigned char cabinetNo,unsigned char logicNo)
{
		
	//���豸�����ͳ�������
	task_msg_main_to_dev.SubSendType = COLUMN_OUTGOODS;
	task_msg_main_to_dev.value_8 = logicNo;

	mbox_post_main_to_dev(TASK_TRADE_REQ);
	OSTimeDly(2);
	return 1;
}



/*********************************************************************************************
**��   ��   ��: ColumnTestByPcPend
**��������˵�������Ի������ս��
**���������������
**����������������
**�� �� ����ֵ����
**********************************************************************************************/
unsigned char ColumnTestByPcPend(void)
{
	unsigned char err;
	MAIN_DEV_TASK_MSG *nMsgPack;
	nMsgPack = OSMboxPend(g_msg_dev_to_main,5,&err);
	if(err == OS_NO_ERR)
	{
		if(nMsgPack->value_8 == 1)//�����ɹ�
		{
			return 0;
		}
		else
		{	
			return 1;
		}
			
	}
	return 0xFF;//��ʱ
}



/*****************************************************************************
** Function name:	ChannelSaveParam	
**
** Descriptions:	������Ϣ����
**					
**														 			
** parameters:		��
					
** Returned value:	��
** 
*******************************************************************************/
void ChannelSaveParam()
{
	saveHuodaoInfo(TYPE_HUODAO_ALL);

}


/*****************************************************************************
** Function name:	addColumnGoods	
**
** Descriptions:	�������
**					
**														 			
** parameters:		��
					
** Returned value:	��
** 
*******************************************************************************/
unsigned char addColumnGoods(unsigned cabinetNo,unsigned char type,unsigned char no)
{
	unsigned char i,j;

	cabinetNo = cabinetNo;
	if(type == 1)//ȫ�������
	{
		for(i =0;i < COLUMN_LEVEL_NUM;i++)
		{
			for(j = 0;j < 8;j++)
			{
				stHuodao[i].stColumn[j].remainCount = stHuodao[i].stColumn[j].maxCount;
			}
		}
		
			
	}
	else if(type == 2)//�����
	{
		for(j = 0;j < 8;j++)
		{
			stHuodao[no - 1].stColumn[j].remainCount = stHuodao[no - 1].stColumn[j].maxCount;
		}
	}
	else if(type == 3)//���������
	{
		getColumnIndex(&i,&j,2,no);		
		stHuodao[i].stColumn[j].remainCount = stHuodao[i].stColumn[j].maxCount;
	}

	return 1;

	
	
}



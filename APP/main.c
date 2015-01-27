/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        �������ļ�                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"
#include "SailControl.h"
#include "../API/Public.h"
#include "Global.h"
#include "Menu.h"
#include "LANGUAGE.h"
#include "MAINTAIN.h"



/*********************************************************************************************************
  ����ȫ�ֱ���
*********************************************************************************************************/
#if 1
#define TRADE_BEGIN						(0x01)				//
#define TRADE_DISP						(0x02)				//��ʾ��Ϣ
#define TRADE_HPOUT						(0x03)				//����
#define TRADE_SAVETRADE					(0x04)				//���潻�׼�¼
#define TRADE_BILLIN					(0x05)				//����Ƿ���ֽ�һ�Ӳ�ҽ���
#define TRADE_ISCANOUTGOODS				(0x06)				//����Ƿ���Գ���
#define TRADE_TRADEOVER					(0x07)				//һ�ν����������
#define TRADE_WEIHU						(0x08)				//����ά��ģʽ
#define TRADE_CHECKCHANNEL				(0x12)				//��ѯ����
#define TRADE_OUTGOODS					(0x13)				//����
#define TRADE_SELECTGOODS				(0x14)				//�Ƿ���ѡ������
#define TRADE_CHECKDEVICEERR			(0x15)				//����豸�Ƿ����
#define TRADE_STOPSERVICE				(0x16)				//ֹͣ����
#endif


//MAIN
#define MAIN_START						(1)//���׿�ʼ
#define MAIN_FREE						(2)//���׿���
#define MAIN_CHECK_GOODS				(4)//����ǰ���
#define MAIN_OUTGOODS					(5)//����
#define MAIN_OVER						(6)//���׽���
#define MAIN_WEIHU						(7)//ά��ģʽ
#define MAIN_FAULT						(0x16)//����ģʽ
#define MAIN_PAYOUT						(9)//����



uint8_t GoToMenuFlag = 0;///ά��ģʽ��־


volatile unsigned int TestPluse;
volatile uint16_t WaitMoneyInTimer;
volatile uint32_t WaitMoneyInTimer_1;

uint32_t nMinDispMoney = 0;
unsigned int MaxColumnMoney=0;
//����������
OS_EVENT *g_KeyMsg;






void *KeyMsgArray[2]; // ������������

extern uint16_t HardWareErr;
volatile uint16_t WaitCmdTimer;


/*********************************************************************************************************
  ������ ����ID�����ȼ�����ջ���ü���������                                
*********************************************************************************************************/
#define TASK_Trade_ID                          7                             /*   ����������ID                       */
#define TASK_Trade_PRIO                        TASK_Trade_ID                      /* �������ȼ�                   */
#define TASK_Trade_STACK_SIZE                  1024                           /* �����û���ջ����             */
OS_STK  TASK_Trade_STACK[TASK_Trade_STACK_SIZE];                                  /* ��������B��ջ                */
extern void    TASK_Trade(void *pdata);                                      /* ��������B                    */

/*********************************************************************************************************
  ���� ����ID�����ȼ�����ջ���ü���������                                
*********************************************************************************************************/
#define TASK_DispState_ID                          10                             /* ��������ID                       */
#define TASK_DispState_PRIO                        TASK_DispState_ID                      /* �������ȼ�                   */
#define TASK_DispState_STACK_SIZE                  128                           /* �����û���ջ����             */
OS_STK  TASK_DispState_STACK[TASK_DispState_STACK_SIZE];                                  /* ��������B��ջ                */
extern void    TASK_DispState(void *pdata);   



/*********************************************************************************************************
  �豸 ����ID�����ȼ�����ջ���ü���������                                
*********************************************************************************************************/   
#define TASK_Device_ID                          5                             /* ����ID                       */
#define TASK_Device_PRIO                        TASK_Device_ID                      /* �������ȼ�                   */
#define TASK_Device_STACK_SIZE                  512                           /* �����û���ջ����             */
OS_STK  TASK_Device_STACK[TASK_Device_STACK_SIZE];                                  /* ��������C ��ջ               */



/*********************************************************************************************************
** Function name:       CreateMBox
** Descriptions:        Ϊ����֮��ͨ�Ŵ���������ź���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void CreateMBox(void)
{
	//������������
	g_KeyMsg = OSQCreate(&KeyMsgArray[0],2);
	CreateCommonMBox();
}



static unsigned char GotoWeiHuMenu(const unsigned char state)
{

	GoToMenuFlag = GetInMenuType();
	
	if(GoToMenuFlag == 1)
	{
		Trace("GotoWeiHuMenu...\r\n",GoToMenuFlag);
		deviceEnable(FALSE);
		if(SYSPara.LCDSel == VMC_DISP_LCD)
			MaintainUserProcess();
		else
			UserMenu();
		
	}
	else
	if(GoToMenuFlag == 2)
	{
		deviceEnable(FALSE);
		if(SYSPara.LCDSel == VMC_DISP_LCD)
			MaintainProcess();
		else
			AdminMenu();
	}	
	if(GoToMenuFlag)
	{
		//�˳�ά��ѡ����ʾҳ��
		if(state == MAIN_FAULT)
		{
			DispErrPage();
			deviceEnable(FALSE);
		}	
		else
		{
			DispFreePage(hopperIsEmpty());
			deviceEnable(TRUE);
		}	
		
		GoToMenuFlag = 0;//����ά�� ���־
		return 1;
		
	}
	return 0;
	
	
}

/*********************************************************************************************************
** Function name:       CreateMBox
** Descriptions:        Ϊ����֮��ͨ�Ŵ���������ź���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void setPayKey(const unsigned char flag)
{

	static unsigned char on = 0;
	on = flag;
	
#if 0
	if(flag >= 2)
	{
		FIO0DIR |= (1ul << 30);
		FIO0CLR	|= (1ul << 30);
		on = 0;
		Trace("setPayKey:%d\r\n",flag);
	}
	else
	{
		if(on != flag)
		{
			Trace("setPayKey:%d\r\n",flag);
			FIO0DIR |= (1ul << 30);
			if(flag == 1)
				FIO0SET |= (1ul << 30);
			else
				FIO0CLR	|= (1ul << 30);

			on = flag;
		}		
	}
#endif	

}


/*********************************************************************************************************
** Function name:       trade_task_changer
** Descriptions:        �������������
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void trade_task_changer(void)
{

#if (VMC_TYPE == VMC_TYPE_CHANGER)
	uint8_t err,billtype=0,hopperState = 0,hasBuy = 0,rcx;
	uint32_t moy=0,bValue=0,iouMoney;
	uint8_t CurrentState = 0;
	
	static unsigned int remainAmount = 0,curBillAmount = 0,curCoinAmount = 0;//��ǰʣ����invalue = 0,isBuyGoods = 0,HpOutMoney=0,
	
	//���ճ�ʼ������ź�
	while(1)
	{
		OSSemPend(g_InitSem,1000,&err);
		if(err == OS_NO_ERR)
		{	
			break;
		}	
	}
	CurrentState = TRADE_BEGIN;
	disp_clear_screen();
	//������
	while(1)
	{
		switch(CurrentState)
		{
			//��ʼ����
			case TRADE_BEGIN:
				remainAmount = 0;
				curBillAmount = 0;
				curCoinAmount = 0;		
				hasBuy = 0;
				Timer.sec_usr_op_timer = 10;
				deviceEnable(TRUE);
				CurrentState = TRADE_DISP;
				break;
			case TRADE_DISP:
				//��ʾ��ӭ����
				Trace("huanying.....\r\n");
				disp_free_page(hopperIsEmpty());
				CurrentState = TRADE_BILLIN;
				break;
			case TRADE_BILLIN:
				//��ѯֽ����
				if(SYSPara.BillType)
				{
					if(BillDevIsBillIn(&billtype,&bValue))
					{
						if(bValue != 0)
						{							
							curBillAmount += bValue;
							remainAmount += bValue;
							disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);	
							stTotalTrade.BillMoneySum += bValue;
							saveTradeInfo();
							Timer.sec_usr_op_timer = 60;//
							Timer.sec_changer_timer = 0;//ֽ����ѡ��10s��ʱ
						}	
					}
				}
				
				//��ѯӲ����
				if(SYSPara.CoinType)
				{	
					moy = getCoinMoney();
					if(moy)
					{						
						curCoinAmount += moy;
						remainAmount += moy;
						stTotalTrade.CoinMoneySum += moy;	
						disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);
						saveTradeInfo();
						Timer.sec_usr_op_timer = 60;//
						Timer.sec_changer_timer = 4;//	
					}
				}	
				//����״̬
				if((remainAmount == 0) || hopperChangerFailCheck(remainAmount))
				{
					//��ѯά������
					if(GotoWeiHuMenu(CurrentState))
						disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);
					if(HardWareErr)//�й�������ת����ҳ��
							CurrentState = TRADE_STOPSERVICE;
						else
							CurrentState = TRADE_BILLIN;		
					if(hopperState != hopperIsEmpty())
					{
						hopperState = hopperIsEmpty();
						//ˢ����ҳ��
						disp_free_page(hopperIsEmpty());
					}	
					if(!Timer.sec_usr_op_timer && remainAmount == 0)
					{
						ledPaomaDisplay();
					}
					
				}
				else
				{
					CurrentState = TRADE_BILLIN;	
				}

				//��ѯ���㰴ť
				//��ʱ����
				if(!Timer.sec_changer_timer && remainAmount )
				{
					if(curCoinAmount)//dsfg
					{
						if(vmcCheckCoinChanger(curCoinAmount))
						{
							Trace("duiling.c remainAmount=%d\r\n",remainAmount);
							CurrentState = TRADE_HPOUT; 
						}
					}
					
					if(curBillAmount)//ֽ��������
					{
						if(vmcCheckBillChanger(curBillAmount))
						{
							Trace("duiling.b remainAmount=%d\r\n",remainAmount);
							CurrentState = TRADE_HPOUT; 
						}
					}				
				}	
				break;
			
			case TRADE_HPOUT://����
			
				if(remainAmount)
				{		
					deviceEnable(FALSE);
					disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);//ˢ�½��ҳ��
					//��ʾ��Ҫ����Ľ��
					OSTimeDly(20);
					disp_payout_page();//��������
					if(curBillAmount)//ֽ�Ҷ���
					{
						if(!hopperChanger(curBillAmount, &iouMoney,1))
							curBillAmount = iouMoney;
						else
							curBillAmount = 0;
					}
					
					if(curCoinAmount)//Ӳ�Ҷ���
					{
						if(!hopperChanger(curCoinAmount, &iouMoney,2))
						{
							print_log("coin payout fail iouMoney = %d\r\n",iouMoney);
							curCoinAmount = iouMoney;
						}
						else
							curCoinAmount = 0;
						
					}	
					//�洢������
					stTotalTrade.DispenceSum += remainAmount - curBillAmount - curCoinAmount;
					
					remainAmount = curBillAmount + curCoinAmount;
					curBillAmount = 0;
					curCoinAmount = remainAmount;
					
					print_log("hp.....remainAmount = %d\r\n",remainAmount);
					
					if(remainAmount)//����ʧ��
					{	
						disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);
						if(hopperChangerFailCheck(remainAmount))//hopper���޷����� �ǹ���
						{
							deviceEnable(TRUE);
							CurrentState = TRADE_BILLIN;
						}
						else //����
						{
							print_log("hopper err.....\r\n");
							stTotalTrade.iouAmount += remainAmount;
							CurrentState = TRADE_STOPSERVICE;
						}
						saveTradeInfo();
					}
					else
					{
						disp_qu_change_page();//ȡ��Ǯ
						CurrentState = TRADE_SAVETRADE;
					}	
					
				}	
				else
					CurrentState = TRADE_BEGIN;
				
				
				break;
			case TRADE_SAVETRADE:
				//������ϸ������Ϣ
				saveDetailTradeInfo();
				//���潻����Ϣ
				saveTradeInfo();
				CurrentState = TRADE_TRADEOVER;
				break;
			case TRADE_TRADEOVER:
				CurrentState = TRADE_BEGIN;
				DispEndPage();//���׽���
				break;
			case TRADE_WEIHU:
				GotoWeiHuMenu(CurrentState);
				CurrentState = TRADE_BEGIN;
				break;
			case TRADE_STOPSERVICE:
				GotoWeiHuMenu(CurrentState);
				checkAllDevice();
				if(HardWareErr==SYS_ERR_NO_NORMAL)//�������
				{
					if(remainAmount)
					{
						CurrentState = TRADE_BILLIN;
						disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);
					}
					else
					{
						disp_free_page(hopperIsEmpty());	
						CurrentState = TRADE_BEGIN;
					}	
				}
				else
				{
					if(remainAmount)
					{	
						led_dispaly_off();
						for(rcx = 0;rcx < 10;rcx++)
						{
							GotoWeiHuMenu(CurrentState);
							OSTimeDly(10);
						}
						disp_err_page();
						for(rcx = 0;rcx < 20;rcx++)
						{
							GotoWeiHuMenu(CurrentState);
							OSTimeDly(10);
						}
						led_dispaly_off();
						for(rcx = 0;rcx < 10;rcx++)
						{
							GotoWeiHuMenu(CurrentState);
							OSTimeDly(10);
						}
						disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);
						for(rcx = 0;rcx < 20;rcx++)
						{
							GotoWeiHuMenu(CurrentState);
							OSTimeDly(10);
						}
					}
					else
					{
						disp_err_page();
					}
					
					deviceEnable(FALSE);
					CurrentState = TRADE_STOPSERVICE;
					
				}
				
				break;
			default:
				break;
		}
		
		OSTimeDly(10);
	}	


#endif
}


/*********************************************************************************************************
** Function name:       trade_task_souhuo
** Descriptions:          �ۻ�����������
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void trade_task_souhuo(void)
{
	uint8_t res,err,billtype=0,hopperState = 0,hasBuy = 0;
	uint32_t moy=0,bValue=0,iouMoney;
	uint8_t CurrentState = 0,Cstate;
	static unsigned int remainAmount = 0,curBillAmount = 0,curCoinAmount = 0,columnPrice = 0;//��ǰʣ����invalue = 0,isBuyGoods = 0,HpOutMoney=0,
	static unsigned char payoutEnable = 0,selectKey = 0,columnNo = 0,logicNo = 0,payoutKey = 0;//��������ı�־	

	//���ճ�ʼ������ź�
	while(1)
	{
		OSSemPend(g_InitSem,20000,&err);
		if(err == OS_NO_ERR)
			break;
		OSTimeDly(5);
	}
	Trace("mainTask init over.....\r\n");
	CurrentState = MAIN_START;
	disp_clear_screen();
	
	while(1)
	{
		switch(CurrentState)
		{	
			case MAIN_START://��ʼ����
				remainAmount 	= 0;
				curBillAmount 	= 0;
				curCoinAmount 	= 0;
				selectKey 		= 0;
				columnNo 		= 0;
				logicNo 		= 0;
				hasBuy 			= 0;
				columnPrice 	= 0;
				deviceEnable(TRUE);
				disp_free_page(hopperIsEmpty());//��ʾ��ӭ����	
				CurrentState = MAIN_FREE;					
				break;
			case MAIN_FREE:
				// 1.��ѯֽ����
				if(SYSPara.BillType)
				{
					if(BillDevIsBillIn(&billtype,&bValue))
					{
						stTotalTrade.BillMoneySum += bValue;
						stDetailTradeInfo.billIncome += bValue;
						curBillAmount += bValue;
						remainAmount += bValue;	
						payoutEnable = 0;//һ����ͶǮ �ͱ��빺����������				
						
						if(curBillAmount >= getColumnsMaxPrice())	//�ж�ֽ�����Ƿ����
						{
							deviceEnable(FALSE);
						}
						disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);
						saveTradeInfo();	
					}
				}
				// 2.��ѯӲ����
				if(SYSPara.CoinType)
				{
					moy = getCoinMoney();
					if(moy != 0)
					{
						stTotalTrade.CoinMoneySum += moy;
						stDetailTradeInfo.coinIncome += moy;
						curCoinAmount += moy;
						remainAmount += moy;	
						disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);
						saveTradeInfo();
					}			
				}

				// 3.��ѯ��������
				selectKey = UserSelectKey();

				// 4.��ѯ�˱Ұ���
				payoutKey = ReturnKey();

				// 5.������� ɨ�谴����
				setColumnLedON(remainAmount);

				if(remainAmount)//�����
				{
					if(selectKey)
					{
						columnNo = selectKey;
						CurrentState = MAIN_CHECK_GOODS;
						break;
					}
					if((HardWareErr & SYS_ERR_NO_HUODAO_EMPTY) ||(HardWareErr & SYS_ERR_NO_HUODAO_FAULT))
						payoutEnable = 1;
					//���˱Ұ�ť��Ӧ ������ ����� ������
					if(payoutKey ==1 && payoutEnable)
					{
						CurrentState = MAIN_PAYOUT;					
					}
					
				}
				else//����
				{
					// 6.��ѯά������	
					GotoWeiHuMenu(CurrentState);
					if(selectKey)//�а�����ʾ������Ϣ
					{
						columnPrice = getColumnInfoBySelectNo(1, selectKey,HUODAO_TYPE_PRICE);
						DispColumnPage(selectKey,columnPrice,SYSPara.nPoinValue);//��ʾ�������
						OSTimeDly(1000/5);
						disp_free_page(hopperIsEmpty());//ˢ����ҳ��
					}
					if(HardWareErr != SYS_ERR_NO_NORMAL)//�й���
						CurrentState = MAIN_FAULT;

					if(hopperState != hopperIsEmpty())
					{
						hopperState = hopperIsEmpty();
						disp_free_page(hopperIsEmpty());//ˢ����ҳ��
					}
					
				}
				
				
				break;

			case MAIN_CHECK_GOODS:
				res = CheckColumnIsOk(columnNo,remainAmount,&columnPrice);
				if(res == 1)
				{
					DispColumnPage(columnNo,columnPrice,SYSPara.nPoinValue);//��ʾ�������
					deviceEnable(FALSE);
					msleep(300);
					CurrentState = MAIN_OUTGOODS;
				}
				else if(res == 5)
				{
					DispCannotBuyPage(columnPrice);//��ʾ����
					CurrentState = MAIN_FREE;
					disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);//ˢ�½��ҳ��	

				}
				else
				{	
					CurrentState = MAIN_FREE;//�������ɳ���					
					disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);//ˢ�½��ҳ��	
				}
				break;

			case MAIN_OUTGOODS:
				DispChuhuoPage();//���ڳ���
				logicNo = fromSelectNoGetLogicNo(1,columnNo);//ת��Ϊ�߼�����
				res = hd_outgoods_send(logicNo,&Cstate);
				if(res==1)
				{
					remainAmount -= columnPrice;
					if(curCoinAmount >= columnPrice)//���� ֽ�� ��Ӳ�� ��Ǯ����
					{
						curCoinAmount -=  columnPrice;
					}
					else
					{
						curBillAmount = (curBillAmount - (columnPrice - curCoinAmount));
						curCoinAmount = 0;
					}
					hasBuy = 1;
					payoutEnable = 1;//������Ʒ��������
					stTotalTrade.tradeAmount += columnPrice;
					stTotalTrade.tradeNums++;
					saveOneTradeInfo(columnNo, columnPrice,res);//����һ�γ�����¼
					disp_outgoods_page(1);//��ȡ��
					
				}
				else
				{
					saveOneTradeInfo(columnNo, columnPrice,0);//����һ�γ�����¼	
					disp_outgoods_page(0);
				}
				
				checkDeviceState();//ˢ�¹�����

				if(remainAmount)//һ�γ����� �ж����
				{
					disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);//ˢ�½��ҳ��	
					//��ѯ ֽ�����ձҺ�ѡ���Ƿ� ���������û�������
					if(curBillAmount >= getColumnsMaxPrice())
					{
						deviceEnable(FALSE);
					}
					else
						deviceEnable(TRUE);
					
					CurrentState = MAIN_FREE;
				}	
				else
					CurrentState = MAIN_OVER;
				
				OSQFlush(g_KeyMsg);//�����һ�λ������ ѡ��������Ϣ����
				break;
			case MAIN_PAYOUT://����
				disp_sale_page(hasBuy,remainAmount,SYSPara.nPoinValue);//ˢ�½��ҳ��
				DispPayoutPage();//��������
				deviceEnable(FALSE);
				if(HopperDispence(remainAmount,&iouMoney))//��ʾ��Ҫ����Ľ��
				{
					//����ɹ�
					DispQuChangePage();//ȡ��Ǯ
				}
				else
				{
					//����ʧ��
					DispIOUPage(iouMoney);
					stTotalTrade.iouAmount += iouMoney;
				}	
					
				CurrentState = MAIN_OVER;	
				break;
			case MAIN_OVER:
				saveHuodaoInfo(TYPE_HUODAO_REMAIN | TYPE_HUODAO_TRADE_COUNT);//���������Ϣ			
				msleep(100);
				saveDetailTradeInfo();//������ϸ������Ϣ
				msleep(100);
				saveTradeInfo();	//���潻����Ϣ
				DispEndPage();		//���׽���			
				msleep(300);
				CurrentState = MAIN_START;
				break;
				
			case MAIN_FAULT:
				disp_err_page();
				deviceEnable(FALSE);
				while(1)
				{
					GotoWeiHuMenu(CurrentState);
					//checkAllDevice();
					if(HardWareErr == SYS_ERR_NO_NORMAL)//�������
					{
						disp_free_page(hopperIsEmpty());	
						CurrentState = MAIN_START;
						break;
					}
					else
					{
						CurrentState = MAIN_FAULT;
						//deviceEnable(FALSE);
					}
				}
				break;
			default:
				break;
		}

		msleep(10);
		
	}	
}




/*********************************************************************************************************
** Function name:       TASK_Trade
** Descriptions:       ����������
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void TASK_Trade(void *pvData)
{
	pvData = pvData;

	//ϵͳ�����ӿڳ�ʼ��
	SystemInit();
	//�������䡢�ź���	
	CreateMBox();
	//��������������豸ά������

   //���� �豸 ����Ӳ���� ֽ���� hopper������ ��������
	OSTaskCreateExt(TASK_Device, 
				(void *)0,
				&TASK_Device_STACK[TASK_Device_STACK_SIZE-1],
				TASK_Device_PRIO, 
				TASK_Device_ID, 
				&TASK_Device_STACK[0], 
				TASK_Device_STACK_SIZE, 
				(void *)0,
				OS_TASK_OPT_STK_CHK);

	//������������
	OSTaskCreateExt(TASK_DispState, 
				(void *)0,
				&TASK_DispState_STACK[TASK_DispState_STACK_SIZE-1],
				TASK_DispState_PRIO, 
				TASK_DispState_ID, 
				&TASK_DispState_STACK[0], 
				TASK_DispState_STACK_SIZE, 
				(void *)0,
				OS_TASK_OPT_STK_CHK);

#if VMC_TYPE == VMC_TYPE_TRADE
	pcControlInit();//����PC��������
	trade_task_souhuo();		
#else 
	trade_task_changer();
#endif	
}



void TASK_DispState(void *pdata)
{
#if VMC_TYPE == VMC_TYPE_TRADE
	unsigned char selectKeyIn=0,SelectKey=0;
	while(1)
	{
		SelectKey = IsSelectKeyIn();
		if(SelectKey!=0)
		{
			selectKeyIn = SelectKey;
			OSQPost(g_KeyMsg,(void *)&selectKeyIn);
		}
		OSTimeDly(10);	
	}
#else
	while(1)
	{
		OSTimeDly(1000);
	}
#endif
	

}




/*********************************************************************************************************
** Function name:       main
** Descriptions:          �û�������ں���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int main(void)
{
    targetInit();																//��ʼ��Ŀ���
    pinInit();      															//IO��ʼ��                                                                     
    OSInit();                                                                                                       
	//���� ����������	
	OSTaskCreateExt(TASK_Trade, 
					(void *)0,
					&TASK_Trade_STACK[TASK_Trade_STACK_SIZE-1],
					TASK_Trade_PRIO, 
					TASK_Trade_ID, 
					&TASK_Trade_STACK[0], 
					TASK_Trade_STACK_SIZE, 
					(void *)0,
					OS_TASK_OPT_STK_CHK);

	OSStart();																	//����������
}
/**************************************End Of File*******************************************************/

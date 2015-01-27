#include "Public.h"
#include "..\App\Menu.h"
#include "..\App\SailControl.h"

#define DISABLEBILLDEV					(1)				//����ֽ�����豸
#define ENABLEBILLDEV					(0)				//ʹ��ֽ�����豸

//��ʱ����ʹ��ֽ����
uint16_t ReEnableTime = 0;
//�Ƿ�����豸��־
uint8_t DisableDevFlag = 0;


//����ͨ������ĶԷ�������
OS_EVENT *g_msg_main_to_dev;
OS_EVENT *g_msg_dev_to_main;
MAIN_DEV_TASK_MSG task_msg_main_to_dev;
MAIN_DEV_TASK_MSG task_msg_dev_to_main;



//ֽ����ͨ������
OS_EVENT *g_MoneyIN;
MONEY_RECV_MSG money_recv_msg; 


//��ʼ�������ź���
OS_EVENT *g_InitSem;
//Ӳ����ͨ����Ϣ����
OS_EVENT *g_CoinIn;

volatile unsigned char coin_recv_flag =0;//��Ӳ�ұ�־1��ʾ������Ӳ��


void *coinInNum[G_COIN_IN_SIZE];
void *billInNum[10];

unsigned char CoinIn[G_COIN_IN_SIZE] = {0};


extern void    TASK_Device(void *pdata);   



void CreateCommonMBox()
{
	//�����������豸���������
	g_msg_main_to_dev = OSMboxCreate(NULL);
	//�����豸���񷵻������������
	g_msg_dev_to_main = OSMboxCreate(NULL);
	
	g_MoneyIN = OSQCreate(&billInNum[0],10);
	g_CoinIn = OSQCreate(&coinInNum[0],G_COIN_IN_SIZE);
	//������ʼ�������ź���
	g_InitSem = OSSemCreate(0);
}

/*********************************************************************************************************
** Function name:     	initDeviecByfile
** Descriptions:	   ���������ļ���ʼ���豸
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void initDeviecByfile(void)
{	

	SYSPara.LCDSel = VMC_DISP_SELECT;//ѡ��lcdҺ������ʾ
	SYSPara.BillType = VMC_BILL_TYPE;
	SYSPara.CoinType= VMC_COIN_TYPE;
	SYSPara.DispenceType = VMC_CHANGER_TYPE;
	stDevValue.hpBaseChange = HOPPER_BASE_CHANGED;
	SYSPara.TraceFlag = VMC_TRACE_FLAG;
	SYSPara.nPoinValue = VMC_PIONT_TYPE;
	
#if VMC_OPEN_COINFG == 1
	
	
	stDevValue.CoinValue[0] = VMC_COIN_CHANNEL_1;
	stDevValue.CoinValue[1] = VMC_COIN_CHANNEL_2;
	stDevValue.CoinValue[2] = VMC_COIN_CHANNEL_3;
	stDevValue.CoinValue[3] = VMC_COIN_CHANNEL_4;
	stDevValue.CoinValue[4] = VMC_COIN_CHANNEL_5;
	stDevValue.CoinValue[5] = VMC_COIN_CHANNEL_6;
	stDevValue.CoinValue[6] = VMC_COIN_CHANNEL_7;
	stDevValue.CoinValue[7] = VMC_COIN_CHANNEL_8;

	stDevValue.HpValue[0] = VMC_CHANGER_CHANNEL_1;
	stDevValue.HpValue[1] = VMC_CHANGER_CHANNEL_2;
	stDevValue.HpValue[2] = VMC_CHANGER_CHANNEL_3;
#endif


}




/*********************************************************************************************************
** Function name:       SystemParaInit
** Descriptions:        ϵͳ������ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SystemParaInit()
{
	//ͳһ��flash�ж�ȡ�����й��ۻ����Ĳ���
	readSystemParaFromFlash();//changed by yoc 2014.2.19
	
	//��flash�ж�ȡ�����йص�����
	readTradeInfo();
	initDeviecByfile();

	
}




/*********************************************************************************************************
** Function name:       SystemDevInit
** Descriptions:        ϵͳ�豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SystemDevInit()
{		
	//Ӳ������ʼ��	
	if(SYSPara.CoinType == VMC_COIN_PARALLEL)
	{
		disp_init_page(1);
		InitParallelPluseCoinAcceptor();
	}	
	else if(SYSPara.CoinType == VMC_COIN_SERIAL)
	{
		disp_init_page(1);
		InitSerialPluseCoinAcdeptor();
	}	
	msleep(1000);
	
	//ֽ������ʼ��
	if(SYSPara.BillType)
	{
		disp_init_page(2);
		BillDevInit();
	}
	
	
	//��������ʼ��
	if(SYSPara.DispenceType)
	{
		disp_init_page(3);
		hopperInit();
	}
	msleep(1000);
		
#if VMC_TYPE == VMC_TYPE_TRADE
	disp_init_page(4);
	msleep(2000);//�ȴ������������ȶ�	
	hd_init();//������ʼ��
	Trace("Channel init over.....\r\n");
#endif

	//ʹ�ܼ���
	EnableKeyBoard();

	//ʹ�ܴ��С�����Ӳ����
	if(SYSPara.CoinType == 2)
		DisableParallelPluseCoinAcceptor();
	else if(SYSPara.CoinType == 1)
		DisableSerialPluseCoinAcceptor();
	Trace("BillType = %d,CoinType = %d,changerType = %d,language = %d, lcdSel = %d,nPointValue = %d \r\n",
		SYSPara.BillType,SYSPara.CoinType,SYSPara.DispenceType,
		SYSPara.Language,SYSPara.LCDSel,SYSPara.nPoinValue);
	
}


/*********************************************************************************************************
** Function name:       SystemInit
** Descriptions:        ϵͳ��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SystemInit()
{
	//ע�⸴λ��Uart0ʹ�ܣ�Uart2/3������
	InitUart0();
	InitUart1();
	InitUart2();
	InitUart3();
    InitBuzzer();
	InitI2C0();
	InitTimer(0,240000);
	InitRtc();
	RTCStartOrStop(1);
	//ά��������ʼ��
	InitKeyboard();
	//��ʼ��LCD��ʾ
#if (VMC_DISP_SELECT == VMC_DISP_LCD)
	LCDInit();	
	DispVersionText();//��ʾ�汾��
#else
	InitDisplay();
	led_show(VMC_VERSION);//��ʾ�汾��
	msleep(1500);		
#endif
	//ѡ��������ʼ��
	SelectKeyInit();
	Trace("SelectKey init over.....\r\n");
	
}



/*********************************************************************************************************
** Function name:       allDeviceIsIdle
** Descriptions:        �����豸�Ƿ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char allDeviceIsIdle(void)
{
	if(GoToMenuFlag)
		return 0;
	if(SYSPara.initOK == 0)
		return 0;
	
	//if(coin_recv_flag)
	//	return 0;

	return 1;
	
}

void TASK_Device(void *pdata)
{
	uint8_t err,res=0;
	uint32_t Bvalue = 0;
	uint8_t Btype = 0;
	MAIN_DEV_TASK_MSG *nMsgPack;
	SYSPara.initOK = 0;

	
	//ϵͳ������ʼ��
	SystemParaInit();
	//ϵͳ�豸��ʼ��
	SystemDevInit();	
	OSSemPost(g_InitSem);
	
	Trace("device init over.....\r\n");
	OSTimeDly(100);
	SYSPara.initOK = 1;


	//�豸��������
	while(1)
	{
		//��ѵֽ����
		if(SYSPara.BillType && (DisableDevFlag == ENABLEBILLDEV))//ֽ�������ƺ���
		{	
			BillDevProcess(&Bvalue,&Btype);			
		}
		//�����豸�ʼ�
		nMsgPack = OSMboxPend(g_msg_main_to_dev,20,&err);
		if(err == OS_NO_ERR)
		{
			//Hopper����ָ��
			if(nMsgPack->SendType == TASK_HOPPER_REQ)
			{
#if VMC_TYPE == VMC_TYPE_CHANGER
				if(nMsgPack->SubSendType == 1)
					vmcBillChanger(nMsgPack->value_32, &task_msg_dev_to_main.value_32);
				else
					vmcCoinChanger(nMsgPack->value_32, &task_msg_dev_to_main.value_32);
#else
				hopperChangerHandle(nMsgPack->value_32,&task_msg_dev_to_main.value_32);
#endif			
				//�����ҽ�����͸���������
				task_msg_dev_to_main.SubSendType = nMsgPack->SubSendType;
				mbox_post_dev_to_main(TASK_HOPPER_RPT);
				
			}
			else if(nMsgPack->SendType == TASK_ENABLE_REQ)//���յ�ʹ��ֽ�������ʼ�
			{
				if(SYSPara.BillType)
				{
					BillDevEnable();
					DisableDevFlag=ENABLEBILLDEV;
				}	
				if(SYSPara.CoinType == VMC_COIN_SERIAL)
				{
					EnableSerialPluseCoinAcceptor();
				}
				task_msg_dev_to_main.value_8 = 1;
				//mbox_post_dev_to_main(TASK_ENABLE_RPT);
				print_log("dev Enabled \r\n");
				
			}
			else if(nMsgPack->SendType == TASK_DISABLE_REQ)//���յ�����ֽ�������ʼ�
			{		
				if(SYSPara.BillType)
				{
					BillDevDisable();
					DisableDevFlag= DISABLEBILLDEV;
				}	
				if(SYSPara.CoinType == VMC_COIN_SERIAL)
				{
					DisableSerialPluseCoinAcceptor();
				}
				task_msg_dev_to_main.value_8 = 1;
				//mbox_post_dev_to_main(TASK_DISABLE_RPT);
				print_log("dev Disabled \r\n");
			}
			else if(nMsgPack->SendType == TASK_TRADE_REQ)//��������
			{
				//���ݲ�ͬ��������ͬ�Ĳ���
				
				//res = ChannelHandle(nMsgPack->SubSendType,nMsgPack->value_8);
				res = hd_outgoods(nMsgPack->value_8);
				//������������͸���������
				task_msg_dev_to_main.value_8 = res;
				task_msg_dev_to_main.SubSendType = nMsgPack->SubSendType;
				mbox_post_dev_to_main(TASK_TRADE_RPT);
			}
			
			OSTimeDly(20);
			
		}
		else
		{
			if(!Timer.checkDeviceTimeout && allDeviceIsIdle())//��ѯ�豸״̬
			{
				print_log("checkeStatus.....\r\n");
					//���hopper
				if(SYSPara.DispenceType)
					checkHopperAndUpdate();
				checkDeviceState(); 
				
				Timer.checkDeviceTimeout = 500;//ÿ3���ѯ�豸״̬
				
			}
		}
			
	}	
}


/*********************************************************************************************************
** Function name:       GetKeyInValue
** Descriptions:        �������İ�����ֵ
** input parameters:    ��
** output parameters:   ��
** Returned value:      �а���������Ӧ�ļ�ֵ���ް�������0xff
*********************************************************************************************************/
uint8_t GetKeyInValue()
{
	uint8_t mkey;
	
	mkey = ReadKeyValue();
	if(mkey != 0x00)
	{
		switch(mkey)
		{
			case 0x31:
				return 0x01;
			case 0x32:
				return 0x02;
			case 0x33:
				return 0x03;
			case 0x34:
				return 0x04;
			case 0x35:
				return 0x05;
			case 0x36:
				return 0x06;
			case 0x37:
				return 0x07;
			case 0x38:
				return 0x08;
			case 0x39:
				return 0x09;
			case 0x30:
				return 0x00;
			case 0x44:
				return 0x0c;
			case 0x2e:
				return 0x0d;
			case 0x3c:
				return 0x0a;
			case 0x3e:
				return 0x0b;
			case 0x43:
				return 0x0e;
			case 0x45:
				return 0x0f;
			default:
				return 0xff;
		}
	}
	return 0xff;	
}






/*********************************************************************************************************
** Function name:       mbox_post_main_to_dev
** Descriptions:        �����������䵽�豸����
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char mbox_post_main_to_dev(unsigned char type)
{
	unsigned char rst;
	task_msg_main_to_dev.SendType = type;
	rst = OSMboxPost(g_msg_main_to_dev,&task_msg_main_to_dev);
	OSTimeDly(5);
	return (rst == OS_NO_ERR) ? 1 : 0;
}




/*********************************************************************************************************
** Function name:       mbox_post_dev_to_main
** Descriptions:        �豸��������������䷢�ͺ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char mbox_post_dev_to_main(unsigned char type)
{
	unsigned char rst;
	task_msg_dev_to_main.SendType = type;
	rst = OSMboxPost(g_msg_dev_to_main,&task_msg_dev_to_main);
	OSTimeDly(5);
	return (rst == OS_NO_ERR) ? 1 : 0;
}


/*********************************************************************************************************
** Function name:       checkAllDevice
** Descriptions:        ��������豸
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char checkAllDevice(void)
{
	//���ֽ����
	GetBillDevState();
	checkHopperAndUpdate();//���hopper
	checkDeviceState();//������ˢ��

	return 1;
}




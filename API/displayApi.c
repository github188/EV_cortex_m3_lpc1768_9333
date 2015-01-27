/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           common.h
** Last modified Date:  2014-04-04
** Last Version:        No
** Descriptions:        ϵͳ�������弰ͨ�ú�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          yanbo
** Created date:        2013-04-04
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/

#include "..\config.h"
#include "..\APP\LANGUAGE.h"






/*********************************************************************************************************
** Function name:       disp_clear_screen
** Descriptions:        ���������Զ�ʶ�������
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void initDisplay(void)
{
	if(SYSPara.LCDSel == 1)
		LCDInit();	
	else
		InitDisplay();
}


/*********************************************************************************************************
** Function name:       disp_clear_screen
** Descriptions:        ���������Զ�ʶ�������
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void disp_clear_screen(void)
{
	if(SYSPara.LCDSel == 1)//LCD 4����
	{
		LCDClrScreen();
	}
}

/*********************************************************************************************************
** Function name:       disp_free_page
** Descriptions:       ����ҳ��
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void disp_free_page(unsigned char vmcChangeLow)
{
	if(SYSPara.LCDSel == VMC_DISP_LCD)//LCD 4����
		DispFreePage(vmcChangeLow);
	else
	{
		if(vmcChangeLow)
			DispLayExceptionInfomation(5);
		else
			DisplayDecimalNumberWithDot(0,SYSPara.nPoinValue);
	}
	

	
}





/*********************************************************************************************************
** Function name:       disp_err_page
** Descriptions:        ���������Զ�ʶ�������
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void disp_err_page(void)
{
	
	if(SYSPara.LCDSel == 1)//LCD ����
		DispErrPage();
	else
	{
		if(HardWareErr & SYS_ERR_NO_BILL)
			led_err_display(LED_ERR_BILL);
		else if(HardWareErr & SYS_ERR_NO_HOPPER)//hopper�й���
		{
			if(hopperErrNo & HP_ERR_NO_HOPPER1_FAULT)
				led_show("HP1E");
			else if(hopperErrNo & HP_ERR_NO_HOPPER1_EMPTY)
				led_show("HP10");
			else if(hopperErrNo & HP_ERR_NO_HOPPER2_EMPTY)
				led_show("HP20");
			else if(hopperErrNo & HP_ERR_NO_HOPPER2_FAULT)
				led_show("HP2E");
			else 
				led_show("HPEE");
		}	
		else if(HardWareErr & SYS_ERR_NO_COIN)
			led_err_display(LED_ERR_COIN);
		
	}
}

/*********************************************************************************************************
** Function name:       disp_sale_page
** Descriptions:        �����ʾ
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void disp_sale_page(unsigned char hasBuy,unsigned int amount,unsigned char point)
{
	unsigned int money;
	if(SYSPara.LCDSel == 1)//LCD ����
		DispSalePage(hasBuy,amount,point);
	else
	{
		money = amountTurnByPoint(amount);
		//��ҳ��ʾ
		if(money>=10000)
		{
			DisplayDecimalNumberWithDot(money/10000,0);
			OSTimeDly(200);
			DisplayDecimalNumberWithDot(money%10000,point+3);
		}
		else
			DisplayDecimalNumberWithDot(money,point);
	}
	

}



/*********************************************************************************************************
** Function name:       disp_payout_page
** Descriptions:       ����
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/


void disp_payout_page(void)
{
	if(SYSPara.LCDSel == 1)//LCD ����
		DispPayoutPage();
	else
	{
		
	}
}

/*********************************************************************************************************
** Function name:       disp_qu_change_page
** Descriptions:       ȡ��Ǯҳ��
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void disp_qu_change_page(void)
{
	if(SYSPara.LCDSel == 1)//LCD ����
		DispQuChangePage();
	else
	{
		
	}
}

/*********************************************************************************************************
** Function name:      disp_outgoods_page
** Descriptions:         �������ҳ��
** input parameters:   type :0����ʧ�� 1�����ɹ� ��ȡ��
**					
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void disp_outgoods_page(unsigned char type)
{
	if(type == 0)
		DispChhuoFailPage();
	else
		DispQuhuoPage();	
}


/*********************************************************************************************************
** Function name:     	DispVersionText
** Descriptions:	    ��ʾ�汾�ţ��ȴ��豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispVersionText(void)
{	
	LCDPutLOGOBmp(20,LINE5,0);
	LCDNumberFontPrintf(40,LINE11,2,"Version%d.%02d",1,0);	
}




/*********************************************************************************************************
** Function name:       disp_init_page
** Descriptions:         ��ʼ��ҳ��
** input parameters:   type :1 ��ʼ��Ӳ���� 2 ��ʼ��ֽ����3������  4 ��ʼ������
**					
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void disp_init_page(unsigned char type)
{
		
	switch(type)
	{
		case 1:
			if(SYSPara.LCDSel == VMC_DISP_LED)
			{
				led_display(ledTab[LED_C],ledTab[LED_O],L8_7, L8_7);
			}
			else
			{
				LCDNumberFontPrintf(10,LINE15,1,"Initializing Coin...");	
			}
			break;
		case 2:
			if(SYSPara.LCDSel == VMC_DISP_LED)
				led_display(ledTab[LED_B],ledTab[LED_L],L8_7, L8_7);
			else
				LCDNumberFontPrintf(10,LINE15,1,"Initializing Bill...");
			break;
		case 3:
			if(SYSPara.LCDSel == VMC_DISP_LED)
				led_display(ledTab[LED_H],ledTab[LED_P],L8_7, L8_7);
			else
				LCDNumberFontPrintf(10,LINE15,1,"Initializing Hopper...");
			break;
		case 4:
			if(SYSPara.LCDSel == VMC_DISP_LED)
				led_display(ledTab[LED_C], ledTab[LED_L],L8_7, L8_7);
			else
				LCDNumberFontPrintf(10,LINE15,1,"Initializing Column...");
			break;
		case 99://��ʼ�����
			
			break;
		default:
			break;
	}


	
}








/*********************************************************************************************************
** Function name:     	DispBusinessText
** Descriptions:	    ����ʱ��ʾ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispBusinessText(char *strlanguage,char *streng,char *rightstrlanguage,char *rightstreng)
{
	LCDDrawLine(0,LINE11);
	LCDSpecialPicPrintf(0,LINE12,1,2);//��ͷ	
	LCDPrintf(40,LINE12,0,SYSPara.Language,strlanguage);
	if(SYSPara.Language != 1)
		LCDPrintf(40,LINE14,0,SYSPara.Language,streng);
	if(strlen(rightstrlanguage)!=NULL)
	{
		LCDSpecialPicPrintf(112,LINE12,1,2);//��ͷ	
		LCDPrintf(152,LINE12,0,SYSPara.Language,rightstrlanguage);
		if(SYSPara.Language != 1)
			LCDPrintf(152,LINE14,0,SYSPara.Language,rightstreng);
	}
}


/*********************************************************************************************************
** Function name:     	DispBusinessText
** Descriptions:	    �彻��ʱ��ʾ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void CLrBusinessText()
{	
	LCDClearLineDraw(0,LINE11,1);
	LCDClearLineDraw(0,LINE12,3);	
}


/*********************************************************************************************************
** Function name:     	DispFreePage
** Descriptions:	    ����ҳ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispFreePage(uint8_t vmcChangeLow)
{
	char strlanguage[30] = {0};
	//char strone[30] = {0},strtwo[30] = {0};

	//Trace("\r\n RTC1-Time");
	LCDClrScreen();
	OSTimeDly(2);
	
	if( vmcChangeLow )
	{
		//Trace("\r\n RTC2-Time");
		//RTCReadTime(&RTCData);
		//OSTimeDly(2);
		//1������ʾ
		LCDSpecialPicPrintf(0,LINE2,4,2);//��ͷ 
		strcpy(strlanguage,BUSINESSERROR[1][20]); 
		LCDNumberFontPrintf(40,LINE2,2,strlanguage);
		strcpy(strlanguage,BUSINESSERROR[1][21]); 
		LCDNumberFontPrintf(40,LINE4,1,strlanguage);
		strcpy(strlanguage,BUSINESSERROR[SYSPara.Language][21]);
		LCDPrintf(40,LINE8,0,SYSPara.Language,strlanguage);
		//strcpy(strlanguage,BUSINESSERROR[SYSPara.Language][21]); 
		//LCDNumberFontPrintf(40,LINE6,1,strlanguage);
		//LCDClearLineDraw(0,LINE11,1);
		//2����
		LCDDrawLine(0,LINE11);
		//3ʱ������ 
		//sprintf( strlanguage, "%04d", RTCData.year);
		//LCDNumberFontPrintf(40,LINE12,2,strlanguage);
		//sprintf( strlanguage, "%02d-%02d", RTCData.month,RTCData.day);
		//LCDNumberFontPrintf(40,LINE14,2,strlanguage);
		//sprintf( strlanguage, "%02d:%02d", RTCData.hour,RTCData.minute);
		//LCDNumberFontPrintf(96,LINE12,3,strlanguage);
		
	}
	else
	{
		//RTCReadTime(&RTCData);
		LCDPutLOGOBmp(75,LINE1,2);	
		strcpy(strlanguage,BUSINESS[SYSPara.Language][6]);
		LCDPrintf32(56,LINE7,0,SYSPara.Language,strlanguage);
		//LCDPrintf(84,LINE7,0,SYSPara.Language,strlanguage);
		//LCDNumberFontPrintf(72,LINE13,2,"%04d-%02d-%02d",RTCData.year,RTCData.month,RTCData.day);
		//LCDNumberFontPrintf(92,LINE15,2,"%02d:%02d",RTCData.hour,RTCData.minute);
	}
	//LCDNumberFontPrintf(51,LINE7,1,"0123456789");
	//LCDNumberFontPrintf(51,LINE9,2,"0123456789");
	//LCDNumberFontPrintf(51,LINE11,3,"0123456789");
	//LCDNumberFontPrintf(0,LINE7,4,"01:23.4");
	//LCDNumberFontPrintf(0,LINE13,4,"56789");
}



/*********************************************************************************************************
** Function name:     	DispFreePage
** Descriptions:	    ����ҳ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispErrPage()
{
	char strlanguage[30] = {0};

	//Trace("\r\n RTC2-Time");
	//RTCReadTime(&RTCData);
	OSTimeDly(2);

	LCDClrScreen();
	//1������ʾ
	LCDSpecialPicPrintf(0,LINE2,3,2);//��ͷ 
	strcpy(strlanguage,BUSINESSERROR[SYSPara.Language][19]); 
	//LCDNumberFontPrintf(40,LINE2,2,strlanguage);
	LCDPrintf(40,LINE2,0,SYSPara.Language,strlanguage);
	strcpy(strlanguage,BUSINESSERROR[SYSPara.Language][0]); 
	//LCDNumberFontPrintf(40,LINE4,1,strlanguage);
	LCDPrintf(40,LINE6,0,SYSPara.Language,strlanguage);
	//LCDClearLineDraw(0,LINE11,1);
	//2����
	LCDDrawLine(0,LINE11);
	//3ʱ������ 
	//sprintf( strlanguage, "%04d", RTCData.year);
	//LCDNumberFontPrintf(40,LINE12,2,strlanguage);
	//sprintf( strlanguage, "%02d-%02d", RTCData.month,RTCData.day);
	//LCDNumberFontPrintf(40,LINE14,2,strlanguage);
	//sprintf( strlanguage, "%02d:%02d", RTCData.hour,RTCData.minute);
	//LCDNumberFontPrintf(96,LINE12,3,strlanguage);
	
}



/*********************************************************************************************************
** Function name:     	DispSalePage
** Descriptions:	    Ͷ�ҽ��н���ҳ��
** input parameters:    haveSale�Ƿ���й�һ�ν���,nPoinValue
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispSalePage(uint8_t haveSale,uint32_t invalue,uint8_t nPoinValue)
{
	char strlanguage[30] = {0},streng[30] = {0};
	char rightstrlanguage[30] = {0},rightstreng[30] = {0};
	//char    *pstr;
	//char	strMoney[10];
	uint32_t dispnum;
	
	//LCDClrScreen();
	OSTimeDly(2);
	//g_Amount += moneyValue;
	//pstr = PrintfMoney(g_Amount);	
	//strcpy(strMoney, pstr);
	//Trace("\r\n2.coin=%s",strMoney);
	//Trace("\r\n money=%ld,%d,%ld,%ld",stDevValue.CoinDecimal,stDevValue.CoinScale,stDevValue.BillDecimal,stDevValue.BillScale);
	LCDClrScreen();
#if 0
	if(nPoinValue==0)
		dispnum = invalue*100;
	else if(nPoinValue==1)
		dispnum = invalue*10;
	if(nPoinValue==2)
		dispnum = invalue;
#else
	dispnum = invalue;

#endif


	//Trace("\r\n 5money=%ld",dispnum);
	//��һ�ι���
	if(haveSale==0) 
	{
		//������ʾ
		if(SYSPara.Language != 1)
			LCDPrintf(76,LINE3,0,SYSPara.Language,"%s/%s",BUSINESSDEAL[SYSPara.Language][0],BUSINESSDEAL[1][0]);			
		else
			LCDPrintf(96,LINE3,0,SYSPara.Language,"%s",BUSINESSDEAL[1][0]);

		LCDClearLineDraw(0,LINE5,4);
		//���
		if(SYSPara.Language == 0)
			LCDSpecialPicPrintf(44,LINE6,0,2);//������		
		switch(nPoinValue) 
	    {
	      case 2://�Է�Ϊ��λ
		  	  //sprintf(strnum,"%02d.%02d",dispnum/100,dispnum%100);	
			  LCDNumberFontPrintf(76,LINE5,4,"%02d.%02d",dispnum/100,dispnum%100);	
			  break;

		  case 1://�Խ�Ϊ��λ
		  	  dispnum /= 10;
			  //sprintf(strnum,"%d.%d",dispnum/10,dispnum%10);
			  LCDNumberFontPrintf(76,LINE5,4,"%d.%d",dispnum/10,dispnum%10);	
			  break;
		  
		  case 0://��ԪΪ��λ
			  //sprintf(strnum,"%d",dispnum/100);
			  LCDNumberFontPrintf(76,LINE5,4,"%d",dispnum/100);	
			  break;
	    }	/**/		
		//������ʾ
		strcpy(strlanguage,BUSINESSDEAL[SYSPara.Language][4]); 
		strcpy(streng,BUSINESSDEAL[1][4]);		
		DispBusinessText(strlanguage,streng,"","");
	}
	//��������
	else
	{
		//������ʾ
		if(SYSPara.Language != 1)
			LCDPrintf(76,LINE3,0,SYSPara.Language,"%s/%s",BUSINESSDEAL[SYSPara.Language][1],BUSINESSDEAL[1][1]);			
		else
			LCDPrintf(96,LINE3,0,SYSPara.Language,"%s",BUSINESSDEAL[SYSPara.Language][1]);
		//Trace("\r\n 3money=%ld",dispnum);
		LCDClearLineDraw(0,LINE5,4);
		//���
		if(SYSPara.Language == 0)
			LCDSpecialPicPrintf(44,LINE6,0,2);//������
		switch(nPoinValue) 
	    {
	      case 2://�Է�Ϊ��λ
		  	  //sprintf(strnum,"%02d.%02d",dispnum/100,dispnum%100);	
			  LCDNumberFontPrintf(76,LINE5,4,"%02d.%02d",dispnum/100,dispnum%100);	
			  break;

		  case 1://�Խ�Ϊ��λ
		  	  dispnum /= 10;
			  //sprintf(strnum,"%d.%d",dispnum/10,dispnum%10);
			  LCDNumberFontPrintf(76,LINE5,4,"%d.%d",dispnum/10,dispnum%10);	
			  break;
		  
		  case 0://��ԪΪ��λ
			  //sprintf(strnum,"%d",dispnum/100);
			  LCDNumberFontPrintf(76,LINE5,4,"%d",dispnum/100);	
			  break;
	    }		
		//������ʾ
		strcpy(strlanguage,BUSINESSDEAL[SYSPara.Language][5]); 
		strcpy(streng,BUSINESSDEAL[1][5]);	
		strcpy(rightstrlanguage,BUSINESSDEAL[SYSPara.Language][6]); 
		strcpy(rightstreng,BUSINESSDEAL[1][6]);
		DispBusinessText(strlanguage,streng,rightstrlanguage,rightstreng);
	}
}



/*********************************************************************************************************
** Function name:     	DispColumnPage
** Descriptions:	    ��ʾ���ҳ��
** input parameters:    haveSale�Ƿ���й�һ�ν���,nPoinValue
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispColumnPage(uint8_t  columnNO,uint32_t invalue,uint8_t nPoinValue)
{
	char strlanguage[30] = {0},streng[30] = {0};
	//char    *pstr;
	//char	strMoney[10];
	uint32_t debtMoney;
	
	//LCDClrScreen();
	OSTimeDly(2);
	//g_Amount += moneyValue;
	//pstr = PrintfMoney(g_Amount);	
	//strcpy(strMoney, pstr);
	//Trace("\r\n2.coin=%s",strMoney);
	//Trace("\r\n money=%ld,%d,%ld,%ld",stDevValue.CoinDecimal,stDevValue.CoinScale,stDevValue.BillDecimal,stDevValue.BillScale);
#if 0
	if(nPoinValue==0)
		debtMoney = invalue*100;
	else if(nPoinValue==1)
		debtMoney = invalue*10;
	if(nPoinValue==2)
		debtMoney = invalue;
#else
	debtMoney = invalue;
#endif

	//Trace("\r\n 5money=%ld",dispnum);
	//��һ�ι���
	switch(nPoinValue) 
    {
	      case 2://�Է�Ϊ��λ
		  	  sprintf(strlanguage,"%s %d,%s%02d.%02d",BUSINESS[SYSPara.Language][7],columnNO,BUSINESS[SYSPara.Language][3],debtMoney/100,debtMoney%100);
			  sprintf(streng,"%s ,%s%02d.%02d",BUSINESS[1][7],BUSINESS[1][3],debtMoney/100,debtMoney%100);	
			  break;

		  case 1://�Խ�Ϊ��λ
		  	  debtMoney /= 10;
			  sprintf(strlanguage,"%s %d,%s%d.%d",BUSINESS[SYSPara.Language][7],columnNO,BUSINESS[SYSPara.Language][3],debtMoney/10,debtMoney%10);
			  sprintf(streng,"%s ,%s%d.%d",BUSINESS[1][7],BUSINESS[1][3],debtMoney/10,debtMoney%10);
			  break;
		  
		  case 0://��ԪΪ��λ
			  sprintf(strlanguage,"%s %d,%s%d",BUSINESS[SYSPara.Language][7],columnNO,BUSINESS[SYSPara.Language][3],debtMoney/100);
			  sprintf(streng,"%s ,%s%d",BUSINESS[1][7],BUSINESS[1][3],debtMoney/100);
			  break;
    }	
	//strcpy(streng,BUSINESS[1][3]);
	DispBusinessText(strlanguage,streng,"","");
	OSTimeDly(10);
}

/*********************************************************************************************************
** Function name:     	DispColumnErrPage
** Descriptions:	    ��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispColumnErrPage()
{
	char strlanguage[30] = {0},streng[30] = {0},strlanguage2[30] = {0};
	
	
	strcpy(strlanguage,BUSINESS[SYSPara.Language][4]);
	strcpy(streng,BUSINESS[1][4]);			
	strcat(strlanguage2,strlanguage);
	DispBusinessText(strlanguage2,streng,"","");
	OSTimeDly(OS_TICKS_PER_SEC * 2);
}

/*********************************************************************************************************
** Function name:     	DispCannotBuyPage
** Descriptions:	    �޷�������Ϊ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispCannotBuyPage(const unsigned int invalue)
{
	char strlanguage[30] = {0},streng[30] = {0};
	uint32_t debtMoney;
#if 0	
	if(SYSPara.nPoinValue ==0)
		debtMoney = invalue*100;
	else if(SYSPara.nPoinValue ==1)
		debtMoney = invalue*10;
	if(SYSPara.nPoinValue ==2)
		debtMoney = invalue;
#else
	debtMoney = invalue;
#endif
	switch(SYSPara.nPoinValue) 
    {
	      case 2://�Է�Ϊ��λ
		  	  sprintf(strlanguage,"%s,%s%02d.%02d",BUSINESSDEAL[SYSPara.Language][3],BUSINESS[SYSPara.Language][3],debtMoney/100,debtMoney%100);
			  sprintf(streng,"%s,%02d.%02d",BUSINESSDEAL[1][3],debtMoney/100,debtMoney%100);	
			  break;

		  case 1://�Խ�Ϊ��λ
		  	  debtMoney /= 10;
			  sprintf(strlanguage,"%s,%s%d.%d",BUSINESSDEAL[SYSPara.Language][3],BUSINESS[SYSPara.Language][3],debtMoney/10,debtMoney%10);
			  sprintf(streng,"%s,%d.%d",BUSINESSDEAL[1][3],debtMoney/10,debtMoney%10);
			  break;
		  
		  case 0://��ԪΪ��λ
			  sprintf(strlanguage,"%s,%s%d",BUSINESSDEAL[SYSPara.Language][3],BUSINESS[SYSPara.Language][3],debtMoney/100);
			  sprintf(streng,"%s,%d",BUSINESSDEAL[1][3],debtMoney/100);
			  break;
    }	

	DispBusinessText(strlanguage,streng,"","");
	
	OSTimeDly(OS_TICKS_PER_SEC * 2);


	
}

/*********************************************************************************************************
** Function name:     	DispChuhuoPage
** Descriptions:	    ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispChuhuoPage()
{
	char strlanguage[30] = {0},streng[30] = {0};

	sprintf(strlanguage,"%s",BUSINESSDEAL[SYSPara.Language][11]);
	sprintf(streng,"%s",BUSINESSDEAL[1][11]);
	//strcpy(strlanguage,BUSINESSDEAL[SYSPara.Language][11]); 
	//strcpy(streng,BUSINESSDEAL[1][11]);		
	DispBusinessText(strlanguage,streng,"",""); 
	//OSTimeDly(OS_TICKS_PER_SEC / 5);
}

/*********************************************************************************************************
** Function name:     	DispChhuoFailPage
** Descriptions:	    ����ʧ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispChhuoFailPage(void)
{
	char strlanguage[30] = {0},streng[30] = {0};
	
	strcpy(strlanguage,BUSINESSDEAL[SYSPara.Language][13]); 
	strcpy(streng,BUSINESSDEAL[1][13]);		
	DispBusinessText(strlanguage,streng,"","");	
	OSTimeDly(OS_TICKS_PER_SEC * 2);
}



/*********************************************************************************************************
** Function name:     	DispQuhuoPage
** Descriptions:	    ȡ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispQuhuoPage(void)
{
	char strlanguage[30] = {0},streng[30] = {0};
	
	strcpy(strlanguage,BUSINESSDEAL[SYSPara.Language][12]); 
	strcpy(streng,BUSINESSDEAL[1][12]); 	
	DispBusinessText(strlanguage,streng,"",""); 
	OSTimeDly(OS_TICKS_PER_SEC / 2);
}


/*********************************************************************************************************
** Function name:     	DispPayoutPage
** Descriptions:	    ��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispPayoutPage(void)
{
	char strlanguage[30] = {0},streng[30] = {0};

	//LCDClrScreen();
	/*
	switch(SYSPara.DecimalNum) 
	{
	  case 2://�Է�Ϊ��λ
		  //sprintf(strnum,"%02d.%02d",dispnum/100,dispnum%100);	
		  LCDNumberFontPrintf(76,LINE5,4,"%02d.%02d",0/100,0%100);	
		  break;

	  case 1://�Խ�Ϊ��λ
		  //sprintf(strnum,"%d.%d",dispnum/10,dispnum%10);
		  LCDNumberFontPrintf(76,LINE5,4,"%d.%d",0/10,0%10);	
		  break;
	  
	  case 0://��ԪΪ��λ
		  //sprintf(strnum,"%d",dispnum/100);
		  LCDNumberFontPrintf(76,LINE5,4,"%d",0/100);	
		  break;
	}*/ 
	strcpy(strlanguage,BUSINESSCHANGE[SYSPara.Language][0]); 
	strcpy(streng,BUSINESSCHANGE[1][0]);		
	DispBusinessText(strlanguage,streng,"",""); 
	//OSTimeDly(OS_TICKS_PER_SEC * 2);
}


/*********************************************************************************************************
** Function name:     	DispQuChangePage
** Descriptions:	    ȡ��Ǯ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispQuChangePage(void)
{
	char strlanguage[30] = {0},streng[30] = {0};
	
	strcpy(strlanguage,BUSINESSCHANGE[SYSPara.Language][2]); 
	strcpy(streng,BUSINESSCHANGE[1][2]);		
	DispBusinessText(strlanguage,streng,"",""); 
	OSTimeDly(OS_TICKS_PER_SEC);
}



/*********************************************************************************************************
** Function name:     	DispEndPage
** Descriptions:	    ���׽���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispEndPage(void)
{
	/*
	char strlanguage[30] = {0};
	LCDClrScreen();
	OSTimeDly(2);
	//LCDPutLOGOBmp(75,LINE1,SYSPara.Logo);
	strcpy(strlanguage,BUSINESSCHANGE[1][3]); 
	LCDNumberFontPrintf(48,LINE6,3,strlanguage);
	if(SYSPara.Language != 1)
	{
		//strcpy(strlanguage,BUSINESSCHANGE[SYSPara.Language][3]);
		//LCDPrintf(32,LINE7,0,SYSPara.Language,strlanguage);
		strcpy(strlanguage,BUSINESSCHANGE[SYSPara.Language][4]);
		LCDPrintf(84,LINE12,0,SYSPara.Language,strlanguage);
	}	
	*/
	Pitu_LCDPutPicBmp();
	OSTimeDly(OS_TICKS_PER_SEC / 2);
}


/*********************************************************************************************************
** Function name:     	DispIOUPage
** Descriptions:	    ����ʧ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DispIOUPage(uint32_t debtMoney)
{
	char strlanguage[30] = {0},streng[30] = {0};
#if 0
	if(SYSPara.nPoinValue==0)
		debtMoney = debtMoney*100;
	else if(SYSPara.nPoinValue==1)
		debtMoney = debtMoney*10;
	if(SYSPara.nPoinValue==2)
		debtMoney = debtMoney;
#else
	debtMoney = debtMoney;
#endif	
	switch(SYSPara.nPoinValue) 
    {
      case 2://�Է�Ϊ��λ
	  	  sprintf(strlanguage,"%s%02d.%02d",BUSINESSCHANGE[SYSPara.Language][1],debtMoney/100,debtMoney%100);
		  //LCDNumberFontPrintf(51,LINE5,4,"%02d.%02d",debtMoney/100,debtMoney%100);	
		  break;

	  case 1://�Խ�Ϊ��λ
	  	  debtMoney /= 10;
		  //LCDNumberFontPrintf(51,LINE5,4,"%d.%d",debtMoney/10,debtMoney%10);	
		  sprintf(strlanguage,"%s%d.%d",BUSINESSCHANGE[SYSPara.Language][1],debtMoney/10,debtMoney%10);
		  break;
	  
	  case 0://��ԪΪ��λ
		  //LCDNumberFontPrintf(51,LINE5,4,"%d",debtMoney/100);	
		  sprintf(strlanguage,"%s%d",BUSINESSCHANGE[SYSPara.Language][1],debtMoney/100);
		  break;
    }	
	strcpy(streng,BUSINESSCHANGE[1][1]);	
	//sprintf(streng,"%ld",debtMoney);
	DispBusinessText(strlanguage,streng,"","");	
	OSTimeDly(OS_TICKS_PER_SEC * 5);
}



void ledPaomaDisplay(void)
{
	static unsigned char paomaIndex  = 1;
	//print_log("paoma.....timer = %d\r\n",Timer.led_paoma_timer);
	if(!Timer.led_paoma_timer && SYSPara.LCDSel == VMC_DISP_LED)
	{
		//��ʾ�����
#if 0
		ledPaoMaShow(paomaIndex);
		if(paomaIndex >= 12)
			paomaIndex = 1;
		else
			paomaIndex++;
#endif
		ledPaoMaShow1(paomaIndex);
		if(paomaIndex >= 4)
			paomaIndex = 1;
		else
			paomaIndex++;	

		
		Timer.led_paoma_timer = 500/10;
	}						
	
	
}


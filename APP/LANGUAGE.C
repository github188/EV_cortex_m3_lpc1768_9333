/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           LANGUAGE.C
** Last modified Date:  2013-03-06
** Last Version:        No
** Descriptions:        �˵�����                    
**------------------------------------------------------------------------------------------------------
** Created by:          gzz
** Created date:        2013-03-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"



/*************************************ά����������˵�************************************************/
/*
const char *     Menu_Channel[LANGUAGENO][5]=
{
	 {
	 "1.�������: ",
	 "2.����:     ",
	 "3.��Ʒ����: ",
	 "4.����״̬: ",
	 "5.��Ʒ���: "	 
	 },
	 {
	 "1.Column code:",
	 "2.Price:",
	 "3.Goods remain:",
	 "4.Column status:",
	 "5.Goods NO:"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.����� �ܧ�ݧ�ߧܧ�: ",
	 "2.���֧ߧ�: ",
	 "3.�����ѧ�ܧ� �֧է�: ",
	 "4.����ѧ��� �ܧ�ݧ�ߧܧ�: ",
	 "5. ����ާ֧� �֧է�: "
	 },
	 //��������;by gzz 20121115
	 {
	 	"1 S\xf0\xfctun kodu:",
		"2.Fiyat:",
		"3.Kalan \xf0\xfcr\xf0\xfcn:",
		"4 S\xf0\xfctun  durumu:",
		"5.\xf0\xdcr\xf0\xfcn numaras\xf1\x31:"
	 }
};
*/










/*************************************���׽�����н���************************************************/
const char *     BUSINESS[MAXLANGUAGETYPE][10]=
{
	 {
	 "������Ʒ���:",
	 "�������",
	 "����",
	 "����:",
	 "����",
	 "ȱ��", 
	 "��ӭѡ��",
	 "����",
	  "�����",
	  "��������ֽ��"
	 },
	 {
	 "Input item code:",
	 "Invalid choice",
	 "Disabled",
	 "Price:",
	 "Error",
	 "Empty",
	 "Welcome",
	 "Channel"
	 },
	 {
	 "���ӧ֧էڧ�� �ܧ�� ���٧ڧ�ڧ�:",
	 "���֧��ѧӧڧݧ�ߧ��� �ӧ��ҧ��",
	 "�����ܧݧ��֧ߧ�",
	 "Price:",
	 "����ڧҧܧ�",
	 "�����էѧ��",
	 "Welcome",
	 "���ѧߧѧ�",
	 "Welcome",
	 "���ѧߧѧ�"
	 },
	 {
	 "f0dcrf0fcn kodunu giriniz:",
	 "Girif15f hatalarf131",
	 "Disable",
	 "Price:",
	 "Hatalf131",
	 "Bitti",
	 "Welcome",
	 "Sf0fctun",
	 "Welcome",
	 "���ѧߧѧ�"
	 },
	 {
	 "ENSERIR CODIGO:",
	 "ESCOLHA INVALIDA",
	 "SEM FUNCIONAMENTO",
	 "VALOR:",
	 "ERRO",
	 "VAZIO",
	 "BENVINDO",
	 "BANDEJA",
	 "Welcome",
	 "���ѧߧѧ�"
	 }
};

/*************************************���׽�����Ͻ���************************************************/
const char  *BUSINESSERROR[MAXLANGUAGETYPE][22] =
{
   {
	  "��ͣ����",   
	  "������:	",
	  "��Ʒ������",
	  "����������",
	  "ֽ��������",
	  "Ӳ��������",
	  "�ұ���ȱ��",
	  "���л����޻�",
	  "���л�������",
	  "���������",
	  "����������",
	  "ϵͳ��������",
	  "LCD ����",
	  "���������쳣",
	  "����ȷ�Ϲ���",
	  "Flash����",
	  "ʱ�ӹ���",
	  "ȡ����������",
	  "������û��",
	  "����",
	  "ע��",
	  "�������� ���Ӧ���Ͷ��"
	  
   },
   {	
	  "Sorry, out of service",
	  "Err code: ",
	  "Channel err",
	  "Hopper is wrong",
	  "Bill err",
	  "Coin err",
	  "Hopper empty",
	  "Column empty",
	  "column err",
	  "column err",
	  "column err",
	  "system err",
	  "LCD err",
	  "price err",
	  "GOC err",
	  "Flash err",
	  "RTC err",
	  "FGP err",
	  "Door Open",
	  "Sorry",
	  "Caution",
	  "Unable to Give Change Now"
   },
   //change by liya 2012-10-16
   {
	  "���� ��ѧҧ��ѧ֧�",
	  "����� ���ڧҧܧ�:",
	  "����ڧҧܧ� �ܧѧߧѧݧ�",
	  "������֧� ",
	  "���ڧݧ� ",
	  "����ߧ֧��",
	  "���������ӧڧ� �ӧѧݧ���",
	  "����էѧߧߧ���",
	  "�ߧ֧ڧ���ѧӧߧ����",
	  "�ߧ֧ڧ���ѧӧߧ����",
	  "�ߧ֧ڧ���ѧӧߧ����",
	  "��ѧ�ѧާ֧��� ��ڧ��֧ާ�",
	  "LCD",
	  "���֧ߧ� �ߧ֧��ѧӧڧݧ�ߧ�",
	  "GOC",
	  "Flash",
	  "RTC",
	  "FGP",
	  "�էӧ֧�� �ܧ�ߧ�֧ۧߧ֧��",
	  "�ڧ٧ӧڧߧڧ��",
	  "�ڧ٧ӧڧߧڧ��",
	  "�ڧ٧ӧڧߧڧ��"
   },
   //��������;by gzz 20121115
   {
	  "Servis d\xf1\x31\xf1\x5f\xf1\x31",   
	  "Hata kodu:  ",
	  "\xf0\xdcr\xf0\xfcn sat\xf1\x31ld\xf1\x31",
	  "De\xf1\x1fi\xf1\x5fikli\xf1\x1fi hatas\xf1\x31 tutun",
	  "Bill yetmezli\xf1\x1fi",
	  "Coin hatas\xf1\x31",
	  "Paralar eksik sikke bul",
	  "T\xf0\xfcm g\xf0\xF6nderiler hi\xf0\xe7bir mal road",
	  "T\xf0\xfcm Kargo Karayolu fay",
	  "Kargo Karayolu kart\xf1\x31 ar\xf1\x31zas\xf1\x31",
	  "Kargo Karayolu kullan\xf1\x31lam\xf1\x31yor",
	  "Sistem parametreleri ar\xf1\x31zas\xf1\x31",
	  "LCD ar\xf1\x31za",
	  "Fiyat anormal",
	  "GOC ar\xf1\x31za",
	  "Flash ar\xf1\x31za",
	  "RTC ar\xf1\x31za",
	  "FGP ar\xf1\x31za",
	  "Kap\xf1\x31 kapal\xf1\x31 de\xf1\x1fil Pick",
	  "\xf0\xfczg\xf0\xfcn\xf0\xfcm",
	  "\xf0\xfczg\xf0\xfcn\xf0\xfcm",
	  "\xf0\xfczg\xf0\xfcn\xf0\xfcm"
   },
   {	
	  "DESLIGADO",
	  "CODIGO ERRADO: ",
	  "Channel err",
	  "Hopper is wrong",
	  "Bill err",
	  "Coin err",
	  "Hopper empty",
	  "Column empty",
	  "column err",
	  "column err",
	  "column err",
	  "system err",
	  "LCD err",
	  "price err",
	  "GOC err",
	  "Flash err",
	  "RTC err",
	  "FGP err",
	  "Door Open",
	  "Sorry",
	  "Sorry",
	  "Sorry"
   }
};



/*************************************���׽��潻�׽���************************************************/
const char  *BUSINESSDEAL[MAXLANGUAGETYPE][18] =
{
   {	  
	  "���",
	  "���",
	  "������С���ֽ��",
	  "����",
	  "�밴��ѡ��",
	  "����ѡ��",
	  "�����˱�",
	  "������Ʒ���:",
	  "�������",
	  "����",
	  "ȱ��",
	  "���ڳ���..",
	  "ѡ���ɹ�, ��ȡ��",
	  "��Ʒ������",
	  "��Ǯ����",
	  "���ڿۿ�,���Ժ�..",
	  "�ۿ�ʧ��",
	  "��ʱ:"
	  
   },
   {
   	  "Amount",	
	  "Balance",
	  "Pls insert a smaller bill",
	  "Insufficient fund",
	  "Pls choose the product",
	  "Continue",
	  "Coin refund",
	  "Input Item code:",
	  "Invalid choice",
	  "Error",
	  "Empty",
	  "Dispensing..",
	  "Taking the goods",
	  "Sold out",
	  "Insufficient change",
	  "Debiting, please wait ... ",
	  "Debit failed",
	  "Time:"
	  
	  
   },
   //change by liya 2012-10-16
   {
   	  "���ѧݧѧߧ�",
	  "���ѧݧѧߧ�",
	  "�����ѧӧ��� ���ާ֧ߧ���",
	  "���ѧۧէ֧ߧ� �ߧ֧�����,",
	  "����اѧݧ�ۧ���, �ߧѧاާڧ�� �ܧ��ڧ��",
	  "����է�ݧاѧ��",
	  "CASHOUT"	 ,
	  "���ӧ֧էڧ�� �ܧ�� ���٧ڧ�ڧ�:",
	  "���֧��ѧӧڧݧ�ߧ��� �ӧ��ҧ��",
	  "����ڧҧܧ�",
	  "�����էѧ��",
	  "���ѧ�ӧѧ� �֧է�..",
	  "���ѧҧ֧�ڧ�� �֧է�",
	  "�����էѧߧ�",
	  "���٧ާ֧ߧڧ�� �ߧ֧�ӧѧ�ܧ�",
	  "�է֧ҧ֧��֧���..",
	  "Fail",
	  "Time:"
	   
   },
   //��������;by gzz 20121115
   {
   	  "Bakiye",
	  "Bakiye",
	  "Kucuk banknot girin",
	  "Yetersiz fon,",
	  "xf0xdcrxf0xfcn kodunu giriniz",
	  "Yeni xf0xfcrxf0xfcn",
	  "Para iade",
	  "xf0xdcrxf0xfcn kodunu giriniz:",
	  "Girixf1x5f hatalarxf1x31",
	  "Hatalxf1x31",
	  "Bitti",
	  "xf0xdcrxf0xfcn veriliyor ,bekleyiniz..",
	  "xf0xfcrxf0xfcnxf0xfc alabilirsiniz",
	  "Urun yok",
	  "sxf1x31kxf1x31ntxf1x31sxf1x31 dexf1x1fixf1x5ftirin",
	  "borxf0xe7 olmak..",
	  "Fail",
	  "Time:"
   },
   {
   	  "VALOR",	
	  "SALDO",
	  "POR FAVOR ENSERIR MENOR NOTA",
	  "SALDO INSUFICIENTE,",
	  "ESCOLHA O PRODUTO",
	  "CONTINUE",
	  "TROCO MOEDA",
	  "ENSERIR CODIGO:",
	  "ESCOLHA INVALIDA",
	  "ERRO",
	  "VAZIO",
	  "PREPARANDO PARA SAIR",
	  "PEGAR PRODUTOS",
	  "SEM PRODUTO",
	  "TROCO  INSUFICIENTE",
	  "COBRANDO, OBRIGADO ... ",
	  "FALHA NA COBRAN\x83\A",
	  "TEMPO:"
	  
	  
   }
};

/*************************************���׽����������************************************************/
const char  *BUSINESSCHANGE[MAXLANGUAGETYPE][5] =
{
   {
      "��������",	  
	  "���µ�ͷ�����,Ƿ��:",
	  "��ȡ��Ǯ",
	  "Ʒζ��;���ղؼ���",
	  "лл�ݹ�!"
   },
   {
      "Changing",
	  "IOU:",	  
	  "Please take the changes",
	  "Thank You",
	  " "
   },
   //change by liya	2012-10-16
   {
      "���ѧާ֧ߧ�:",
	  "�� �է�ݧ֧� ���ѧ�: ",	 
	  "���ѧҧ�� �ާ�ߧ֧�",
	  "����ѧ�ڧҧ�",
	  "����ѧ�ڧҧ�"
	  
   },
   //��������;by gzz 20121115
   {
      "Dexf1x1fixf1x5fim:",
	  "Borxf0xe7 makbuzu: ",	 
	  "xf0xfcrxf0xfcnxf0xfc dexf1x1fixf1x5ftirebilirsiniz",
	  "Texf1x5fekkxf0xfcrler",
	  "Texf1x5fekkxf0xfcrler"
   },
   {
      "TROCANDO",
	  "IOU:",	  
	  "POR FAVOR PEGUE O TROCO",
	  "OBRIGADO",
	  "OBRIGADO"
   }
};




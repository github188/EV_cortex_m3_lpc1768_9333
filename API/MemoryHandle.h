#ifndef _MEMORYHANDLE_H
#define _MEMORYHANDLE_H

#if 0
#define ADD_HP1				(10)
#define ADD_HP2				(14)
#define ADD_HP3				(18)
#define ADD_BILL			(22)
#define ADD_HP1VALUE		(26)
#define ADD_HP2VALUE		(30)
#define ADD_HP3VALUE		(34)
#define ADD_BILLVALUE1		(38)
#define ADD_BILLVALUE2		(42)
#define ADD_BILLVALUE3		(46)
#define ADD_BILLVALUE4		(50)
#define ADD_BILLVALUE5		(54)
#define ADD_BILLVALUE6		(58)
#define ADD_BILLVALUE7		(62)
#define ADD_BILLVALUE8		(66)
#define ADD_COINVALUE1		(70)
#define ADD_COINVALUE2		(74)
#define ADD_COINVALUE3		(78)
#define ADD_COINVALUE4		(82)
#define ADD_COINVALUE5		(86)
#define ADD_COINVALUE6		(90)
#define ADD_COINVALUE7		(94)
#define ADD_COINVALUE8		(98)
#define ADD_DEBTMONEYFLAG	(102)
#define ADD_IOUTMONEY		(106)//Ƿ���ܽ��
#define ADD_POINT			(110)
#define ADD_LASTMONEY		(114)
#define ADD_SELFHPSET1		(118)
#define ADD_SELFHPSET2		(122)
#define ADD_SELFHPSET3		(126)
#define ADD_SELFHPSET4		(130)
#define ADD_SELFHPSET5		(134)
#define ADD_SELFHPSET6		(138)
#define ADD_SELFHPSET7		(142)
#define ADD_SELFHPSET8		(146)
#define ADD_CHANNELPRICE	(150)
#define ADD_CHANNELCOUNT	(186)
#define ADD_CHANNELSTATE	(222)
#define ADD_CHANNELTRADECOUNT	(260)

#define ADD_BILLMONEYSUM		(300)
#define ADD_COINMONEYSUM		(330)
#define ADD_DISPMONEYSUM		(360)
#define ADD_MINDISPMONEY		(370)

#endif



//=======================flashҳ��==============================

//flashҳ����󲻳��� 0x1FFF = 8191 ���� flash�ᷭת��ַΪ0x0000
//ϵͳ����д��flash  add by yoc 2014.2.19
#define ADD_SYSTEM_PARA     (256)  //Ԥ��256bytes�ռ�洢
//#define ADD_SYSTEM_PARA_LEN (2) // 32 * 8

//�û������� д��flash��ַ
#define ADD_USER_DATA		(512)
//#define ADD_USER_LEN        (2)//�û����洢���� 256�ֽ�

//������Ϣ�洢
#define ADD_HUODAO_INFO     (768)  //Ԥ��512bytes�ռ�洢
//#define ADD_HUODAO_LEN		(2) //�����洢���� 512�ֽ�

//���׼�¼�洢����
#define ADD_TRADE_INFO      (1280)
#define ADD_TRADE_PAGE_LEN  (64)//ÿ�ʽ��׼�¼���� 64 �ֽ�





//=======================�������ʹ洢==============================
#define TYPE_HUODAO_ADDR			0x0001
#define TYPE_HUODAO_PRICE			0x0002
#define TYPE_HUODAO_STATE		    0x0004
#define TYPE_HUODAO_OPEN_FLAG		0x0008
#define TYPE_HUODAO_REMAIN			0x0010
#define TYPE_HUODAO_TRADE_COUNT		0x0020
#define TYPE_HUODAO_MAX_COUNT		0x0040
#define TYPE_HUODAO_PHYSIC_NO		0x0080
#define TYPE_HUODAO_LEVEL_OPEN      0x0100
#define TYPE_HUODAO_LOGIC      		0x0200

#define TYPE_HUODAO_ALL				0xFFFF

//=====================================================================

/*********************************************************************************************
**��   ��   ��: SaveData
**��������˵����������д���ڴ浱��
**�������������DataType���������ݵĵ�ַҳ��
				nData����Ҫ���������
**����������������
**�� �� ����ֵ��1���ɹ���0��ʧ�ܣ�
**********************************************************************************************/
unsigned char SaveData(unsigned int DataType,unsigned int nData);

/*********************************************************************************************
**��   ��   ��:	GetMemoryData
**��������˵������ȡָ����ַ�ϵ�����
**�������������DataType����ȡ���ݵĵ�ַ
**����������������
**�� �� ����ֵ��1���ɹ���0��ʧ�ܣ�
**********************************************************************************************/
unsigned int GetMemoryData(unsigned int DataType);


unsigned char saveFlash(unsigned int addr,unsigned char *buf,unsigned short len);
unsigned char readFlash(unsigned int addr,unsigned char *buf,unsigned short len);


#endif

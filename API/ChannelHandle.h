#ifndef _CHANNELHANDLE_H
#define _CHANNELHANDLE_H


/*********************************************************************************************
**				������������������������
**********************************************************************************************/

#define COLUMN_OUTGOODS			(0x70)        //��������
#define COLUMN_CHECK_RST		(0x71)			//����ϴγ������
#define COLUMN_CHECK_EMPTY		(0x81)			//����Ƿ��޻�


/*********************************************************************************************
**				�������ͨѶ���ԭ��
**********************************************************************************************/
#define COLUMN_RST_OK		(1)//�����ɹ�
#define COLUMN_RST_FAULT	(2)//��������
#define COLUMN_RST_EMPTY	(3)//����ȱ��
#define COLUMN_RST_N_A		(4)//����������
#define COLUMN_RST_COM		(5)//ͨѶʧ��
#define COLUMN_RST_ZERO		(6)//����Ϊ��




#if 0
#define CHANNEL_OUTGOODS		(1)			//��������
#define CHANNEL_CHECKSTATE		(2)			//��������ǰ״̬
#define CHANNEL_CHECKOUTRESULT	(3)			//�������ϴεĳ������
#define CHANNEL_ISEMPTY			(4)			//������޻�
#define OUTGOODSCMD				(0x70)			//����
#define CHECKCHANNEL			(0x71)			//��ѯ�������
#define CHECKOUTRESULT			(0x81)			//��ѯ��ǰ״̬

#endif


extern unsigned char ChannelHandle(unsigned char Handletype,unsigned char logicNo);

//extern unsigned char ChannelhandleProcess(unsigned char handletype,unsigned char logicNO,unsigned char *chlstate);

extern unsigned char CheckColumnIsOk(unsigned char selectNo,unsigned int remainAmount,unsigned int *price);

extern void ChannelIsEmptyLed(void);
extern unsigned char ChannelHandleTest(unsigned char columnNo);
extern void columnInit(const unsigned char flag);
extern unsigned char fromPhysicGetLogicNum(unsigned char cabinetNo,unsigned char physicNo);
extern unsigned char fromLogicNoGetSelectNo(unsigned char cabinetNo,unsigned char logicNo);
extern unsigned char fromSelectNoGetLogicNo(unsigned char cabinetNo,unsigned char selectNo);

extern unsigned int getColumnInfoByPhysic(unsigned char cabinetNo,unsigned char physicNo,unsigned char type);
unsigned char setColumnInfoByPhysic(unsigned char cabinetNo,
						unsigned char physicNo,unsigned int value,unsigned char type);


extern unsigned int getColumnInfoByLogic(unsigned char cabinetNo,unsigned char logicNo,unsigned char type);
extern unsigned char setColumnInfoByLogic(unsigned char cabinetNo,
						unsigned char logicNo,unsigned int value,unsigned char type);
extern unsigned int getColumnInfoBySelectNo(unsigned char cabinetNo,unsigned char selectNo,unsigned char type);
extern unsigned char setColumnInfoBySelectNo(unsigned char cabinetNo,
						unsigned char selectNo,unsigned int value,unsigned char type);

extern void ChannelSaveParam(void);
extern unsigned char addColumnGoods(unsigned cabinetNo,unsigned char type,unsigned char no);

extern unsigned char ColumnTestByPcPost(unsigned char cabinetNo,unsigned char logicNo);
extern	unsigned char ColumnTestByPcPend(void);
extern	unsigned char getColumnIndex(unsigned char *levelIndex,unsigned char *columnIndex,
						unsigned char type,unsigned char no);

extern	unsigned char checkAllColumn(unsigned char type);
extern unsigned int getColumnsMaxPrice(void);
extern unsigned char getColumnIndex(unsigned char *levelIndex,unsigned char *columnIndex,
						unsigned char type,unsigned char no);
unsigned char hd_open_handle(unsigned char cabinetNo,unsigned char no,
								unsigned char type,unsigned char flag);


void hd_init(void);
unsigned char hd_outgoods_send(unsigned char logicNO,unsigned char *chlstate);
unsigned char hd_outgoods(unsigned char logicNo);
unsigned char hd_check_state(unsigned char logicNo);
unsigned char hd_outgoods_test(unsigned char logicNo);


#endif

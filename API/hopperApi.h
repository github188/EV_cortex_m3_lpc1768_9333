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
#ifndef _HOPPER_H_
#define _HOPPER_H_

//��ǰֻ��Hopper��ָ��������л����Ĳ���ָ���
#define EVB_HANDLE_HP_QUERY				(0x51)				//Hopper�Ĳ�ѯ����ָ��
#define EVB_HANDLE_HP_OUTPUT			(0x50)				//Hopper�ĳ��Ҳ���ָ��
#define HOPPER1_ADDR					0x00
#define HOPPER2_ADDR					0x01
#define HOPPER3_ADDR					0x02


#define HOPPER_NUMS                   3  //hopper����
#define HOPPER_ADDR					{HOPPER1_ADDR,HOPPER2_ADDR,HOPPER3_ADDR}

#define HOPPER_FAULT_NUM         1   //����hopper�����ֵĹ��ϴ���


//����hopper�� �ṹ��
typedef struct __st_hopper_{

    unsigned char addr;//hopper �����ַ
    unsigned char no;//hopper���
    unsigned char state;//hopper״̬  0 ���� 1 ȱ�� 2 ����
    unsigned char channelValue;//ͨ����ֵ
	unsigned char LastOutPutCount;//�ϴβ�����������
	unsigned char levelFlag;//��������־
	unsigned char isCycle;//ѭ������־
	unsigned char specialFault;//������ϱ�־
	


}ST_HOPPER;

extern ST_HOPPER stHopper[HOPPER_NUMS];//��������hopper�� �ṹ������

typedef struct _st_hopper_level_{

    unsigned char num;//������hopper����
    //unsigned char useAbleNum;//��������õ�hopper����
    ST_HOPPER *hopper[HOPPER_NUMS];//����hopper���ṹ��ָ������
    unsigned char channelValue;//������ͨ����ֵ
    unsigned char ratio;//�Աұ�������


}ST_HOPPER_LEVEL;


//����hopper���� �ṹ������ ���������λ��ּ��� 1 > 2 > 3
//���� hopper1 hopper2 ͨ����ֵ 1Ԫ hopper3 ͨ����ֵΪ 5ë �� hopper1 hopper2
//�����ڼ���1��,hopper3 �����ڼ���2��  ����3Ϊ��  ��

extern ST_HOPPER_LEVEL stHopperLevel[HOPPER_NUMS];



void hopperInit(void);
void checkHopperAndUpdate(void);
unsigned char  hopperChangerHandle(unsigned int changeMoney,unsigned int *remainMoney);
uint8_t hopperOutput(unsigned short  count,unsigned char addr);
unsigned int evb_device_hp_dispence(unsigned int ChangeMoney,unsigned int *SurplusMoney,
		unsigned char *Hp1OutNum,unsigned char  *Hp2OutNum,unsigned char *Hp3OutNum)
;
unsigned short getHopperHandleTime(unsigned int value);
unsigned char vmcChangerHandle(unsigned int changedMoney,unsigned int *remainMoney,unsigned char changedType);

unsigned char  vmcBillChanger(unsigned int changeAmount,unsigned int *remainAmount);
unsigned char vmcCoinChanger(unsigned short changeAmount,unsigned int *remainAmount);
unsigned char vmcCheckCoinChanger(unsigned short changeAmount);

unsigned char vmcCheckBillChanger(unsigned short changeAmount);
uint32_t hopperChanger(uint32_t changedAmount,uint32_t *remainAmount,unsigned char type);
uint8_t evb_hopper_check(uint8_t address);
unsigned char hopperChangerFailCheck(const unsigned int remainAmount);
unsigned char getMinHopperPriceState(void);
uint8_t hopperTestOutput(unsigned short  count,unsigned char addr);


#endif


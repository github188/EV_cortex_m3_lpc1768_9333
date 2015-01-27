#include "MemoryHandle.h"
#include "../config.h"
#include "ChannelHandle.h"



/*********************************************************************************************************
** Function name:       SaveData
** Descriptions:        �������ݵ�flash��
** input parameters:    DataType�������������������ͣ�nData����Ҫ�������������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char SaveData(unsigned int DataType,unsigned int nData)
{
	uint8_t res = 0,buf[4]={0};
	uint32_t lData=0;

	lData = nData;
	buf[0] = (uint8_t)(lData>>24);
	buf[1] = (uint8_t)((lData>>16)&0x000000ff);
	buf[2] = (uint8_t)((lData>>8)&0x000000ff);
	buf[3] = (uint8_t)(lData&0x000000ff);
	res = I2C0WrNByte(0xa0,2,DataType,buf,4);
	if(res == 1)
		return 0;
	else
		return 1;
}


/*********************************************************************************************************
** Function name:       saveFlash
** Descriptions:        �������ݵ�flash��
** input parameters:    page��ҳ�ţ�buf����Ҫ�������������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char saveFlash(unsigned int page,unsigned char *buf,unsigned short len)
{
	if(!buf)
		return 0;
	return I2C0WrNByte(0xa0,2,page,buf,len);
}


/*********************************************************************************************************
** Function name:       readFlash
** Descriptions:        ��ȡָ��Flash��ַ�ϵ�����
** input parameters:    addr����ַһ�ι̶���ȡ256�ֽ�
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char readFlash(unsigned int page,unsigned char *buf,unsigned short len)
{
	if(!buf)
		return 0;
	
	return I2C0RdNByte(0xa0,2,page,buf,len);
}



/*********************************************************************************************************
** Function name:       GetMemoryData
** Descriptions:        ��ȡָ��Flash��ַ�ϵ�����
** input parameters:    DataType����ַ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned int GetMemoryData(unsigned int DataType)
{
	uint8_t buf[4];
	uint32_t nData;
		
	I2C0RdNByte(0xa0,2,DataType,buf,4);	
	nData = (uint32_t)((buf[0]<<24)|(buf[1]<<16)|(buf[2]<<8)|(buf[3]));
	return nData;
}









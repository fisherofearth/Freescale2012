#include "i2c.h"

#define MOD_MasterWrite 	0x00  //���豸д
#define MOD_MasterRead   	0x01  //���豸��

#define deviceWrite  DeviceAdress<<1 | 0x00
#define deviceRead   DeviceAdress<<1 | 0x01

void i2c_start() {
	I2C0_C1 |= I2C_C1_TX_MASK;
	I2C0_C1 |= I2C_C1_MST_MASK;
}

void i2c_restart() {
	I2C0_C1 |= I2C_C1_RSTA_MASK;
}

void i2c_stop() {
	 I2C0_C1 &=(~I2C_C1_MST_MASK);
	 I2C0_C1 &=(~I2C_C1_TX_MASK);  
}

void i2c_waitACK() {
	unsigned int time_out = 0;

	while (!(I2C0_S & I2C_S_IICIF_MASK)) {
		if (time_out > 50000) //����ȴ���ʱ��ǿ���˳�
			break;
		else
			time_out++;
	}
	I2C0_S |= I2C_S_IICIF_MASK;
}
void i2c_ShutACK()
{
	I2C0_C1 |= I2C_C1_TXAK_MASK; 
}

void i2c_SendData(unsigned int data){
	I2C0_D = data; 
}

unsigned int i2c_ReadData(){
	return I2C0_D;
}

void i2c_SetMasterRW(unsigned char mode){

	if(mode == MOD_MasterRead) 
	      I2C0_C1 &= (~I2C_C1_TX_MASK);
	    else
	      I2C0_C1 |= ( I2C_C1_TX_MASK);
}

void i2c_StartTransfer(uint8_t addr, uint8_t mode){
	  //I2C����start�ź�
	  i2c_start();
	  //���ӻ���ַ��������дλ�ϳ�һ���ֽ�д��
	  i2c_SendData((addr<<1)|mode);
}


#define deviceWrite  DeviceAdress<<1 | 0x00
#define deviceRead   DeviceAdress<<1 | 0x01
unsigned char i2c_ReadRegister(unsigned char DeviceAdress, unsigned char RegisterAddress){

	unsigned result;

	  //���ʹӻ���ַ
	i2c_StartTransfer(DeviceAdress,MOD_MasterWrite);
	i2c_waitACK();

	  //дMMA7660�Ĵ�����ַ
	i2c_SendData(RegisterAddress);
	i2c_waitACK();

	  //�ٴβ�����ʼ�ź�
	i2c_restart();

	  //���ʹӻ���ַ�Ͷ�ȡλ
	i2c_SendData(deviceRead);
	i2c_waitACK();

	  //ת������ģʽΪ��
	i2c_SetMasterRW(MOD_MasterRead);

	  //�ر�Ӧ��ACK
	i2c_ShutACK();

	  //��IIC����
	  result =	i2c_ReadData();
	  i2c_waitACK();

	  //����ֹͣ�ź�
	  i2c_stop();

	   //��IIC����
	  result = i2c_ReadData();

	  return result;
}


void i2c_WriteRegister(unsigned char DeviceAdress, unsigned char RegisterAddress, unsigned char Data){
	 //���ʹӻ���ַ
	i2c_StartTransfer(DeviceAdress,MOD_MasterWrite);
	i2c_waitACK();
	  
	  //дMMA7660�Ĵ�����ַ
	i2c_SendData(RegisterAddress);
	i2c_waitACK();
	  
	  //��Ĵ�����д��������
	i2c_SendData(Data);
	i2c_waitACK();
	
	i2c_stop();
}















#include "i2c.h"

#define MOD_MasterWrite 	0x00  //主设备写
#define MOD_MasterRead   	0x01  //主设备读

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
		if (time_out > 50000) //如果等待超时，强行退出
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
	  //I2C产生start信号
	  i2c_start();
	  //将从机地址和主机读写位合成一个字节写入
	  i2c_SendData((addr<<1)|mode);
}


#define deviceWrite  DeviceAdress<<1 | 0x00
#define deviceRead   DeviceAdress<<1 | 0x01
unsigned char i2c_ReadRegister(unsigned char DeviceAdress, unsigned char RegisterAddress){

	unsigned result;

	  //发送从机地址
	i2c_StartTransfer(DeviceAdress,MOD_MasterWrite);
	i2c_waitACK();

	  //写MMA7660寄存器地址
	i2c_SendData(RegisterAddress);
	i2c_waitACK();

	  //再次产生开始信号
	i2c_restart();

	  //发送从机地址和读取位
	i2c_SendData(deviceRead);
	i2c_waitACK();

	  //转换主机模式为读
	i2c_SetMasterRW(MOD_MasterRead);

	  //关闭应答ACK
	i2c_ShutACK();

	  //读IIC数据
	  result =	i2c_ReadData();
	  i2c_waitACK();

	  //发送停止信号
	  i2c_stop();

	   //读IIC数据
	  result = i2c_ReadData();

	  return result;
}


void i2c_WriteRegister(unsigned char DeviceAdress, unsigned char RegisterAddress, unsigned char Data){
	 //发送从机地址
	i2c_StartTransfer(DeviceAdress,MOD_MasterWrite);
	i2c_waitACK();
	  
	  //写MMA7660寄存器地址
	i2c_SendData(RegisterAddress);
	i2c_waitACK();
	  
	  //向寄存器中写具体数据
	i2c_SendData(Data);
	i2c_waitACK();
	
	i2c_stop();
}















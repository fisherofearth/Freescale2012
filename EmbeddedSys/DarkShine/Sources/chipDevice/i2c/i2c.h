#ifndef __I2C_H__
#define __I2C_H__

#include "derivative.h" /* include peripheral declarations */

#include "i2c.h"
/*
extern void i2c_start();
extern void i2c_restart();
extern void i2c_stop();
extern void i2c_waitACK();
extern void i2c_SendData(unsigned int data);
extern unsigned int i2c_ReadData();
extern void i2c_SetMasterRW(unsigned char mode);
extern void i2c_StartTransfer(uint8_t addr, uint8_t mode);
*/
extern unsigned char i2c_ReadRegister(unsigned char DeviceAdress, unsigned char RegisterAddress);
extern void i2c_WriteRegister(unsigned char DeviceAdress, unsigned char RegisterAddress, unsigned char Data);

	
#endif
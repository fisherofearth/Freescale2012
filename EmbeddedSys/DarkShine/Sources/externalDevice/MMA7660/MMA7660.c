#include "MMA7660.h"
//#include  "chipDevice/i2c/i2c.h"

//==========MMA7660 寄存器地址==================//
#define MMA7660_XOUT  0x00   // 6-bit output value X 
#define MMA7660_YOUT  0x01   // 6-bit output value Y 
#define MMA7660_ZOUT  0x02   // 6-bit output value Z
#define MMA7660_TILT  0x03   // Tilt Status 
#define MMA7660_SRST  0x04   // Sampling Rate Status
#define MMA7660_SPCNT 0x05   // Sleep Count
#define MMA7660_INTSU 0x06   // Interrupt Setup
#define MMA7660_MODE  0x07   // Mode
#define MMA7660_SR    0x08   // Auto-Wake/Sleep and 
                      // Portrait/Landscape samples 
                      // per seconds and Debounce Filter
#define MMA7660_PDET  0x09   // Tap Detection
#define MMA7660_PD    0x0A   // Tap Debounce Count
//=========MMA7660 功能参数==================//
#define MMA7660_DEV_ADDR   0x4C //Normally,can range 0x08 to 0xEF
#define MMA7660_DEV_WRITE  MMA7660_DEV_ADDR<<1 | 0x00
#define MMA7660_DEV_READ   MMA7660_DEV_ADDR<<1 | 0x01

void MMA7660_InitialDevice(){
	i2c_WriteRegister(MMA7660_DEV_ADDR, MMA7660_MODE, 0x00); //Disable tap detection
	i2c_WriteRegister(MMA7660_DEV_ADDR, MMA7660_SR, 0x02);   //Enable auto-sleep, auto-wake, and put in active mode
	i2c_WriteRegister(MMA7660_DEV_ADDR, MMA7660_MODE, 0x11); //Disable tap detection
}

unsigned char MMA7660_X()
{
	return i2c_ReadRegister(MMA7660_DEV_ADDR,MMA7660_XOUT);
}
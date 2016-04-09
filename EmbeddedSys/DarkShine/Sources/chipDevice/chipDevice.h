#ifndef __CHIP_DEVICE_H__
#define __CHIP_DEVICE_H__

//1.Include head files
#include "uart/uart.h"
#include "spi/spi.h"
//#include "i2c/i2c.h"


//2.Define types

//3.Define interfaces

//4.Redefine public functions
#define	MCU_enableIRQPortA() 	NVICICPR2 = 1 << (87 % 32);NVICISER2 = 1 << (87 % 32)//IRQ PORTA中断
#define	MCU_enableIRQPortB() 	NVICICPR2 = 1 << (88 % 32);NVICISER2 = 1 << (88 % 32)//IRQ PORTB中断
#define	MCU_enableIRQPortC() 	NVICICPR2 = 1 << (89 % 32);NVICISER2 = 1 << (89 % 32)//IRQ PORTC中断
#define	MCU_enableIRQPortD() 	NVICICPR2 = 1 << (90 % 32);NVICISER2 = 1 << (90 % 32)//IRQ PORTD中断
#define	MCU_enableIRQPortE() 	NVICICPR2 = 1 << (91 % 32);NVICISER2 = 1 << (91 % 32)//IRQ PORTE中断

#define	MCU_enableIRQPIT0()		NVICICPR2 = 1 << (68 % 32);NVICISER2 = 1 << (68 % 32)//IRQ PIT0中断
#define	MCU_enableIRQPIT1()		NVICICPR2 = 1 << (69 % 32);NVICISER2 = 1 << (69 % 32)//IRQ PIT1中断
#define	MCU_enableIRQPIT2()		NVICICPR2 = 1 << (70 % 32);NVICISER2 = 1 << (70 % 32)//IRQ PIT2中断

#define	MCU_enableIRQUART2()	NVICICPR1 = 1 << (49 % 32);NVICISER1 = 1 << (49 % 32)//IRQ uart2中断



#define MCU_enableIQR() 	MCU_enableIRQPortA();\
							MCU_enableIRQPortC();\
							MCU_enableIRQPIT0();\
							MCU_enableIRQPIT1();\
							MCU_enableIRQPIT2();\
							MCU_enableIRQUART2()

#define MCU_EnablePITInterrupt0() 	PIT_TCTRL0 |=  PIT_TCTRL_TIE_MASK
#define MCU_DisablePITInterrupt0() 	PIT_TCTRL0 &= ~PIT_TCTRL_TIE_MASK

#define MCU_EnablePITInterrupt1() 	PIT_TCTRL1 |=  PIT_TCTRL_TIE_MASK
#define MCU_DisablePITInterrupt1() 	PIT_TCTRL1 &= ~PIT_TCTRL_TIE_MASK

#define MCU_EnablePITInterrupt2() 	PIT_TCTRL2 |=  PIT_TCTRL_TIE_MASK
#define MCU_DisablePITInterrupt2() 	PIT_TCTRL2 &= ~PIT_TCTRL_TIE_MASK

#define MCU_EnablePITInterrupt3() 	PIT_TCTRL3 |=  PIT_TCTRL_TIE_MASK
#define MCU_DisablePITInterrupt3() 	PIT_TCTRL3 &= ~PIT_TCTRL_TIE_MASK

//5.Redefine public variable


//6.Define private parametera


#endif

#ifndef __POWERMANAGEMENT_H__
#define __POWERMANAGEMENT_H__


//1.Include the head files
#include "derivative.h" /* include peripheral declarations */

//2.Define the types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces
#define 	PORT_PM	GPIOD_PDOR
#define 	PM_SevroDirection	(0x00000001 << 10)

//4.Redefine the functions
extern void PowerSwitch_SevroDirection(uchar s);

#define power_on 	1
#define power_off 	0

#endif

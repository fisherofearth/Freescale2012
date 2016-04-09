#ifndef __INTEGRAL_H__
#define __INTEGRAL_H__


//1.Include head files
#include "MCUinit.h"

//2.Define types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces

//4.Redefine public functions
extern void IntegralClock(void);

//5.Redefine public variable
extern uint 	integralPeriod;
extern uchar 	F_integral;

//6.Define private parametera


#endif

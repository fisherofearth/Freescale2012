#ifndef __SPEEDCONTROL_H__
#define __SPEEDCONTROL_H__

//1.Include the head files
#include  	"externalDevice/externalDevice.h"
#include 	"MCUinit.h"

//2.Define the types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces

//4.Redefine the functions
extern void ClockSpeedControl();
extern void ControlSpeed_motor(int D_Speed, int myMaxPower, int myMinPower);

//extern uchar F_speedControlClock;


extern int C_pulse_Encode;
extern int D_speedNow;
extern int minCutveSpeedRate;
extern long D_speedOutput; 
extern long D_speedIntegration;



#define	ABS(x) 			((x)<0?-(x):(x))
#endif

#ifndef _BEEP_H_
#define _BEEP_H_

//1.Include the head files
#include "derivative.h" /* include peripheral declarations */

//2.Define types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces
#define beep_I_control   	GPIOB_PDOR 	//define the beep use which port of MCU
#define beep_Pn_control		(((ulong)0x01)<<(9)) 	//BIT 9
	
//4.Redefine public functions
extern void SwitchBeep_beep(uchar s);

//5.Redefine public variable
extern signed int beep_F_enableBeep;

//6.Define private parameter
#define 	Pr_on_beep 		1
#define 	Pr_off_beep 	0



#endif

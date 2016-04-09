#ifndef _OV7620_H_
#define _OV7620_H_


//1.Include the head files
#include "derivative.h" /* include peripheral declarations */
#include "MCUinit.h"

//2.Define the types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces

#define ov7620_I_RESET 	GPIOC_PDOR
#define ov7620_Pn_RESET	(0x00000001 << 8)

//4.Redefine the functions
extern void ov7620_Init(void);
extern void ov7620_FlagVSYNC();


extern uchar ov7620_F_VSYNC;


#endif



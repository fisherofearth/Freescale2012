#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//1.Include the head files
#include "derivative.h" /* include peripheral declarations */
#include "MCUinit.h"




//2.Define types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces
#define keyboard_I_input 	((GPIOA_PDIR >> 10) & (0x0000000f))

/*
 #define keyboard_Pn_upButton  	13
 #define keyboard_Pn_downButton  12
 #define keyboard_Pn_enterButton 10
 #define keyboard_Pn_closeButton 11
 */


//4.Redefine public functions
extern int keyboard_ScanKeyboard(void);
extern void keyboard_TimerKeyPressed(void);
extern void keyboard_InputClose(void);
#define keyboard_EnableLonePress() 	MCU_EnablePITInterrupt0()

//5.Redefine public variable
extern uchar keyboard_D_input;
extern uchar keyboard_F_buttonCloseInterrupt;

//6.Define private parameter
#define keyboard_S_upPressed 		1
#define keyboard_S_downPressed 		2
#define keyboard_S_enterPressed  	3
#define keyboard_S_closePressed 	4

#endif

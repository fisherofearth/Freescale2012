#ifndef __OLED_H__
#define __OLED_H__

//1.Include the head files
#include "derivative.h" /* include peripheral declarations */


//2.Define types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces
#define oled_PORT_SDIN   	GPIOB_PDOR 	
#define oled_PORT_SCLK		GPIOA_PDOR
#define oled_PORT_DC		GPIOA_PDOR
#define oled_PORT_RST		GPIOB_PDOR
#define oled_PORT_CS		GPIOC_PDOR

#define oled_BIT_SDIN    	((0x00000001)<<(20))
#define oled_BIT_SCLK		((0x00000001)<<(26))
#define oled_BIT_DC			((0x00000001)<<(28))
#define oled_BIT_RST		((0x00000001)<<(22))
#define oled_BIT_CS			((0x00000001)<<( 0))



//4.Redefine public functions
extern void oled_InitDevice(void);
extern void oled_ClearScreen(void);
extern void oled_SetXY(unsigned char X, unsigned char Y);
extern void oled_WriteData(unsigned char dat);
extern void oled_WriteData_inversion(unsigned char dat);
extern void oled_WriteASSIC(uchar X, uchar Y, char *s, char inversion);

extern void oled_DisplayUnsignedNum(uchar x, uchar y, char close, int num, int length, char inversion);
extern void oled_DisplaySignedNum(uchar x, uchar y, char close, int num, int length, char inversion);

extern int GetLength(int num);
//5.Redefine public variable
#define oled_closeLeft	0
#define	oled_closeRight	1

#define oled_uninversion 	0
#define oled_inversion 		1
//6.Define private parameter

#endif

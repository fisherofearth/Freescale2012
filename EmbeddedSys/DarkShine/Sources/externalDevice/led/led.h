#ifndef _LED_H_
#define _LED_H_

//1.Include head files
#include "derivative.h" /* include peripheral declarations */
    
//2.Define types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces
#define led_I_LED 		GPIOA_PDOR 
#define led_I_tailLED 	GPIOD_PDOR 

#define led_Pn_1     ((0x00000001)<<(17))      
#define led_Pn_2     ((0x00000001)<<(16))       
#define led_Pn_3     ((0x00000001)<<(15))         
#define led_Pn_4     ((0x00000001)<<(14))       

#define led_Pn_tailLED     ((0x00000001)<<(14)) 
//4.Redefine public functions
extern void led_SwitchLEachLED(uchar which, uchar state);
extern void ShineTailLed();

#define OnTailLED()		led_I_tailLED |=  led_Pn_tailLED
#define OffTailLED()	led_I_tailLED &= ~led_Pn_tailLED
#define TailLED(x) ((x)>0? (OffTailLED()):(OnTailLED()))

//5.Redefine public variable


//6.Define private parameter
#define led_on       	0         //灯亮(对应低电平)
#define led_off      	1         //灯暗(对应高电平)


#endif 

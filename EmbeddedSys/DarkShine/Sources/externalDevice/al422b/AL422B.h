#ifndef _AL422B_H_
#define _AL422B_H_


//1.Include the head files
#include "derivative.h" /* include peripheral declarations */


//2.Define the types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces
#define AL422B_Control_Interface	GPIOC_PDOR

#define AL422B_WE1_MASK			(0x00000001 << 2)
#define AL422B_WE2_MASK			(0x00000001 << 3)
#define AL422B_RE1_MASK			(0x00000001 << 13)
#define AL422B_RE2_MASK			(0x00000001 << 15)
#define AL422B_RRST_MASK		(0x00000001 << 17)
#define AL422B_RCK_MASK  		(0x00080000)//(0x00000001 << 19)
#define AL422B_RCKz_MASK  		(0xFFF7FFFF)

#define AL422B_SCL_MASK			(0x00000001 << 17)
#define AL422B_SDA_MASK			(0x00000001 << 18)


#define AL422B_VSYNC_status		((GPIOB_PDIR >> 19) & (1))
#define AL422B_Data_Interface	((uchar)(GPIOB_PDIR & 0xFF))


/*---------FIFO_发送读时钟---------*/
/*#define al422b_SendReadClock() \
	AL422B_Control_Interface |= AL422B_RCK_MASK;\
	AL422B_Control_Interface &= ~AL422B_RCK_MASK*/
/*---------FIFO_发送读时钟---------*/
//4.Redefine public functions
extern void al422b_SendReadClock() ;
extern uchar al422b_ReadData(void);
extern void al422b_Reset(void);
extern void al422b_Init(void);
extern void al422b_SelectFIFO(int device);
//#define al422b_SendReadClock()	\
//	AL422B_Control_Interface |= AL422B_RCK_MASK;\
//	AL422B_Control_Interface &= AL422B_RCKz_MASK
	
//5.Redefine public variable
//uchar al422b_Pr_selectFIFO;

//6.Define private parameter


// parameters interface ------>>>
#define T_RPL 0
#define T_RPH 0

#define T_RRS 5
#define T_RRH 2

#define T_RES 5
#define T_REH 2

#define T_WES 5
#define T_WEH 2


// <<<------

#endif

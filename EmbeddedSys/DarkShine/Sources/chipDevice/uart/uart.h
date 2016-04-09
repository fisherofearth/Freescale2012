#ifndef _UART_H_
#define _UART_H_

//1.Include the head files
#include "derivative.h" /* include peripheral declarations */
#include "MCUinit.h"

//2.Define the types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int


//3.Define interfaces
//串行通信寄存器及标志位定义
#define ReSendStatusR(base)   ((base)->S1) 	//状态寄存器
#define ReSendDataR(base)     ((base)->D)  	//数据寄存器
#define SendTestBit           7        		//发送缓冲区空标志位
//#define UART0 0
//#define UART1 1
#define UART2 2
//#define UART3 3
//#define UART4 4
//#define UART5 5

//4.Redefine the functions
extern void uart2_sendChar(uchar data);
extern void uart_SendChannel2 (uchar data);
extern void uart_ReceiveData();
extern int uart_RestoreCMD();
extern int uart_RestoreData();

extern uchar D_uartDataReceive;
extern uchar F_uartDataReceive;
#endif

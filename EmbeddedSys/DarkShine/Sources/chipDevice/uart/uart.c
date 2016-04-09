#include "uart.h"

uchar D_uartDataReceive = 0;
uchar F_uartDataReceive = 0;

void uart2_sendChar(uchar data)
{
	unsigned long k = 0;
        
	for (k = 0; k < 0xFFFFFFFF; k++)       //有时间限制
	{
		if((UART2_S1 & (0x40)) != 0)
		{
			ReSendDataR(UART2_BASE_PTR) = data;
		
			break;
		}
	  
	}
 }


void uart_SendChannel2 (uchar data)
{
    //等待发送缓冲区空
    while(!(UART_S1_REG(UART2_BASE_PTR) & UART_S1_TDRE_MASK));
    //发送数据
    ReSendDataR(UART2_BASE_PTR) = data;
}

char buffer_ReceiveData[4];
uchar point_ReceiveData=0;
uchar F_startReceive;
void uart_ReceiveData()
{
	
	//D_uartDataReceive = ;
	
	
	
	if(D_uartDataReceive > 191)
	{
		F_startReceive = 1;
		point_ReceiveData = 0;
	}
	if(F_startReceive == 1)
	{
		buffer_ReceiveData[point_ReceiveData] = UART2_D;
		if(point_ReceiveData < 3)point_ReceiveData ++;//point_ReceiveData 最大到3
		else 
		{
			F_startReceive =0;
			F_uartDataReceive = 1;
		}
	}
}

int uart_RestoreCMD()
{
	return (buffer_ReceiveData[0] & 0x3f);
	
}
int uart_RestoreData()
{
	int tmp;
	tmp = ((int)(buffer_ReceiveData[1] & 0x0f)) << 12;
	tmp += ((int)(buffer_ReceiveData[2] & 0x3f)) << 6 ;
	tmp += (int)(buffer_ReceiveData[3] & 0x3f);
	return tmp;
}

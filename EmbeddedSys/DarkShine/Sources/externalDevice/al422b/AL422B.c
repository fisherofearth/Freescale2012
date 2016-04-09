#include "al422b.h"
void AL422B_delay(uint t)
{
	while(t--);
}
/*---------FIFO_发送读时钟---------*/
void al422b_SendReadClock() 
{
	
	AL422B_Control_Interface |= AL422B_RCK_MASK;
	AL422B_Control_Interface &= AL422B_RCKz_MASK;
}
/*---------FIFO_发送读时钟---------*/

/*---------读取数据----------------*/
uchar al422b_ReadData(void) { //读取数据

	al422b_SendReadClock();
	return AL422B_Data_Interface;
}
/*---------读取数据----------------*/

/*---------FIFO_读时针重置---------*/
void al422b_Reset(void) { //FIFO读指针复位
	AL422B_Control_Interface &= ~AL422B_RRST_MASK;
	AL422B_delay(T_RRS);
	al422b_SendReadClock();
	al422b_SendReadClock();
	AL422B_delay(T_RRH);
	AL422B_Control_Interface |= AL422B_RRST_MASK;

}
/*---------FIFO_读时针重置---------*/

/*---------FIFO_读初始化 ----------*/
void al422b_Init(void) {
	AL422B_delay(100);
	al422b_SelectFIFO(0);
}
/*---------FIFO_读初始化 ----------*/

void al422b_SelectRead(int device) {
	switch (device) {
	case 0:
		AL422B_delay(T_REH);
		AL422B_Control_Interface |= AL422B_RE2_MASK;
		AL422B_Control_Interface &= ~AL422B_RE1_MASK;
		AL422B_delay(T_RES);
		break;
	case 1:
		AL422B_delay(T_WEH);
		AL422B_Control_Interface &= ~AL422B_RE2_MASK;
		AL422B_Control_Interface |= AL422B_RE1_MASK;
		AL422B_delay(T_WES);
		
		break;
	}
}

void al422b_SelectWrite(int device) {
	switch (device) {
	case 0:
		AL422B_delay(T_REH);
		AL422B_Control_Interface |= AL422B_WE2_MASK;
		AL422B_Control_Interface &= ~AL422B_WE1_MASK;
		AL422B_delay(T_RES);
		break;
	case 1:
		AL422B_delay(T_WEH);
		AL422B_Control_Interface &= ~AL422B_WE2_MASK;
		AL422B_Control_Interface |= AL422B_WE1_MASK;
		AL422B_delay(T_WES);
		break;
	}
}

/*---------切换两片FIFO------------*/
void al422b_SelectFIFO(int device) {
	if(device == 0)
	{
		al422b_SelectRead(0);
		al422b_SelectWrite(0);
	}
	else
	{
		al422b_SelectRead(1);
		al422b_SelectWrite(1);
	}
	
		
}
/*---------切换两片FIFO------------*/


#include "oled.h" 
#include "assic_6x8.h"

char *space[] = {
"",
" ",
"  ",
"   ",
"    ",
"     "
};

void oled_delay(unsigned int t)
{
	while(t--);
}


#define   oled_SwitchSID(x) ((x)==0?(oled_PORT_SDIN &= ~oled_BIT_SDIN):(oled_PORT_SDIN |= oled_BIT_SDIN))


void oled_SendClock()
{
	oled_PORT_SCLK &= ~oled_BIT_SCLK;
	oled_PORT_SCLK |= oled_BIT_SCLK;
}
void oled_WriteCMD(unsigned char dat){
	
	oled_PORT_CS &= ~oled_BIT_CS;
	oled_delay (10);
	oled_PORT_DC &= ~oled_BIT_DC;	//INSTRUCTION
	oled_delay (10);
	
	oled_SwitchSID(((dat & 0x80) == 0x80)); oled_SendClock(); // send MSB
	oled_SwitchSID(((dat & 0x40) == 0x40)); oled_SendClock();
	oled_SwitchSID(((dat & 0x20) == 0x20)); oled_SendClock();
	oled_SwitchSID(((dat & 0x10) == 0x10)); oled_SendClock();
	oled_SwitchSID(((dat & 0x08) == 0x08)); oled_SendClock();
	oled_SwitchSID(((dat & 0x04) == 0x04)); oled_SendClock();
	oled_SwitchSID(((dat & 0x02) == 0x02)); oled_SendClock();
	oled_SwitchSID(((dat & 0x01) == 0x01)); oled_SendClock(); // sedn LSB

	oled_PORT_CS |= oled_BIT_CS;
}

void oled_WriteData(unsigned char dat){
	oled_PORT_CS &= ~oled_BIT_CS;
	oled_PORT_DC |= oled_BIT_DC;	//DISPLAY DATA

  	oled_SwitchSID(((dat & 0x80) == 0x80)); oled_SendClock(); // send MSB
  	oled_SwitchSID(((dat & 0x40) == 0x40)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x20) == 0x20)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x10) == 0x10)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x08) == 0x08)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x04) == 0x04)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x02) == 0x02)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x01) == 0x01)); oled_SendClock(); // sedn LSB
  	oled_PORT_CS |= oled_BIT_CS;
}

void oled_WriteData_inversion(unsigned char dat){
	oled_PORT_CS &= ~oled_BIT_CS;
	oled_PORT_DC |= oled_BIT_DC;	//DISPLAY DATA

	dat = ~dat;
  	oled_SwitchSID(((dat & 0x80) == 0x80)); oled_SendClock(); // send MSB
  	oled_SwitchSID(((dat & 0x40) == 0x40)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x20) == 0x20)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x10) == 0x10)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x08) == 0x08)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x04) == 0x04)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x02) == 0x02)); oled_SendClock();
  	oled_SwitchSID(((dat & 0x01) == 0x01)); oled_SendClock(); // sedn LSB
  	
  	oled_PORT_CS |= oled_BIT_CS;
}

void oled_SetXY(unsigned char X, unsigned char Y){
    oled_WriteCMD(0xB0 | Y);	
    oled_WriteCMD(0x10 | (X>>4));    
	oled_WriteCMD(0x0F & X);  
}

void oled_ClearScreen(void){
    unsigned char x,y;
   
	for(y=0;y<12;y++)
	{ 
		oled_SetXY(0,y);
		for(x=0;x<128;x++)
		{
			oled_WriteData(0x00);
			oled_WriteData(0x00);
		}
	}		      	
}


void oled_InitDevice(void)
{	
	oled_PORT_RST &= ~oled_BIT_RST;
	oled_delay(1000);
	oled_PORT_RST |= oled_BIT_RST;
	
	oled_WriteCMD(0xAE);	/* �ر���ʾ */
	oled_WriteCMD(0xD5);	/* ����ʱ�ӷ�Ƶ����,��Ƶ�� */
	oled_WriteCMD(0x80); /* [3:0],��Ƶ����;[7:4],��Ƶ�� */
	oled_WriteCMD(0xA8);	/* ��������·�� */
	oled_WriteCMD(0x3F);	/* Ĭ��0x3F(1/64) */
	oled_WriteCMD(0xD3);	/* ������ʾƫ�� */
	oled_WriteCMD(0x00);	/* Ĭ��Ϊ0 */

	oled_WriteCMD(0x40);	/* ������ʾ��ʼ��[5:0],���� */

	oled_WriteCMD(0x8D);	/* ��ɱ����� */
	oled_WriteCMD(0x14);	/* bit2,����/�ر� */
	oled_WriteCMD(0x20);	/* �����ڴ��ַģʽ */
	oled_WriteCMD(0x02);	/* [1:0],00,�е�ַģʽ;01,�е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10; */
	oled_WriteCMD(0xA1);	/* ���ض�������,bit0:0,0->0;1,0->127; */
	oled_WriteCMD(0xC0);	/* ����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� */
	oled_WriteCMD(0xDA);	/* ����COMӲ���������� */
	oled_WriteCMD(0x12);	/* [5:4]���� */

	oled_WriteCMD(0x81);	/* �Աȶ����� */
	oled_WriteCMD(0xEF);	/* 1~255;Ĭ��0x7F (��������,Խ��Խ��) */
	oled_WriteCMD(0xD9);	/* ����Ԥ������� */
	oled_WriteCMD(0xF1);	/* [3:0],PHASE 1;[7:4],PHASE 2; */
	oled_WriteCMD(0xDB);	/* ����VCOMH ��ѹ���� */
	oled_WriteCMD(0x30);	/* [6:4] 000,0.65*VCC;001,0.77*VCC;011,0.83*VCC; */

	oled_WriteCMD(0xA4);	/* ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����) */
	oled_WriteCMD(0xA6);	/* ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ */
	
	oled_WriteCMD(0xAF);	/* ������ʾ */
	oled_ClearScreen();  
}

void oled_WriteASSIC(uchar X, uchar Y, char *s, char inversion) {
	unsigned char i;
	unsigned char j = 0;
	
	while (*s != 0) {
		oled_SetXY((X + (j * 6)), Y);
		for (i = 0; i < 6; i++) {
			if(inversion == 0)
				oled_WriteData(assic_6x8[(*s - 32)][i]);
			else
				oled_WriteData_inversion(assic_6x8[(*s - 32)][i]);
		}
		s++;
		j++;
	}
}

int GetLength(int num) {
	int i;
	for (i = 0; i < 5; i++) {
		num /= 10;
		if (num == 0)
			return (i + 1);
	}
}

void oled_DisplayUnsignedNum(uchar x, uchar y, char close, int num, int length, char inversion) {
	int i, j;
	uchar len;
	int tmp, tmp2;

	len = GetLength(num);
	
	
	tmp = length - len;
	if(close == oled_closeLeft)
	{
		oled_WriteASSIC((x + (6 * len)), y ,space[tmp],oled_uninversion);
	}
	else
	{
		oled_WriteASSIC((x - (6 * length)), y ,space[tmp],oled_uninversion);
	}
	
	//set start place
	oled_SetXY(x - (close * (6 * len)), y);

	tmp2 = 10000;
	for (i = 0; i < (5 - len); i++)
		tmp2 /= 10;

	for (i = 0; i < len; i++) {

		tmp = (num / tmp2) % 10;
		for (j = 0; j < 6; j++) {
			if (inversion == 0)
				oled_WriteData(assic_6x8[tmp + 16][j]);
			else
				oled_WriteData_inversion(assic_6x8[tmp + 16][j]);
		}
		tmp2 /= 10;
	}
}


void oled_DisplaySignedNum(uchar x, uchar y, char close, int num, int length, char inversion) {
	
	if(num < 0){
		oled_WriteASSIC(x - (close * (6 * GetLength(num))) - (close * 6),y,"-",inversion);
		num *= (-1);
		
	}
	else
	{
		oled_WriteASSIC(x - (close * (6 * GetLength(num))) ,y," ",inversion);
		
	}
	oled_DisplayUnsignedNum(x+((1-close) * 6),y,close,num,length,inversion);
}










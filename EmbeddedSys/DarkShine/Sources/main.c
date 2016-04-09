#define PE_MCUINIT

#include "main.h"

void MCU_init(void); /* Device initialization function declaration */
void delay(unsigned long t);
void InitialSystem();


 void main(void) {
	int fun;
	uchar tmp;
	InitialSystem();
	
	ReadPublicParametersFromSD();

	MMA7660_InitialDevice();
	
	while (1) {
		TailLED(led_on);	//打开尾灯
		motor_DisableHbridge();	//关闭电机驱动
		motor_StopMotor();	//电机动力 = 0
		PowerSwitch_SevroDirection(power_off);	//关闭舵机电源
		uart2_sendChar(0xff);
		fun = RunMenu();	//菜单
		
		/*for(;;)
		{
			oled_WriteASSIC(0,0,"   ",oled_uninversion);
			oled_WriteASSIC(0,1,"   ",oled_uninversion);
			
			
			tmp = ADC1_RA;
			oled_DisplayUnsignedNum(0,1,oled_closeLeft,(tmp),oled_uninversion);	
			//uart2_sendChar(tmp);
			//oled_DisplayUnsignedNum(0,0,oled_closeLeft,(MMA7660_X()&(0x0f)),oled_uninversion);
			delay(0xFF);
		}*/
		
		
		
		RunFunctions(fun);	
	}
}





void delay(unsigned long t) {
	unsigned int tmp;
	while (t--) {
		for (tmp = 0; tmp < 1000; tmp++)
			asm ("nop");
	}
}

void InitialSystem() {
	uchar tmp = 0;
	uchar C_tmp = 0;
	
	MCU_init(); /* call device initialization */
	OffTailLED();
	MCU_DisablePITInterrupt0();
	MCU_DisablePITInterrupt1();//关闭速度控制中断
	MCU_DisablePITInterrupt2();
	MCU_DisablePITInterrupt3();
	
	oled_InitDevice();
	
	oled_WriteASSIC(31,4,"DarkShine",oled_uninversion);
	oled_WriteASSIC(34,0,"Loading...",oled_uninversion);
	
	do {
		switch (C_tmp) {
		case 0:
			break;
		case 1:
		case 2:
			oled_WriteASSIC(34,0,"Rebooting ",oled_uninversion);
			delay(0x3000);
			oled_WriteASSIC(34,0,"Loading...",oled_uninversion);
			break;
		default:
			PowerSwitch_SevroDirection(power_off);
			oled_WriteASSIC(34,0," ERROR!!! ",oled_uninversion);
			PowerSwitch_SevroDirection(power_off); //disable sevro
			for(;;);
			break;
		}
		C_tmp++;
		motor_DisableHbridge();	//disable motor
		motor_StopMotor();	
		sevro_TurnWheel(0);	//sevro reset
		PowerSwitch_SevroDirection(power_on);	//enable sevro
		ov7620_Init();
		al422b_Init();
		
		delay(3000);
		tmp = microSD_InitDevice();
		if(tmp != 0)
		{
			delay(3000);
			tmp = microSD_InitDevice();
		}
		//tmp =0 ;//跳过MicroSD检查
	} while (tmp != 0);
	PowerSwitch_SevroDirection(power_off); //disable sevro
	oled_ClearScreen();
	OnTailLED();
	
	MCU_enableIQR();
}
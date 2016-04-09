#include "mainFunction.h"

int C_startLines = 0;
int T_startLine = 0;
int T_afterStartLine = 0;
int F_afterStartLine = 0;

struct CHARACTER last_crctr_BTX[2];

void InitialFunctions(int fun);
void RunFunctions(int fun);
int Race();

void InitialFunctions(int fun){
	switch (fun) {
		case 0:
			sevro_TurnWheel(0);
			delay(0x3666);
			PowerSwitch_SevroDirection(power_on);//前轮舵机电源
			MCU_EnablePITInterrupt1();//使能速度控制中断
			motor_EnableHbridge();//使能H桥驱动
			//MCU_EnablePITInterrupt2();//使能积分控制中断in

			C_startLines = 0;
			T_startLine = 0;
			T_afterStartLine = 0;
			F_afterStartLine = 0;
			break;
		case 3:
			break;
		case 4:
			oled_WriteASSIC(12, 7, "X_R", oled_inversion);
			oled_WriteASSIC(36, 7, "Z_R", oled_inversion);
			oled_WriteASSIC(60, 7, "X_L", oled_inversion);
			oled_WriteASSIC(84, 7, "Z_L", oled_inversion);
							
			oled_WriteASSIC(0, 6, "Xt", oled_inversion);
			oled_WriteASSIC(0, 5, "Xm", oled_inversion);
			oled_WriteASSIC(0, 4, "Xh", oled_inversion);
			break;
		case 5:
			sevro_TurnWheel(0);
			PowerSwitch_SevroDirection(power_on);//前轮舵机电源
			break;
		case 6:
			oled_WriteASSIC(0, 7, "R", oled_inversion);
			oled_WriteASSIC(0, 6, "Kht", oled_inversion);
			oled_WriteASSIC(0, 5, "Kit", oled_inversion);
			oled_WriteASSIC(0, 4, "Khi", oled_inversion);
							
			oled_WriteASSIC(0, 3, "Pt", oled_inversion);
			oled_WriteASSIC(0, 2, "Pi", oled_inversion);
			oled_WriteASSIC(0, 1, "Pm", oled_inversion);
			oled_WriteASSIC(0, 0, "Ph", oled_inversion);
			break;
		default:
			break;
		}
}

void subfunction_DisplayVideo()
{
	if (ov7620_F_VSYNC == 1) {
		
		DisplayVideo();
		ShineTailLed();
		
		ov7620_F_VSYNC = 0;
	}
}

void subfunction_Debug1()
{
	if (ov7620_F_VSYNC == 1) {
		SearchEdge();
		DisplayEdge();
		ShineTailLed();
						
		oled_DisplayUnsignedNum(30,6,oled_closeRight,chrctr_BTX[left].tail.x,3,oled_uninversion);
		oled_DisplayUnsignedNum(54,6,oled_closeRight,chrctr_BTX[left].tail.z,2,oled_uninversion);
		oled_DisplayUnsignedNum(78,6,oled_closeRight,chrctr_BTX[right].tail.x,3,oled_uninversion);
		oled_DisplayUnsignedNum(102,6,oled_closeRight,chrctr_BTX[right].tail.z,2,oled_uninversion);
		
		oled_DisplayUnsignedNum(30,5,oled_closeRight,chrctr_BTX[left].mid.x,3,oled_uninversion);
		oled_DisplayUnsignedNum(54,5,oled_closeRight,chrctr_BTX[left].mid.z,3,oled_uninversion);
		oled_DisplayUnsignedNum(78,5,oled_closeRight,chrctr_BTX[right].mid.x,3,oled_uninversion);
		oled_DisplayUnsignedNum(102,5,oled_closeRight,chrctr_BTX[right].mid.z,3,oled_uninversion);
		
		oled_DisplayUnsignedNum(30,4,oled_closeRight,chrctr_BTX[left].head.x,3,oled_uninversion);
		oled_DisplayUnsignedNum(54,4,oled_closeRight,chrctr_BTX[left].head.z,3,oled_uninversion);
		oled_DisplayUnsignedNum(78,4,oled_closeRight,chrctr_BTX[right].head.x,3,oled_uninversion);					
		oled_DisplayUnsignedNum(102,4,oled_closeRight,chrctr_BTX[right].head.z,3,oled_uninversion);					
				
		
		ov7620_F_VSYNC = 0;
	}
}

void subfunction_Debug2()
{
	if (ov7620_F_VSYNC == 1) {
		Race();
		
		
		ov7620_F_VSYNC = 0;
	}
}

void subfunction_Debug3()
{
	if (ov7620_F_VSYNC == 1) {
			SearchEdge();
			//DisplayEdge();
			ShineTailLed();
							
			oled_DisplaySignedNum(42,7,oled_closeRight,chrctr_BTX[left].R_hit+chrctr_BTX[right].R_hit,3,oled_uninversion);
			oled_DisplaySignedNum(42,6,oled_closeRight,chrctr_BTX[left].k_ht +chrctr_BTX[right].k_ht,3,oled_uninversion);
			oled_DisplaySignedNum(42,5,oled_closeRight,chrctr_BTX[left].k_it +chrctr_BTX[right].k_it,3,oled_uninversion);			
			oled_DisplaySignedNum(42,4,oled_closeRight,chrctr_BTX[left].k_hi +chrctr_BTX[right].k_hi,3,oled_uninversion);

			oled_DisplayUnsignedNum(42,3,oled_closeRight,chrctr_BTX[left].tail.x+chrctr_BTX[right].tail.x,3,oled_uninversion);
			oled_DisplayUnsignedNum(70,3,oled_closeRight,chrctr_BTX[left].tail.z+chrctr_BTX[right].tail.z,2,oled_uninversion);
						
			oled_DisplayUnsignedNum(42,2,oled_closeRight,chrctr_BTX[left].inflection.x+chrctr_BTX[right].inflection.x,3,oled_uninversion);
			oled_DisplayUnsignedNum(70,2,oled_closeRight,chrctr_BTX[left].inflection.z+chrctr_BTX[right].inflection.z,2,oled_uninversion);
						
			oled_DisplayUnsignedNum(42,1,oled_closeRight,chrctr_BTX[left].mid.x+chrctr_BTX[right].mid.x,3,oled_uninversion);
			oled_DisplayUnsignedNum(70,1,oled_closeRight,chrctr_BTX[left].mid.z+chrctr_BTX[right].mid.z,2,oled_uninversion);
						
			oled_DisplayUnsignedNum(42,0,oled_closeRight,chrctr_BTX[left].head.x+chrctr_BTX[right].head.x,3,oled_uninversion);
			oled_DisplayUnsignedNum(70,0,oled_closeRight,chrctr_BTX[left].head.z+chrctr_BTX[right].head.z,2,oled_uninversion);
						
			
			
			
			ov7620_F_VSYNC = 0;
		}
}

uchar accelerationSmoothing[5];
uchar pointer_accelerationSmoothing = 0;

int wheelOutputSmoothing[5];
int pointer_wheelOutputSmoothing = 0;


void RunFunctions(int fun) {
	uchar tmp;
	
	InitialFunctions(fun);
	
	switch (fun) {
	case 0://竞赛
		for (;;) {
			if (keyboard_I_input == 13)
				break;//按下了key_close
			Race();
			
			
		}
		break;
	case 3://video
		for (;;) {
			if (keyboard_I_input == 13)
				break;//按下了key_close
			subfunction_DisplayVideo();
		}
		break;
	case 4://debug 1
		for (;;) {
			if (keyboard_I_input == 13)
				break;//按下了key_close
			subfunction_Debug1();
		}
		break;
	case 5://debug 2
		for (;;) {
			if (keyboard_I_input == 13)
				break;//按下了key_close
			subfunction_Debug2();
		}
		break;
	case 6://debug 3
		for (;;) {
			if (keyboard_I_input == 13)
				break;//按下了key_close
			subfunction_Debug3();
		}
		break;
	}
}


#define thresholdLostEdge 2
int Race() {
	uchar i;
	uchar tmpChar = 0;
	int setSpeed = 200;
	int T_slowDown=0;
	
	int tmp = 0;
	int tmp1, tmp2;
	int LorR;
	int tailPX;
	int midPX;
	int headPX;
	int tailPZ;
	int midPZ;
	int headPZ;
	
	int averageWheelOutput;
	int averageAcceleration;
	
	int theoryAcceleration;

	int setMaxPower = 0;
	struct CHARACTER N_chrctr[2];

	//终项
	int E_PointHead = 0, E_PointMid = 0, E_PointTail = 0, E_K_hi = 0,
			E_K_it = 0, E_K_ht = 0, E_K_R = 0;

	int wheelOutput;

	
	if (ov7620_F_VSYNC == 1) {			
		
	
		SearchEdge();
		//ShineTailLed();

		if (C_activePoint[right] > thresholdLostEdge)//右线捕获
			N_chrctr[right] = chrctr_BTX[right];
		else
			//右线丢失
			N_chrctr[right] = last_crctr_BTX[right];

		if (C_activePoint[left] > thresholdLostEdge)//左线捕获
			N_chrctr[left] = chrctr_BTX[left];
		else
			//左线丢失
			N_chrctr[left] = last_crctr_BTX[left];

		tailPX = N_chrctr[left].tail.x + N_chrctr[right].tail.x - middleX;
		midPX = N_chrctr[left].mid.x + N_chrctr[right].mid.x	- middleX;
		headPX = N_chrctr[left].head.x + N_chrctr[right].head.x - middleX;

		tmp1 = N_chrctr[left].tail.z + N_chrctr[right].tail.z;
		tmp2 = N_chrctr[left].head.z + N_chrctr[right].head.z;
		tailPZ = (ABS(tmp1 - tmp2)) / 5;

		//E_PointHead = headPX * P_PointH / 10;
		E_PointHead = headPX * P_PointH / 10;//????????
				
		//E_PointHead = headPX * P_PointH / (((C_activePoint[left] + C_activePoint[right])/10) + 5);
		
		E_PointMid = midPX * P_PointI / 10;
		//E_PointTail = ((tailPX * P_PointT) / (tailPZ + 4));
		//E_PointTail = ((tailPX * P_PointT) / ((tailPZ/2) + 10));
		E_PointTail = ((tailPX * P_PointT) / (10));
		E_K_ht = ((N_chrctr[left].k_ht + N_chrctr[right].k_ht) * P_K_ht) / 5;
		E_K_hi = ((N_chrctr[left].k_hi + N_chrctr[right].k_hi) * P_K_hi) / 5;
		E_K_R = (N_chrctr[left].R_hit + N_chrctr[right].R_hit) * P_R / 5;

		wheelOutput = 0;
		wheelOutput += E_PointHead;
		wheelOutput += E_PointMid;
		wheelOutput += E_PointTail;
		wheelOutput += E_K_ht;
		wheelOutput += E_K_hi;
		wheelOutput += E_K_it;
		wheelOutput += E_K_R;

		if(wheelOutput > maxWheelOutput)wheelOutput = maxWheelOutput;
		if(wheelOutput < minWheelOutput)wheelOutput = minWheelOutput;
		sevro_TurnWheel(wheelOutput);

		//tmp = ABS(N_chrctr[left].k_hi + N_chrctr[right].k_hi);
		//setSpeed = D_baseSpeed - (tmp * 3);
		
		//tmp = ((D_baseSpeed * minCutveSpeedRate) / 10);
		//if (setSpeed < tmp)
		//	setSpeed = tmp;

		//继承特征量
		//tmp = N_chrctr[left].inflection.x + N_chrctr[right].inflection.x - 160;
		//tmp /= 16;
		//if(N_chrctr[left].k_hi<100)N_chrctr[left].k_hi += tmp;
		//if(N_chrctr[right].k_hi<100)N_chrctr[right].k_hi += tmp;
		
		last_crctr_BTX[left] = N_chrctr[left];
		last_crctr_BTX[right] = N_chrctr[right];

		
		//start line
		T_startLine++;
		if (T_startLine == 200) {
			C_startLines = 1;
		}
		if (F_startLine == 1) {
			if ((ABS(N_chrctr[left].k_ht + N_chrctr[right].k_ht)) < 15) {
				if ((C_activePoint[left] + C_activePoint[right]) > 30) {
					if (C_startLines == 1) {
						F_startLine = 0;
						F_afterStartLine = 1;
						F_integral = 0;
						//MCU_DisablePITInterrupt2();
						TailLED(led_on);
						sevro_TurnWheel(0);
						delay(0x3FF);
						motor_SetPower((-500) - (D_speedNow*3));
						delay(D_speedNow * 30);
						motor_SetPower(-100);
						//
					}
				}
			}
		}
		if (F_afterStartLine == 1) {
			T_afterStartLine++;
			if (T_afterStartLine > 10) {
				MCU_DisablePITInterrupt2();
				
				motor_SetPower(-100);
				
				
				if (T_afterStartLine > 30) {
					motor_SetPower(0);
					motor_DisableHbridge();
				}
			}
			motor_SetPower(0);
			return 100;
		}
			
			
	/*
		setMaxPower = ABS(wheelOutput);
		if (setMaxPower > 890)
			setMaxPower = 890;
		setMaxPower /= 89;
		setMaxPower = (setMaxPower * P_MPC) / 10;
		setMaxPower = 20 - setMaxPower;
		setMaxPower = (D_maxPower * setMaxPower) / 20;
	
			
		//入弯减速
		if ((ABS(N_crctr[left].k_hi + N_crctr[right].k_hi)) < 16) {
			if ((ABS(N_crctr[left].k_it + N_crctr[right].k_it)) > 39) {
				if (D_speedNow > (setSpeed)) {
					setSpeed = D_baseSpeed / 5;
					D_speedOutput = D_maxPower / 2;
					motor_SetPower(-300);
					goto nextC;
				} else {
					if (D_speedNow > (setSpeed / 2)) {
						setSpeed = D_baseSpeed / 2;
						setMaxPower = (D_maxPower * 2) / 3;
						D_speedOutput = 0;
						motor_SetPower(0);
					}
				}
			}
		}*/
		//nextC:
		
		
		
		
		
		//加速度滤波
		pointer_accelerationSmoothing++;
		if(pointer_accelerationSmoothing>4)pointer_accelerationSmoothing=0;
		accelerationSmoothing[pointer_accelerationSmoothing] =ADC0_RA;
		averageAcceleration = (int)accelerationSmoothing[0];
		averageAcceleration += (int)accelerationSmoothing[1];
		averageAcceleration += (int)accelerationSmoothing[2];
		averageAcceleration += (int)accelerationSmoothing[3];
		averageAcceleration += (int)accelerationSmoothing[4];
		averageAcceleration /= 5;
		
		//舵机转角滤波
		pointer_wheelOutputSmoothing++;
		if(pointer_wheelOutputSmoothing>4)pointer_wheelOutputSmoothing=0;
		wheelOutputSmoothing[pointer_wheelOutputSmoothing] =wheelOutput;
		averageWheelOutput = (int)wheelOutputSmoothing[0];
		averageWheelOutput += (int)wheelOutputSmoothing[1];
		averageWheelOutput += (int)wheelOutputSmoothing[2];
		averageWheelOutput += (int)wheelOutputSmoothing[3];
		averageWheelOutput += (int)wheelOutputSmoothing[4];
		averageWheelOutput /= 5;
		
		//计算理论
		theoryAcceleration = D_speedNow/10;
		theoryAcceleration *= D_speedNow;
		theoryAcceleration /= 100;
		theoryAcceleration *= averageWheelOutput/7;
		theoryAcceleration /= 15;
		

		
		//入弯制动
		if ((C_activePoint[left] + C_activePoint[right]) > 30) {
			if ((C_activePoint[left] + C_activePoint[right]) < 50) {
				if(D_speedNow > speedEnterCurve){
					if((ABS(N_chrctr[left].k_ht + N_chrctr[right].k_ht))<40)//类似直道
					{
						if((ABS(N_chrctr[left].k_it + N_chrctr[right].k_it)) > 35)
						{
							//要进急弯了，减速吧！
							T_slowDown = brakingTime + (D_speedNow - speedEnterCurve) ;
							tmp = brakingPower ;
							tmp += ((D_speedNow - speedEnterCurve)*5);
							tmp += ABS(middleX - (N_chrctr[left].tail.x + N_chrctr[right].tail.x));
							//tmp += ABS(middleX - (N_chrctr[left].head.x + N_chrctr[right].head.x));
							tmp += ABS(middleX - (N_chrctr[left].mid.x + N_chrctr[right].mid.x));
							if(tmp>1000)tmp =1000;
							motor_SetPower(-(tmp));
							D_speedOutput = slowdownPower;
							TailLED(led_on);
						}
					}
				}
				
			}
		}
		if(T_slowDown == 0)
		{
			if((ABS(theoryAcceleration - (averageAcceleration - 145))) < thresholdSlide)//没打滑
			{
				
				//oled_DisplaySignedNum(42,6,oled_closeRight,chrctr_BTX[left].k_ht +chrctr_BTX[right].k_ht,3,oled_uninversion);
				//oled_DisplaySignedNum(42,5,oled_closeRight,chrctr_BTX[left].k_it +chrctr_BTX[right].k_it,3,oled_uninversion);			
		
				//oled_DisplaySignedNum(128,0,oled_closeRight,(C_activePoint[left] + C_activePoint[right]),3,oled_uninversion);				
				
				//oled_DisplaySignedNum(128,1,oled_closeRight,D_speedNow,3,oled_uninversion);				
										
				//E_K_ht = ((N_chrctr[left].k_ht + N_chrctr[right].k_ht) * P_K_ht) / 5;
				//E_K_hi = ((N_chrctr[left].k_hi + N_chrctr[right].k_hi) * P_K_hi) / 5;
				//E_K_R = (N_chrctr[left].R_hit + N_chrctr[right].R_hit) * P_R / 5;
				
				
				//弯道速度调节
				setSpeed = D_baseSpeed ;
				//setSpeed = (ABS(tailPX))/15 + (ABS(midPX))/10;
				//setSpeed += (ABS(N_chrctr[left].k_ht + N_chrctr[right].k_ht))/10;
				//setSpeed += (ABS(N_chrctr[left].k_it + N_chrctr[right].k_it))/5;
							
				setSpeed -= ((58 -(C_activePoint[left] + C_activePoint[right]))/2);
				
				//速度控制
				
				TailLED(led_off);
				ControlSpeed_motor(setSpeed, D_maxPower, D_minPower); 
				
			}
			else{//转向不足
				
				//ControlSpeed_motor(D_baseSpeed/2, 500/tmp, 0); //速度控制
				if(D_speedNow > speedEnterCurve){
					motor_SetPower(-slidePower);
					D_speedOutput = slowdownPower;
				}
			}
		}
		
		
		
		//oled_DisplaySignedNum(0,0,oled_closeLeft,(averageAcceleration - 145),4,oled_uninversion);
		//oled_DisplaySignedNum(0,1,oled_closeLeft,(theoryAcceleration),4,oled_uninversion);
		
		
		
		
		
		
		tmpChar = (unsigned char)averageAcceleration;
		if(tmpChar > 0xFD)tmpChar = 0xFD;		
		
		uart2_sendChar(0xff);
		uart2_sendChar(tmpChar);
		
		tmp = D_speedNow;
		//tmp = theoryAcceleration+128;
		if(tmp > 0xFD)tmp = 0xFD;
		uart2_sendChar((char)tmp);
		
		
		tmp = averageWheelOutput / 7;
		tmp += 128;
		uart2_sendChar((char)tmp);
		
		uart2_sendChar(0xfe);
		
		
		
		
		
		
		
		
		if(T_slowDown>0)T_slowDown--;
		
		
		
		ov7620_F_VSYNC = 0;
		return 1;	
	}
		
}



#include "speedControl.h"
#include "publicParameters.h"

//uchar F_speedControlClock = 0;
int D_speedNow;
long D_speedIntegration;
long D_speedOutput; 

int minCutveSpeedRate;




void ClockSpeedControl()//write into MCUinit.c
{
	//F_speedControlClock = 1;
	D_speedNow = I_pulseCounter_encode;
	encode_ClearCounter();
}


int D_last_speedIntegration = 0;
void ControlSpeed_motor(int D_Speed, int myMaxPower, int myMinPower) //
{
	long tmp;	
	D_speedIntegration = D_Speed - D_speedNow;
	
	//tmp = ABS(D_speedIntegration * P_PID);
	//tmp /= 100;
	//tmp+=1;
	D_speedOutput += D_speedIntegration;// * tmp;
	
	
	if (D_speedOutput < (myMinPower))D_speedOutput = myMinPower;
	if (D_speedOutput > (myMaxPower)){
		D_speedOutput = myMaxPower;
	}
		

	motor_SetPower((int)(D_speedOutput));
}


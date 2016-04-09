#ifndef __MOTOR_H__
#define __MOTOR_H__

//1.Include the head files
#include "derivative.h" /* include peripheral declarations */


#define motor_I_powerOutput 	FTM0_C4V
#define motor_I_enableHbridge 	GPIOD_PDOR
#define motor_Pn_enableHbridge	(0x00000001 << 6)



#define 	motor_midPWMValue	1249

#define 	motor_EnableHbridge()	motor_I_enableHbridge &= ~motor_Pn_enableHbridge
#define 	motor_DisableHbridge()	motor_I_enableHbridge |=  motor_Pn_enableHbridge
#define 	motor_SetPower(x)	(motor_I_powerOutput = (motor_midPWMValue + (x)))
#define 	motor_StopMotor()	(motor_I_powerOutput = (motor_midPWMValue))

#define 	motor_maxPower	(1249)
#define 	motor_minPower	(-1249)

#endif

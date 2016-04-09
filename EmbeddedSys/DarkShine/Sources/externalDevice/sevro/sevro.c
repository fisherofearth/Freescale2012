#include "sevro.h"

unsigned long valueOutput ;
/*
void sevro_TurnCamera(signed int direction)
{
	valueOutput = midValueCameraSevro - direction;

	if(valueOutput > maxValueCameraSevro)valueOutput = maxValueCameraSevro;
	if(valueOutput < minValueCameraSevro)valueOutput = minValueCameraSevro;
	sevro_I_cameraPWM = (uint)valueOutput;
}
*/

void sevro_TurnWheel(signed int direction)
{
	valueOutput = midValueWheelSevro - direction;

	//if(valueOutput > maxValueWheelSevro)valueOutput = maxValueWheelSevro;
	//if(valueOutput < minValueWheelSevro)valueOutput = minValueWheelSevro;
	sevro_I_wheelPWM = (uint)valueOutput;
}

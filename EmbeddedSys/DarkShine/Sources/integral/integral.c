#include "integral.h"

ulong 	C_integralClock = 0;
uint 	integralPeriod = 4;
uchar 	F_integral = 0;

void IntegralClock(void)
{
	C_integralClock ++;
	if(C_integralClock > integralPeriod)
	{
		C_integralClock = 0;
		F_integral = 1;
	}
}
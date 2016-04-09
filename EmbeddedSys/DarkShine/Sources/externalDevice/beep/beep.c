#include "beep.h"
signed int beep_F_enableBeep = 0;

void Switch_beep(uchar s){
	if(beep_F_enableBeep == 1)
	{
		switch(s){
			case Pr_off_beep:
				beep_I_control &= ~beep_Pn_control;
			break;
			default:
				beep_I_control |= beep_Pn_control;
		}
	}
	
}

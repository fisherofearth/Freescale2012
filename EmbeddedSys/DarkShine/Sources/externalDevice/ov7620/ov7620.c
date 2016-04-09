#include "ov7620.h"

uchar ov7620_F_VSYNC = 1;

void ov7620_Init(void) {
	ov7620_I_RESET &= ~ov7620_Pn_RESET;
	al422b_Init();
}

void ov7620_FlagVSYNC()
{
	ov7620_F_VSYNC = 1;
}


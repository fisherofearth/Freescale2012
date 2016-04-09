#include "led.h"

uint T_tailLED = 0;
uchar tailLEDSwitch = 0;

void led_SwitchLEachLED(uchar which, uchar state) {
	switch (which) {
	case 1:
		if (state == led_on)
			led_I_LED &= ~led_Pn_1;
		else
			led_I_LED |= led_Pn_1;
		break;
	case 2:
		if (state == led_on)
			led_I_LED &= ~led_Pn_2;
		else
			led_I_LED |= led_Pn_2;
		break;
	case 3:
		if (state == led_on)
			led_I_LED &= ~led_Pn_3;
		else
			led_I_LED |= led_Pn_3;
		break;
	case 4:
		if (state == led_on)
			led_I_LED &= ~led_Pn_4;
		else
			led_I_LED |= led_Pn_4;
		break;
	}
}

void ShineTailLed() {
	T_tailLED++;
	if (T_tailLED > 30) {
		T_tailLED = 0;
		tailLEDSwitch = ~tailLEDSwitch;
		TailLED(tailLEDSwitch);
	}
}

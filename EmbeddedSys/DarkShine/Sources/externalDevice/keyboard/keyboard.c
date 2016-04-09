#include "keyboard.h"
//#include  	".\external_device\LCD_LM9033\LCD_LM9033.H"
uchar keyboard_D_input = 0;
ulong T_keyPressed_keyboard = 0;

uchar keyboard_F_buttonCloseInterrupt = 0;

void delay_keyboard(unsigned int t) {
	while (t--);
}

int keyboard_ScanKeyboard(void) {
	delay_keyboard(0x5FFF);
	switch (keyboard_I_input) {
	case 14:
		if (T_keyPressed_keyboard <10 || T_keyPressed_keyboard > 100) {
			keyboard_D_input = keyboard_S_enterPressed ;
			T_keyPressed_keyboard += 10;
			return 1;
		}
		break;
	case 13:
		if (T_keyPressed_keyboard <10 || T_keyPressed_keyboard > 100) {
			keyboard_D_input = keyboard_S_closePressed;
			T_keyPressed_keyboard += 10;
			return 1;
		}
		break;
	case 11:
		if (T_keyPressed_keyboard <10 || T_keyPressed_keyboard > 100) {
			keyboard_D_input = keyboard_S_downPressed;
			T_keyPressed_keyboard += 10;
			return 1;
		}
		break;
	case 7:
		if (T_keyPressed_keyboard <10 || T_keyPressed_keyboard > 100) {
			keyboard_D_input = keyboard_S_upPressed;
			T_keyPressed_keyboard += 10;
			return 1;
		}
		break;
	default:
		keyboard_D_input = 0;
		T_keyPressed_keyboard = 0;
		return 0;
		break;
	}
}

void keyboard_InputClose(void) {
	keyboard_F_buttonCloseInterrupt = 1;
}
void keyboard_TimerKeyPressed(void) {
	T_keyPressed_keyboard++;
}


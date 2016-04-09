#ifndef __MENU_H__
#define __MENU_H__

//1.Include the head files
#include <string.h>
#include "externalDevice/oled/oled.h"
#include "externalDevice/keyboard/keyboard.h"
#include "chipDevice/chipDevice.h"
#include "SDparameters/SDparameters.h"

//2.Define the types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces


//4.Redefine the functions
extern int RunMenu();

typedef struct PARAMETERS{
	char 	*name;
	int 	*data;
	int 	min;
	int 	max;
};







#endif

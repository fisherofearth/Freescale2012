#include "menu.h"
#include "parameters.h"

int pointer_mainPage = 0;//主菜单中的光标
int pointer_ParameterPage = 0;//二级菜单中的光标
int pointer_page = 0;//选择页指针，0==主菜单，1==二级菜单,2==参数设定

char *menu_space[] = {
"",
" ",
"  ",
"   ",
"    ",
"     ",
"      ",
"       ",
"        ",
"         ",
"          ",
"           ",
"            ",
};


void DisplayBar(char *pagename)
{
	int i;
	uchar x;
	
	x = 0;
	oled_SetXY(x,0);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
		
	x+=6;
	oled_DisplayUnsignedNum(x,0,oled_closeLeft,0,2,oled_uninversion);
	
	x+=7;
	oled_SetXY(x,0);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
	
	x+=3;
	oled_WriteASSIC(x,0,pagename,oled_uninversion);
	
	x=123;
	oled_SetXY(x,0);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
	
	x-=(13);
	oled_DisplayUnsignedNum(x,0,oled_closeLeft,99,2,oled_uninversion);
	
	x-=3;
	oled_SetXY(x,0);
	oled_WriteData(0xff);
	oled_WriteData(0xff);
}

void DisplayMainPage() {
	int i;
	int inversion;
	
	for (i = 0; i < 7; i++) {
		if (pointer_mainPage == i)
			inversion = oled_inversion;
		else
			inversion = oled_uninversion;
		oled_WriteASSIC(0, 7 - i, itemsName[i], inversion);
	}
}

void DisplayParameterPage(uchar group) {
	int i,j;
	int list[7];
	int inversion;
	int tmp;
	
	oled_WriteASSIC(84, 4, ":", oled_uninversion);
	
	if (group == 1) {
		for (i = 3; i < 7; i++) {
			list[i] = pointer_ParameterPage + (i - 3);
			if (list[i] >= lengthOfGroup1)list[i] = list[i] % lengthOfGroup1;
		}
		for (i = 2; i >= 0; i--) {
			list[i] = pointer_ParameterPage - (3 - i);
			if (list[i] < 0)list[i] = lengthOfGroup1 + list[i];
		}
		for (i = 0; i < 7; i++) {
			if ((pointer_ParameterPage == list[i]) && (pointer_page < 2))
				inversion = oled_inversion;
			else
				inversion = oled_uninversion;
			oled_DisplayUnsignedNum(12, 7 - i, oled_closeRight, list[i] + 1, 2,inversion);
			tmp = strlen(parametersGroup1[list[i]].name);
			oled_WriteASSIC(84 - (tmp * 6), 7 - i,parametersGroup1[list[i]].name, inversion);
			oled_WriteASSIC(12, 7 - i, menu_space[12 - tmp], inversion);
			
			if ((i == 3) && (pointer_page == 2))
				inversion = oled_inversion;
			else
				inversion = oled_uninversion;
			oled_DisplaySignedNum(128, 7 - i, oled_closeRight,(*parametersGroup1[list[i]].data), 5, inversion);
		}
	}
	else
	{
		for (i = 3; i < 7; i++) {
			list[i] = pointer_ParameterPage + (i - 3);
			if (list[i] >= lengthOfGroup2)list[i] = list[i] % lengthOfGroup2;
		}
		for (i = 2; i >= 0; i--) {
			list[i] = pointer_ParameterPage - (3 - i);
			if (list[i] < 0)list[i] = lengthOfGroup2 + list[i];
		}
		for(i=0;i<7;i++)
		{
			if ((pointer_ParameterPage == list[i])&&(pointer_page <2))
				inversion = oled_inversion;
			else
				inversion = oled_uninversion;
			oled_DisplayUnsignedNum(12,7-i,oled_closeRight,list[i]+1,2,inversion);
			tmp = strlen(parametersGroup2[list[i]].name);
			oled_WriteASSIC(84-(tmp*6), 7-i, parametersGroup2[list[i]].name, inversion);
			oled_WriteASSIC(12,7-i,menu_space[12-tmp],inversion);
			
			if ((i == 3)&&(pointer_page == 2))
				inversion = oled_inversion;
			else
				inversion = oled_uninversion;
			oled_DisplaySignedNum(128,7-i,oled_closeRight,(*parametersGroup2[list[i]].data),5, inversion);
		}
	}
}




int currentMax;
int currentMin;
int *currentPointer;
void SelectPointer()
{
	switch(pointer_page)
	{
	case 0:
		pointer_ParameterPage=0;
		currentPointer = &pointer_mainPage;
		currentMax = 6;
		currentMin = 0;
		break;
	case 1:
		currentPointer = &pointer_ParameterPage;
		if(pointer_mainPage == 1)currentMax = lengthOfGroup1-1;//第1组参数
		if(pointer_mainPage == 2)currentMax = lengthOfGroup2-1;//第2组参数
		currentMin = 0;
		break;
	case 2:
		
		if(pointer_mainPage == 1)//第1组参数
		{
			currentPointer = parametersGroup1[pointer_ParameterPage].data;
			currentMax = parametersGroup1[pointer_ParameterPage].max;
			currentMin = parametersGroup1[pointer_ParameterPage].min;
		}
		if(pointer_mainPage == 2)//第2组参数
		{
			currentPointer = parametersGroup2[pointer_ParameterPage].data;
			currentMax = parametersGroup2[pointer_ParameterPage].max;
			currentMin = parametersGroup2[pointer_ParameterPage].min;
		}
		break;
	default:
		break;
	}
}


void InitMenu() {
	MCU_EnablePITInterrupt0();
			
	pointer_page = 0;
	pointer_ParameterPage=0;
	pointer_mainPage = 0;
	SelectPointer();
			
	oled_ClearScreen();
	DisplayBar("   Main Page");
	DisplayMainPage();
	

	pointer_ParameterPage = 0;
}

int RunMenu()
{
	InitMenu();
	
	for(;;)
	{
		if (keyboard_ScanKeyboard() != 0) {

			switch (keyboard_D_input) {
			case keyboard_S_upPressed:
				if (pointer_page == 2) {
					(*currentPointer)++;
					if ((*currentPointer) > currentMax)(*currentPointer) = currentMax;
				} else {
					(*currentPointer)--;
					if ((*currentPointer) < currentMin)(*currentPointer) = currentMax;
				}
				break;
			case keyboard_S_downPressed:
				if (pointer_page == 2) {
					(*currentPointer)--;
					if ((*currentPointer) < currentMin)(*currentPointer) = currentMin;
				} else {
					(*currentPointer)++;
					if ((*currentPointer) > currentMax)(*currentPointer) = currentMin;
				}
				break;
			case keyboard_S_enterPressed:
				if (pointer_page == 0) {
					switch (pointer_mainPage) {
					case 0:
					case 3:
					case 4:
					case 5:
					case 6:
						goto quit;
					}
				}
				if (pointer_page < 2)
				{
					pointer_page++;
					SelectPointer();
					if (pointer_page < 1)oled_ClearScreen();
				}
				break;
			case keyboard_S_closePressed:
				if (pointer_page > 0){
					pointer_page--;
					SelectPointer();
					if (pointer_page < 1)oled_ClearScreen();
					if (pointer_page == 1)	
						WritePublicParametersToSD();
				}
				else
				{
					pointer_mainPage = 0;
				}
				break;
			}

			switch (pointer_page) {
			case 0:
				DisplayMainPage();
				DisplayBar("   Main Page");
				break;
			case 1:
			case 2:
				DisplayParameterPage(pointer_mainPage);
				DisplayBar(itemsName[pointer_mainPage]);
				break;
			}
			keyboard_D_input = 0; //clear
		}
	}
	
	quit:
	keyboard_D_input = 0;
	MCU_DisablePITInterrupt0(); //关闭按键计时器
	oled_ClearScreen();
	return pointer_mainPage;
}
















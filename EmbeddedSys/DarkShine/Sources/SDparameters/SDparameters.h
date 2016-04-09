#ifndef _SDPARAMETERS_H_
#define _SDPARAMETERS_H_

//1.Include the head files
#include  "externalDevice/microSD/microSD.h"
//#include "main.h"
//2.Define the types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int


//4.Redefine public functions
extern void WritePublicParametersToSD();
extern void ReadPublicParametersFromSD();



typedef struct SD_PARAMETERS{
	int 	*data;
};


#endif








#include "SDparameters.h"
#include "parameters.h"

//#include "menu/parameters.h"
/*
#define BufferChar(address, data)	\
	microSD_fifoData[address] = (data)
*/
#define BufferInt(address, data)	\
	microSD_fifoData[address] = (uchar)(data);\
	microSD_fifoData[address + 1] = (uchar)((data) >> 8)
/*
#define BufferLong(address, data)	\
	microSD_fifoData[address] = (uchar)(data);\
	microSD_fifoData[address + 1] = (uchar)((data) >> 8);\
	microSD_fifoData[address + 2] = (uchar)((data) >> 16);\
	microSD_fifoData[address + 3] = (uchar)((data) >> 24)
*/
/*
#define RestoreChar(address, parameter)	\
	parameter = microSD_fifoData[address]
*/
#define RestoreInt(address, parameter)	\
	parameter =  (int)microSD_fifoData[address];\
	parameter += (((int)microSD_fifoData[address+1]) << 8)
/*
#define RestoreLong(address, parameter)	\
	parameter =  (long)microSD_fifoData[address];\
	parameter += (((long)microSD_fifoData[address+1]) << 8);\
	parameter += (((long)microSD_fifoData[address+2]) << 16);\
	parameter += (((long)microSD_fifoData[address+3]) << 24)
*/

void WritePublicParametersToSD()
{
	int i;
	for (i = 0; i < lengthSDParameters; i++) {
		BufferInt((i*2),(*SDParametersList[i].data));
	}
	microSD_WriteSector(sector_publicParameters);
}
void ReadPublicParametersFromSD()
{
	int i;
	microSD_ReadSector(sector_publicParameters);
	for (i = 0; i < lengthSDParameters; i++) {
		RestoreInt((i*2),(*SDParametersList[i].data));
	}
}



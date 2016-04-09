#include "microSD.h"

uchar microSD_fifoData[512];


void microSD_delay(uint t)
{
	while(t--);
}

uchar microSD_SendByte(uchar data)
{
	uchar tmp;
	
	//write data to TX FIFO
	SPI1_PUSHR |=  ((ulong)data);
	//start transfer
	SPI1_MCR &= ~SPI_MCR_HALT_MASK;
	//wait until transfer is over
	while( SPI_SR_TCF_MASK != (SPI1_SR & SPI_SR_TCF_MASK));	
	//clear TX FIFO 
	SPI1_MCR |= SPI_MCR_CLR_TXF_MASK;
	//clear TX flag
	SPI1_SR = SPI1_SR | SPI_SR_EOQF_MASK | SPI_SR_TCF_MASK;
	//get RX 
	tmp = (uchar)SPI1_POPR;
	//clear RX FIFO
	SPI1_MCR |= SPI_MCR_CLR_RXF_MASK;
	
	return tmp;
}
#define microSD_SendClock()		microSD_SendByte(0xFF)
#define microSD_ReadByte()		microSD_SendByte(0xFF)

#define microSD_SendLong(x) \
	microSD_SendByte((uchar)((x)>>24));\
	microSD_SendByte((uchar)((x)>>16));\
	microSD_SendByte((uchar)((x)>> 8));\
	microSD_SendByte((uchar)((x)>> 0))


uchar microSD_SendCMD(uchar commond, ulong parameter, uchar CRC)
{
	uchar tmp;
	
	SetCStoHigh();
	microSD_SendByte(0xFF);
	SetCStoLow(); 
	
	microSD_SendByte(commond | 0x40);
	microSD_SendLong(parameter);
	microSD_SendByte(CRC | 0x01);
	
	microSD_SendClock();
	tmp = microSD_ReadByte();
	
	return tmp;
}


uchar microSD_InitDevice(void)
{
	uchar tmp;
	uchar i;
	uchar C_tmp; 
	SetCStoHigh(); //CS置高
	
	for(i=0;i<15;i++)
	{
		microSD_SendClock();
	}
	SetCStoLow(); //CS置低
	microSD_delay(0xFFff);
	
	C_tmp = 0;
	do
	{
		tmp = microSD_SendCMD(CMD0, 0, 0x95);//复位
		C_tmp ++;
		if(C_tmp > 100)
		{
			return microSD_error_init;
		}
	}while(tmp != 1);
	
	C_tmp = 0;
	do
	{
		tmp = microSD_SendCMD(CMD1, 0, 0xff);//初始化
		C_tmp ++;
		if(C_tmp > 200)
		{
			return microSD_error_init;
		}
	}while(tmp != 0);

	tmp = microSD_SendCMD(CMD59, 0, 0xff);
	/*
	C_tmp = 0;
	do
	{
		tmp = microSD_SendCMD(CMD16, microSD_block, 0xff);
		C_tmp ++;
		if(C_tmp > 200)
		{
			return microSD_error_init;
		}
	}while(tmp == 0xFF);*/

	SetCStoHigh(); //CS置高
	return 0;
}


uchar microSD_WriteSector(ulong sectorAddress)
{
	uint i;
	uchar tmp;
	uchar C_tmp;
	
	sectorAddress = sectorAddress *512;
	C_tmp = 0;
	do
	{
		tmp = microSD_SendCMD(CMD24, sectorAddress, 0xFF);

		C_tmp ++;
		if(C_tmp > 200)
		{
			return microSD_error_writeData;
		}
	}while(tmp != 0);
	
	microSD_SendByte(0xfe);
	
	for(i=0;i<microSD_block;i++)
	{
		microSD_SendByte(microSD_fifoData[i]);
	}
	microSD_SendByte(0xFf);
	microSD_SendByte(0xFf);
	tmp = microSD_SendByte(0xFf);
	
	C_tmp = 0;
	do
	{
		tmp = microSD_ReadByte();
		C_tmp ++;
		if(C_tmp > 200)
		{
			return microSD_error_writeData;
		}
	}while(tmp != 0xFF);	
	
	SetCStoHigh(); //CS置高
	microSD_SendClock();
	
	return 0;
}


uchar microSD_ReadSector(ulong sectorAddress)
{
	uint i;
	uchar tmp;
	uchar C_tmp;
	
	sectorAddress = sectorAddress *512;
	
	C_tmp = 0;
	do
	{
		tmp = microSD_SendCMD(CMD17, sectorAddress, 0xFF);
		C_tmp ++;
		if(C_tmp > 200)
		{
			return microSD_error_readData;
		}
	}while(tmp != 0);
	
	C_tmp = 0;
	do
	{
		tmp = microSD_ReadByte();
		if(C_tmp > 200)
		{
			return microSD_error_readData;
		}
	}while(tmp != 0xfe);
	for(i=0;i<microSD_block;i++)
	{
		microSD_fifoData[i] = microSD_ReadByte();
	}
	microSD_SendClock();
	microSD_SendClock();
	SetCStoHigh(); //CS置高
	return 0;
}

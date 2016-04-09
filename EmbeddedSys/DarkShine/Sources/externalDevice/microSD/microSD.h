
#ifndef _MICROSD_H_
#define _MICROSD_H_


//1.Include the head files
#include "derivative.h" /* include peripheral declarations */

//2.Define the types
#define	ulong	unsigned long
#define uchar 	unsigned char
#define uint 	unsigned int

//3.Define interfaces


//4.Redefine public functions
extern uchar microSD_InitDevice(void);
extern uchar microSD_WriteSector(ulong sectorAddress);
extern uchar microSD_ReadSector(ulong sectorAddress);

#define SetCStoHigh() SPI1_MCR |= SPI_MCR_PCSIS(1)
#define SetCStoLow()  SPI1_MCR &= ~SPI_MCR_PCSIS(1)

//5.Redefine public variable
extern uchar microSD_fifoData[512];

//6.Define private parameter
#define CMD0    0   /* GO_IDLE_STATE */  
#define CMD1    1   /* SEND_OP_COND */  
#define CMD8    8   /* SEND_IF_COND */  
#define CMD9    9   /* SEND_CSD */  
#define CMD10   10  /* SEND_CID */  
#define CMD12   12  /* STOP_TRANSMISSION */  
#define CMD13   13  /* SEND_STATUS */  
#define CMD16   16  
#define CMD17   17  /* READ_SINGLE_BLOCK */  
#define CMD18   18  /* READ_MULTIPLE_BLOCK */  
#define CMD24   24  /* WRITE_BLOCK */  
#define CMD25   25  /* WRITE_MULTIPLE_BLOCK */  
#define CMD58   58   
#define CMD59   59  

#define microSD_error_init 		1
#define microSD_error_writeData	1
#define microSD_error_readData 	1

#define microSD_block 512

//7.Define Public parameter


#endif

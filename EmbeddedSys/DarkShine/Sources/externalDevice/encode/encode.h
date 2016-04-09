#ifndef __ENCODE_H__
#define __ENCODE_H__


//1.Include the head files
#include "derivative.h" /* include peripheral declarations */

//2.Define types

//3.Define interfaces
#define I_pulseCounter_encode 	LPTMR0_CNR

//4.Redefine public functions
#define encode_EnableCounter() 		LPTMR0_CSR |=  LPTMR_CSR_TEN_MASK
#define encode_DisableCounter() 	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK
#define encode_ClearCounter()  		LPTMR0_CSR&=~LPTMR_CSR_TEN_MASK;LPTMR0_CSR|=LPTMR_CSR_TEN_MASK

//5.Redefine public variable

//6.Define private parameter


#endif

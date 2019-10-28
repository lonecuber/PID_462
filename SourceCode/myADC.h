/* Texas A&M University
** Electronic Systems Engineering Technology
** ESET-420 Capstone II ** Author: Jacob Faseler
** File: ADC.h
** --------
** Interactions with the ADC block
*/
#ifndef SOURCECODE_MYADC_H_
#define SOURCECODE_MYADC_H_

/* Bare-Metal Info */
#define ADC12_A_BASE_ADDR (uint16_t)0x0700

/* TI Kernel-Aware Drivers don't support ADC, so driverlib */
#include <driverlib/MSP430F5xx_6xx/adc12_a.h>
#include <driverlib/MSP430F5xx_6xx/inc/hw_memmap.h>

/* TI Runtime library */
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Task.h>

/* TI Gate Mutex */
#include <ti/sysbios/gates/GateMutex.h>

/* std types */
#include <stdint.h>
#include <stdbool.h>

/* Function Prototypes */
void ConstructADCmutex(void);
void InitializeADCs (uint16_t base_address);
uint16_t readADC(uint16_t base_address, uint8_t index);

#endif /* SOURCECODE_MYADC_H_ */

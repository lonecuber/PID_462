/* Texas A&M University
** Electronic Systems Engineering Technology
** ESET-462 Control Systems ** Author: Jacob Faseler
** File: ADC.c
** --------
** Interactions with the ADC block
*/


/* Include own header */
#include "SourceCode/myADC.h"

/* ADC Mutex Resources */
GateMutex_Struct adc_mut_struct;
GateMutex_Params adc_mut_params;
GateMutex_Handle adc_mutex;

/*
 * ConstructADCmutex() - Self-descriptive
 * Params: none
 * Return: none
 */
void ConstructADCmutex(void)
{
	GateMutex_Params_init(&adc_mut_params);
	GateMutex_construct(&adc_mut_struct, &adc_mut_params);
	adc_mutex = GateMutex_handle(&adc_mut_struct);
}


/*
 * initADCs() - Configures ADC Block and derives meaning for handle
 * 					Using driverlib
 * Params: base_addr - Base address of the ADC
 * Return: none
 */
void InitializeADCs (uint16_t base_address)
{
	bool success;
	ADC12_A_configureMemoryParam adc_param = {0};

	/* Allowing ADC interrupts breaks RTOS */
	ADC12_A_disableInterrupt(base_address, 0xFFFF);

	/* Initialize block, exception on failure */
	success = ADC12_A_init(base_address,
							ADC12_A_SAMPLEHOLDSOURCE_SC,
							ADC12_A_CLOCKSOURCE_MCLK,
							ADC12_A_CLOCKDIVIDER_32);
	if (success == STATUS_FAIL)
	{
		System_abort("Error: myADC/initADCs - success");
	}

	/* Enable block */
	ADC12_A_enable(base_address);

	/* Setup sampling timer to take single samples
	 * */
	ADC12_A_setupSamplingTimer(base_address,
								ADC12_A_CYCLEHOLD_1024_CYCLES,
								ADC12_A_CYCLEHOLD_4_CYCLES,
								ADC12_A_MULTIPLESAMPLESDISABLE);

	/* Input to mem buf w/ ref v's */
	adc_param.memoryBufferControlIndex = ADC12_A_MEMORY_0;
	adc_param.inputSourceSelect = ADC12_A_INPUT_A0;
	adc_param.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
	adc_param.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
	adc_param.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
	ADC12_A_configureMemory(base_address, &adc_param);
}


/*
 * readADC() - Thread-safe access to ADC Block
 * 					Using TI kernel-aware drivers
 * Params: index - index of ADC's memory buffer as defined in
 * 					driverlib/MSP430F5xx_6xx/adc12_a.h on line 262
 * Return: uint16_t adcValue - Value read from the ADC
 */
uint16_t readADC(uint16_t base_address, uint8_t index){
	uint16_t result;
	IArg key;

	/* Only access hardware while holding the keys */
	key = GateMutex_enter(adc_mutex);

	/* Start Conversion running */
	ADC12_A_startConversion(base_address,
							ADC12_A_MEMORY_0,
							ADC12_A_SINGLECHANNEL);
	/* Stop conversions just before reading and ensure that last
	 * conversion is complete to ensure sensible data, read data out,
	 * and then set ADC back to work
	 * (max 4096 clocks, which is 10 systicks) */
	ADC12_A_disableConversions(base_address, ADC12_A_COMPLETECONVERSION);
	while (ADC12_A_isBusy(base_address) == ADC12_A_BUSY)
	{
		Task_sleep(3);
	}
	result = ADC12_A_getResults(base_address,index);

	/* Return the keys before returning to caller */
	GateMutex_leave(adc_mutex,key);

	return result;
}

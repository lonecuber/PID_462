/* Texas A&M University
** Electronic Systems Engineering Technology
** ESET-420 Capstone II ** Author: Jacob Faseler
** File: Health.h
** --------
** Description of the contents of this file
*/
#ifndef SOURCECODE_HEALTH_H_
#define SOURCECODE_HEALTH_H_

/* XDCtools Header files */
#include <xdc/std.h>

/* BIOS Header files */
#include <ti/sysbios/knl/Clock.h>

/* Task Stack Definitions */
#define TASKSTACKSIZE   256

/* Constants */
#define FLASH_FREQ_HZ 1
#define FLASH_PERIOD_TICKS (((1000000 / Clock_tickPeriod) / FLASH_FREQ_HZ ) /2)

/* Function Prototypes */
void constructHeartbeat (void);
void heartBeatFxn (UArg arg0, UArg arg1);

#endif /* SOURCECODE_HEALTH_H_ */

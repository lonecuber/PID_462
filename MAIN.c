/* Texas A&M University
** Electronic Systems Engineering Technology
** ESET-462 Control Systems ** Author: Jacob Faseler
** File: MAIN.c
** --------
** Main Sourcecode for porting functionality of
** 419 testbench to TI-RTOS
*/

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/gates/GateMutex.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Board Header file */
#include "Board.h"

/* Controller Family Header Files */
#include <ti/sysbios/family/msp430/Power.h>

/* Project Header files */
#include "SourceCode/Health.h"
#include "SourceCode/SerialComm.h"
#include "SourceCode/myADC.h"


 /*
 **  !!! Idle Task is configured to enter LPM4 !!!
 */

int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initUART();
    //Board_initWatchdog();

    /* Initialize HW blocks */
    InitializeSerialBlocks();
    InitializeADCs(ADC12_A_BASE_ADDR);

    /* Construct kernel primitives */
    ConstructSerialMutexes();
    ConstructSerialSemaphores();
    ConstructSerialQueues();
    ConstructADCmutex();

    /* Construct Tasks */
    constructHeartbeat();
    ConstructBackchannelWriteTask();
    ConstructBackChannelReadTask();

    /* Start BIOS */
    BIOS_start();

    return (0);
}

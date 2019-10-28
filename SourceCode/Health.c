/* Texas A&M University
** Electronic Systems Engineering Technology
** ESET-420 Capstone II ** Author: Jacob Faseler
** File: Health.c
** --------
** Contains Heartbeat function and tasks related to
** monitoring health of the kernel
*/

#include "SourceCode/Health.h"

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* Board Header file */
#include "Board.h"

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* Global Resources */
#include "SourceCode/Priorities.h"

/* Heartbeat Task Resources */
Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

/* Global Task Resources */
Task_Params taskParams;


 /*
 ** Constructs task resources for the heartbeat thread
 */
void constructHeartbeat (void){
	Task_Params_init(&taskParams);
	taskParams.arg0 = FLASH_PERIOD_TICKS;
	taskParams.stackSize = TASKSTACKSIZE;
	taskParams.stack = &task0Stack;
	taskParams.priority = HRTBTPRIORITY;
	taskParams.__iprms.name = "Heartbeat";
	Task_construct(&task0Struct, (Task_FuncPtr)heartBeatFxn, &taskParams, NULL);
}

 /*
 ** Heartbeat Task Function
 **
 ** Toggles LED at 1 Hz to indicate functional kernel
 */
Void heartBeatFxn(UArg arg0, UArg arg1){
    while (1)
    {
        Task_sleep((unsigned int)arg0);
        GPIO_toggle(Board_LED0);
    }
}

/* Texas A&M University
** Electronic Systems Engineering Technology
** ESET-420 Capstone II ** Author: Jacob Faseler
** File: SerialComm.c
** --------
** Contains Tasks, Threads, and Functions pertaining to
** the backchannel UART serial port (USCI_A1) and
** inter-device comm (USCI_A0).
*/

#include "SourceCode/SerialComm.h"

/* XDC Header Files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header Files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/gates/GateMutex.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/family/msp430/Hwi.h>

/* TI Drivers */
#include "ti/drivers/UART.h"

/* Board Header file */
#include "Board.h"

/* Standard Includes */
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/* Global Resources */
#include "SourceCode/Priorities.h"

/* Mutex Resources */
GateMutex_Params mutex_params;
GateMutex_Struct bc_ser_tx_struct;
GateMutex_Struct bc_ser_rx_struct;
GateMutex_Struct bc_tx_q_struct;

/* Private Serial Mutex Handles */
GateMutex_Handle bc_ser_tx_mutex;
GateMutex_Handle bc_ser_rx_mutex;
GateMutex_Handle bc_tx_q_mutex;

/* Semaphore Resources */
Semaphore_Params sem_params;
Semaphore_Struct bc_tx_slots_struct;

/* Semaphore Handles */
Semaphore_Handle bc_tx_slots;

/* Global Task Resources */
Task_Params taskParams;

/* Backchannel Write Task Resources */
Task_Struct bc_write_struct;
Char bc_write_stack[BCWSTACKSIZE];

/* Backchannel Read Task Resources */
Task_Struct bc_read_struct;
Char bc_read_stack[BCRSTACKSIZE];

/* Backchannel Test Task Resources */
Task_Struct bc_test_struct;
Char bc_test_stack[BCTSTACKSIZE];

/* UART Handles, Params, Buffers */
UART_Params uart_params;
UART_Handle bc_uart;
q_el_t bc_tx_q[TX_BUF_SIZE_BC];
unsigned char bc_rx_q[RX_BUF_SIZE_BC];


/* TASK: BackChannelReadTask - Places user-input into queue until a \r
 * 								 is registered, interpreted and calls the
 * 								 appropriate fn.
 * Params:	None
 * Return:	NEVER
 * Notes:	No guard for RX queue, as this should be the only task interacting
 * 			with it. Command echo uses public function for safely accessing
 * 			TX channel. Task grabs bc_rx key until entire string is dictated.
 *
 * 			In transfer loop, it is vital to peek for newline before reading
 * 			to ensure that terminating character is ingested, preventing
 * 			infinite loops.
 */
void BackChannelReadTask(UArg arg0, UArg arg1)
{
	uint8_t i;
	bool cond;
	IArg key_hw;
	const char token1[10] = "Mitelight\r";
	const char token2[8] = "TestSMS\r";
	const char token3[13] = "TestPriority\r";
	char str[20];

	while(1)
	{

		/* Clear Buffer */
		memset(bc_rx_q, 0, RX_BUF_SIZE_BC);

		/* Spin until there are characters in the buffer to process */
		while (UART_control(bc_uart, UART_CMD_GETRXCOUNT, NULL) == 0)
		{
			Task_sleep (BC_LATENCY);
		}
		/* Transfer Character into RX buffer, HW guarded by gate */
		key_hw = GateMutex_enter(bc_ser_rx_mutex);
		for (i = 0 , cond = TRUE; cond ; i++){
			UART_read(bc_uart, &bc_rx_q[i], sizeof(unsigned char));
			cond = (bc_rx_q[i] != '\r') && (i < RX_BUF_SIZE_BC);
		}
		GateMutex_leave (bc_ser_rx_mutex, key_hw);

		/* Interpret, Tokenize, etc. */
		if (!memcmp(token1, bc_rx_q, sizeof(token1)))
		{
			BcPuts ("\n\rCo, Ltd.", DEBUG_MSG);
		}
		else if (!memcmp(token2, bc_rx_q, sizeof(token2)))
		{
			BcPuts ("\n\rAT + SMS 18309998888 This is a Test", DEBUG_MSG);
		}
		else if (!memcmp(token3, bc_rx_q, sizeof(token3)))
		{
			BcPuts ("Low Priority 1", DEBUG_MSG);
			BcPuts ("Low Priority 2", DEBUG_MSG);
			BcPuts ("Low Priority 3", DEBUG_MSG);
			BcPuts ("High Priority Message", URGENT_MSG);
		}
		else
		{
			BcPuts("\n\rNo Recognized Tokens", DEBUG_MSG);
			sprintf(str, "ADC Reads: %d", readADC(ADC12_A_BASE_ADDR,ADC12_A_MEMORY_0));
			BcPuts(str,DEBUG_MSG);
		}
	}
}


/* TASK: BackChannelWriteTask - Constantly writes highest priority entry
 * 								from queue to backchannel UART buffer
 * Params:	None
 * Return:	NEVER
 * Notes:	Iterating through elements ages them, providing them an
 * 			advantage for selection in the case of priority ties. Not
 * 			completely FIFO for same priority messages, but close enough.
 */
void BackChannelWriteTask(UArg arg0, UArg arg1)
{
	uint8_t max_pri_i;
	int16_t i;
	IArg key_hw;
	IArg key_q;

	while (1)
	{
		i = 0;

		/* Spin until data is written to the queue */
		while (Semaphore_getCount(bc_tx_slots) == TX_BUF_SIZE_BC){
			Task_sleep(BC_LATENCY);
		}

		/* Scan queue for highest priority message, favoring seniority
		 * and aging each element not selected. Only access queue while
		 * inside corresponding gate.
		 */
		max_pri_i = 0;

		for (i = 0 ; i < TX_BUF_SIZE_BC ; i++)
		{
			if ((!bc_tx_q[i].is_open) 								&&
				( bc_tx_q[i].priority > bc_tx_q[max_pri_i].priority)	&&
				(bc_tx_q[i].age > bc_tx_q[max_pri_i].age) )
			{
				max_pri_i = i;
			}
			else
			{
				if (!bc_tx_q[i].is_open)
				{
					bc_tx_q[i].age++;
				}
			}
		}

		/* Write Message (HW protected by gate) */
		key_hw = GateMutex_enter(bc_ser_tx_mutex);
	    UART_write(bc_uart, bc_tx_q[max_pri_i].message, sizeof(bc_tx_q[max_pri_i].message));
	    UART_write(bc_uart, "\r\n", sizeof("\r\n"));

		/* reset slot */
		bc_tx_q[max_pri_i].is_open = TRUE;
		bc_tx_q[max_pri_i].priority = PRIORITY_COUNT;
		bc_tx_q[max_pri_i].age = 0;
		for (i = 0 ; i < TX_MESSAGE_SIZE ; i++)
		{
			bc_tx_q[max_pri_i].message[i] = 0;
		}

		/* Tell the other tasks what's been done */
		Semaphore_post(bc_tx_slots);

		/* Share your toys with the other kids on the block */
		GateMutex_leave(bc_ser_tx_mutex, key_hw);
		GateMutex_leave(bc_tx_q_mutex, key_q);
		Task_sleep(100);
	}
}


/*TASK - Test Back Channel Write: Strains backchannel uart by repetitively
 * 			posting to it.
 * Params:	None
 * Return:	NEVER
 * Notes:
 */
void TestBackChannelWrite(UArg arg0, UArg arg1)
{
	while (1)
	{
		BcPuts("Hello,World!", DEBUG_MSG);
		Task_sleep(50);
	}
}


/* BcPuts() - 	Public function to interface the BackChannel Queue. Writes
 * 				to first available slot, overwriting only if higher priority.
 * Params:	message	- The string to be written to backchannel UART
 * 			length	- Number of bytes in message
 * 			priority	- the priority level of the message
 * Return:	Serial process success / error indicator
 * Notes:	Takes 2 * BC_LATENCY + Processor time max to post priority message
 * 			to serial port. Priority overwrite indexing gives no weight to age.
 * 			Call blocks for a maximum of 2 * BC_LATENCY + Processor time. To
 * 			block until message is posted, call inside while loop:
 * 				while(BcPuts(params) == SERIAL_QUEUE_FULL){;}
 */
Ser_Ret_t BcPuts(void* message, Ser_Mess_Pri_t priority)
{
	IArg key;
	uint8_t i;
	bool posted;

	/* Needs to start false each time called */
	posted = FALSE;

	/* Wait until there's an available slot to enter gate */
	if (Semaphore_pend(bc_tx_slots, BC_LATENCY))
	{
		key = GateMutex_enter(bc_tx_q_mutex);
		for (i = 0 ; ((i < TX_BUF_SIZE_BC) && (!posted)) ; i++)
		{
			if (bc_tx_q[i].is_open)
			{
				strcpy(bc_tx_q[i].message, message);
				bc_tx_q[i].is_open = FALSE;
				bc_tx_q[i].age = 0;
				bc_tx_q[i].priority = priority;
				posted = TRUE;
			}
		}
		GateMutex_leave(bc_tx_q_mutex, key);
	}
	/* Unless you're impatient, and then only overwrite if important */
	else
	{
		key = GateMutex_enter(bc_tx_q_mutex);
		for (i = 0 ; ((i < TX_BUF_SIZE_BC) && (!posted)) ; i++)
		{
			if (bc_tx_q[i].priority < priority)
			{
				strcpy(bc_tx_q[i].message, message);
				bc_tx_q[i].is_open = FALSE;
				bc_tx_q[i].age = 0;
				bc_tx_q[i].priority = priority;
				posted = TRUE;
			}
		}
		GateMutex_leave(bc_tx_q_mutex, key);
	}

	if (posted)
	{
		return SERIAL_SUCCESS;
	}
	return SERIAL_QUEUE_FULL;
}


/*
 * InitializeSerialBlocks() - Uses TI drivers to configure HW USCI blocks
 * 							to operate in UART mode with desired performance:
 * 							USCI_A1 (backchannel) operates at 9k6 baud, and
 * 							USCI_A0 (device-channel) operates according to
 * 							uart channel specifications of HL7800.
 * 								115200 baud
 * Params: None
 * Return: None
 */
void InitializeSerialBlocks (void)
{

	/* Initialize Backchannel UART (from TI example) */
	UART_Params_init(&uart_params);
	uart_params.writeDataMode = UART_DATA_BINARY;
	uart_params.readDataMode = UART_DATA_BINARY;
	uart_params.readReturnMode = UART_RETURN_FULL;
	uart_params.readEcho = UART_ECHO_ON;
	uart_params.baudRate = 9600;
	bc_uart = UART_open(Board_UART0, &uart_params);

	if (bc_uart == NULL)
	{
		System_abort("Error opening the bcUART");
	}
}


/* ConstructBackchannelWriteTask() - Construct task for reading from
 * 									backchannel UART connection
 * Params: None
 * Return: None
*/
void ConstructBackChannelReadTask(void)
{
	Task_Params_init(&taskParams);
	taskParams.stackSize = BCRSTACKSIZE;
	taskParams.stack = &bc_read_stack;
	taskParams.priority = BC_READ_TASK_PRIORITY;
	taskParams.instance->name = "BackChannel Read";
	Task_construct(&bc_read_struct, (Task_FuncPtr)BackChannelReadTask,
					&taskParams, NULL);
}


/* ConstructBackchannelWriteTask() - Construct task for writing to backchannel
 * 									UART connection
 * Params: None
 * Return: None
*/
void ConstructBackchannelWriteTask (void)
{
	Task_Params_init(&taskParams);
	taskParams.stackSize = BCWSTACKSIZE;
	taskParams.stack = &bc_write_stack;
	taskParams.priority = BC_WRITE_TASK_PRIORITY;
	taskParams.instance->name = "BackChannel Write";
	Task_construct(&bc_write_struct, (Task_FuncPtr)BackChannelWriteTask,
					&taskParams, NULL);
}


/* ConstructBackchannelTest() - Construct task for testing BackChannelWriteTask
 * 								and BcPuts functions
 * Params: None
 * Return: None
*/
void ConstructBackchannelTest (void)
{
	Task_Params_init(&taskParams);
	taskParams.stackSize = BCTSTACKSIZE;
	taskParams.stack = &bc_test_stack;
	taskParams.priority = BC_TEST_TASK_PRIORITY;
	taskParams.instance->name = "BackChannel Test";
	Task_construct(&bc_test_struct, (Task_FuncPtr)TestBackChannelWrite, &taskParams, NULL);
}


 /*
  * ConstructSerialQueues: Initializes Serial Queues with 'NULL'
  * Params:	None
  * Return:	None
  */
void ConstructSerialQueues (void)
{
	uint8_t i;
	uint8_t j;

	for (i = 0 ; i < TX_BUF_SIZE_BC ; i++)
	{
		bc_tx_q[i].is_open = TRUE;
		bc_tx_q[i].priority = PRIORITY_COUNT;
		for(j = 0 ; j < TX_MESSAGE_SIZE ; j++)
		{
			bc_tx_q[i].message[j] = NULL;
		}
	}
}


/*
* Construct Serial Mutexes() Self-Descriptive
* Params:	None
* Return:	None
* Notes:	The TI Drivers for the USCI UART block supports full duplex
* 			operation. To safely take advantage of this, separate guards
* 			are used for each channel in each direction of operation.
*/
void ConstructSerialMutexes (void){
	/* Initialize Universal Resources */
	GateMutex_Params_init(&mutex_params);

	/* Configure params for backchannel tx guard */
	mutex_params.instance->name = "BC_TX";
	GateMutex_construct(&bc_ser_tx_struct, &mutex_params);
	bc_ser_tx_mutex = GateMutex_handle(&bc_ser_tx_struct);

	/* Configure params for backchannel rx guard */
	mutex_params.instance->name = "BC_RX";
	GateMutex_construct(&bc_ser_rx_struct, &mutex_params);
	bc_ser_rx_mutex = GateMutex_handle(&bc_ser_rx_struct);

	/* Configure params for backchannel tx queue guard */
	mutex_params.instance->name = "BC_TX_Q";
	GateMutex_construct(&bc_tx_q_struct, &mutex_params);
	bc_tx_q_mutex = GateMutex_handle(&bc_tx_q_struct);
}


/*
 * ConstructSerialSemaphores: Builds Primitives
 * Params:	None
 * Return:	None
 * Notes:	at_ok_		:	Binary	:	indicates dev read task has recieved
 * 										 the string "OK"
 *
 * 			dev_tx_ready	:	Binary	:	indicates dev_tx_q has been written
 * 										 to and is ready to be transmitted
 *
 * 			bc_tx_slots	:	Counting	:	indicates number of available slots
 * 										 in queue for new messages
 */
void ConstructSerialSemaphores (void){
	Semaphore_Params_init(&sem_params);

	/* Binary Semaphores */
		/*none*/
	/* Counting Semaphores */
	sem_params.mode = Semaphore_Mode_COUNTING;
	sem_params.__iprms.name = "BcTxSlotsSem";
	sem_params.__iprms.__size = sizeof("BcTxSlotsSem");
	Semaphore_construct(&bc_tx_slots_struct, TX_BUF_SIZE_BC, &sem_params);
	bc_tx_slots = Semaphore_handle(&bc_tx_slots_struct);
}

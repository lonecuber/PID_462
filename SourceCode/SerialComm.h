/* Texas A&M University
** Electronic Systems Engineering Technology
** ESET-420 Capstone II ** Author: Jacob Faseler
** File: SerialComm.h
** --------
** Description of the contents of this file
*/
#ifndef SOURCECODE_SERIALCOMM_H_
#define SOURCECODE_SERIALCOMM_H_

/* BIOS Includes */
#include <ti/sysbios/knl/Semaphore.h>

/* XDC Tools Includes */
#include <xdc/std.h>

/* TI Driver Includes */
#include <ti/drivers/UART.h>

/* Standard Includes */
#include <stdbool.h>

/* Project Includes */
#include "SourceCode/myADC.h"

/* Numerical Constants */
#define BCTSTACKSIZE 256
#define BCWSTACKSIZE 256
#define BCRSTACKSIZE 256
#define BACKCHANNEL_BAUD 9600
#define BACKCHANNEL_TEST_DELAY 100
#define TX_BUF_SIZE_BC 3
#define RX_BUF_SIZE_BC 150
#define TX_MESSAGE_SIZE 100
#define DEV_TX_Q_SIZE 1
#define BC_LATENCY 200
#define MBX_SLOTS 1
#define MBX_TIMEOUT 500

/* Typedefs */
typedef enum Serial_Return_type {SERIAL_FAIL, SERIAL_SUCCESS,
								SERIAL_QUEUE_FULL} Ser_Ret_t;
typedef enum serial_message_priority_type 	{PRIORITY_COUNT, DEBUG_MSG,
											ERROR_MSG, URGENT_MSG}
											Ser_Mess_Pri_t;
typedef struct queue_element_type {
	char message[TX_MESSAGE_SIZE];
	Ser_Mess_Pri_t priority;
	uint8_t age;
	bool is_open;
} q_el_t;

/* Bare-Metal Constants */
#define USCI_A0_UART_BASE ((uint16_t)0x05C0)
#define USCI_A1_UART_BASE ((uint16_t)0x0600)

/* Public Backchannel Access */
Ser_Ret_t BcPuts(void* message, Ser_Mess_Pri_t priority);

/* Task fn Prototypes */
void TestBackChannelWrite(UArg arg0, UArg arg1);
void BackChannelWriteTask(UArg arg0, UArg arg1);
void BackChannelReadTask(UArg arg0, UArg arg1);

/* Task Construction */
void ConstructBackChannelReadTask (void);
void ConstructBackchannelWriteTask(void);
void ConstructBackchannelTest (void);

/* Primitive Construction */
void InitializeSerialBlocks (void);
void ConstructSerialSemaphores (void);
void ConstructSerialMutexes (void);
void ConstructSerialQueues (void);

#endif /* SOURCECODE_SERIALCOMM_H_ */

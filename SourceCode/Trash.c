/* Texas A&M University
** Electronic Systems Engineering Technology
** ESET-420 Capstone II ** Author: Jacob Faseler
** File: Trash.c
** --------
** Stuff that could potentially be useful later
*/

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
 #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

/* Board Header file */
#include "Board.h"

void trash (void){
	System_printf("Program Start!");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();
}

void read_callback_fn(UART_Handle handle, void* buf, size_t count){
	;
}


void write_callback_fn(UART_Handle handle, void* buf, size_t count){

}

#ifdef GARBAGE
uart_params.readCallback = read_callback_fn;
uart_params.writeCallback = write_callback_fn;



/*
** Attempts to write to backchannel Tx Queue by first posting the
** bc_serial_tx_mutex and then the bc_tx_q_clr semaphore. The mutex can
** block indefinitely, but the Semaphore blocks without timeout so that
** the write function can clear the queue and raise the semaphore (mutex
** required to raise semaphore, like a locked flag pole)
*/
Ser_Ret_t WriteBackChannel (char string[], size_t size){
	uint8_t ret;
	IArg key;

	ret = 0;
	key = GateMutex_enter(bc_serial_tx_mutex);
	ret = UART_writePolling(bc_serial, string, size);
	GateMutex_leave(bc_serial_tx_mutex, key);
	if (ret == 0) return SERIAL_SUCCESS;
	else return SERIAL_FAIL;
}

/*
** ISR for recieved transmissions on the backchannel UART (indicated by a
** newline).
*/
Ser_Ret_t ReadBackChannel (char buf[], size_t buf_size){
	uint8_t ret;
	IArg key;
	uint8_t i;

	ret = 0;
	key = GateMutex_enter(bc_serial_rx_mutex);
	for (i = 0 ; ((i < RX_BUF_SIZE) && (buf[i-1] != '\n')) ; i++){
	;
	}
	ret = UART_read(bc_serial, buf, (size_t)1);
	GateMutex_leave(bc_serial_rx_mutex, key);
	if (ret == 1) return SERIAL_SUCCESS;
	if (ret > 1 ) return SERIAL_TOO_MANY;
	return SERIAL_FAIL;
}

/* Semaphores Not yet in use */

Semaphore_Struct read_main_sem_struct;
Semaphore_Struct read_label_sem_struct;
Semaphore_Struct read_ui_sem_struct;

Semaphore_Handle read_main_sem;
Semaphore_Handle read_label_sem;
Semaphore_Handle read_ui_sem;

sem_params.__iprms.name = "ReadMainSem";
sem_params.__iprms.__size = sizeof("ReadMainSem");
Semaphore_construct(&read_main_sem_struct, 0, &sem_params);
read_main_sem = Semaphore_handle(&read_main_sem_struct);

sem_params.__iprms.name = "ReadLabelSem";
sem_params.__iprms.__size = sizeof("ReadLableSem");
Semaphore_construct(&read_label_sem_struct, 0, &sem_params);
read_label_sem = Semaphore_handle(&read_label_sem_struct);

sem_params.__iprms.name = "ReadUISem";
sem_params.__iprms.__size = sizeof("ReadUISem");
Semaphore_construct(&read_ui_sem_struct, 0, &sem_params);
read_ui_sem = Semaphore_handle(&read_ui_sem_struct);


/* Mutexes not yet implemented */

GateMutex_Struct dev_ser_tx_struct;
GateMutex_Struct dev_ser_rx_struct;


GateMutex_Handle dev_serial_tx_mutex;
GateMutex_Handle dev_serial_rx_mutex;


/* Configure params for dev <-> dev tx guard */
mutex_params.instance->name = "DEV_TX";
GateMutex_construct(&dev_ser_tx_struct, &mutex_params);
dev_serial_tx_mutex = GateMutex_handle(&dev_ser_tx_struct);

/* Configure params for dev <-> dev rx guard */
mutex_params.instance->name = "DEV_RX";
GateMutex_construct(&dev_ser_rx_struct, &mutex_params);
dev_serial_rx_mutex = GateMutex_handle(&dev_ser_rx_struct);



/*TASK: Test Back Channel Write
 * Strains backchannel uart by repetitively posting to it.
 */
void TestBackChannelWrite(UArg arg0, UArg arg1){
	uint8_t n;
	IArg key;
	while (1)
	{
		Semaphore_pend(bc_tx_slots, BIOS_WAIT_FOREVER);
		key = GateMutex_enter(bc_tx_q_mutex);
		n = (TX_BUF_SIZE - 1) - (Semaphore_getCount(bc_tx_slots));
		strcpy(bc_tx_q[n].message, "Hello, World!");
		GateMutex_leave(bc_tx_q_mutex, key);
		Task_sleep(100);
	}
}


/* Basic BackChannelWrite Test input */
strcpy (bc_tx_q[0].message, "Hello, World!");
bc_tx_q[0].is_open = TRUE;
bc_tx_q[0].priority = DEBUG_MSG;
bc_tx_q[0].length = sizeof("Hello, World!");




// OLD BACKCHANNEL WRITE TASK
void BackChannelWriteTask(UArg arg0, UArg arg1){
	uint8_t max_pri_i;
	int16_t i;
	int16_t j;
	IArg key_q;
	IArg key_hw;

	while (1){
		i = 0;
		j = 0;

		/* Only need to write if new messages available in queue */
		i = (int8_t) Semaphore_getCount(bc_tx_slots);
		while (Semaphore_getCount(bc_tx_slots) == TX_BUF_SIZE){
			Task_sleep(BC_LATENCY);
		}

		/* Enter gate to access queue */
		key_q = GateMutex_enter(bc_tx_q_mutex);

		/* Scan queue for highest priority message, favoring seniority
		 * and aging each element not selected
		 */
		max_pri_i = 0;
		for (i = 0 ; i < TX_BUF_SIZE ; i++){
			if ((!bc_tx_q[i].is_open) 								&&
				( bc_tx_q[i].priority > bc_tx_q[max_pri_i].priority)	&&
				(bc_tx_q[i].age > bc_tx_q[max_pri_i].age) )
			{
				max_pri_i = i;
			}
			else{
				bc_tx_q[i].age++;
			}
		}

		/* Print highest Priority Message, reset slot */
		key_hw = GateMutex_enter(bc_serial_tx_mutex);
		for (j = 0 ; (bc_tx_q[max_pri_i].message[j] != NULL) ; j++){
			USCI_A_UART_transmitData(USCI_A1_UART_BASE, bc_tx_q[max_pri_i].message[j]);
		}
		bc_tx_q[max_pri_i].is_open = TRUE;
		bc_tx_q[max_pri_i].length = 0;
		bc_tx_q[max_pri_i].priority = PRIORITY_COUNT;
		bc_tx_q[max_pri_i].age = 0;

		/* Alert other tasks of actions */
		Semaphore_post(bc_tx_slots);

		/* Return keys */
		GateMutex_leave(bc_serial_tx_mutex, key_hw);
		GateMutex_leave(bc_tx_q_mutex, key_q);
	}
}


#endif

/*
  FreeRTOS.org V6.0.4 - Copyright (C) 2003-2009 Richard Barry.

  This file is part of the FreeRTOS.org distribution.

  FreeRTOS.org is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License (version 2) as published
  by the Free Software Foundation and modified by the FreeRTOS exception.

  FreeRTOS.org is distributed in the hope that it will be useful,	but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along
  with FreeRTOS.org; if not, write to the Free Software Foundation, Inc., 59
  Temple Place, Suite 330, Boston, MA  02111-1307  USA.

  A special exception to the GPL is included to allow you to distribute a
  combined work that includes FreeRTOS.org without being obliged to provide
  the source code for any proprietary components.  See the licensing section
  of http://www.FreeRTOS.org for full details.


  ***************************************************************************
  *                                                                         *
  * Get the FreeRTOS eBook!  See http://www.FreeRTOS.org/Documentation      *
  *                                                                         *
  * This is a concise, step by step, 'hands on' guide that describes both   *
  * general multitasking concepts and FreeRTOS specifics. It presents and   *
  * explains numerous examples that are written using the FreeRTOS API.     *
  * Full source code for all the examples is provided in an accompanying    *
  * .zip file.                                                              *
  *                                                                         *
  ***************************************************************************

  1 tab == 4 spaces!

  Please ensure to read the configuration and relevant port sections of the
  online documentation.

  http://www.FreeRTOS.org - Documentation, latest information, license and
  contact details.

  http://www.SafeRTOS.com - A version that is certified for use in safety
  critical systems.

  http://www.OpenRTOS.com - Commercial support, development, porting,
  licensing and training services.
*/

/* System headers. */
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

#define INCLUDE_vTaskDelay 1

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static const short sUsingPreemption = configUSE_PREEMPTION;

void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );

typedef struct task_params {
    TaskHandle_t * _h_communication;
    int * _priority_communication_current;
    int * _priority_communication_low;
    int * _priority_communication_high;
} task_params;

//global var for easy access from tick ISR ---
int raise_priority_communication;
int lower_priority_communication;
unsigned long long ticks_total;

TaskHandle_t * h_communication;
TaskHandle_t * h_matrix;

eTaskState state_prev_comm;
eTaskState state_prev_matrix;

unsigned long long time_prev_comm;
unsigned long long time_prev_matrix;
unsigned long long time_diff_comm;
unsigned long long time_diff_matrix;
// ---

#define SIZE 10
#define ROW SIZE
#define COL SIZE
static void matrix_task()
{
    int i;
    double **a = (double **)pvPortMalloc(ROW * sizeof(double*));
    for (i = 0; i < ROW; i++) a[i] = (double *)pvPortMalloc(COL * sizeof(double));
    double **b = (double **)pvPortMalloc(ROW * sizeof(double*));
    for (i = 0; i < ROW; i++) b[i] = (double *)pvPortMalloc(COL * sizeof(double));
    double **c = (double **)pvPortMalloc(ROW * sizeof(double*));
    for (i = 0; i < ROW; i++) c[i] = (double *)pvPortMalloc(COL * sizeof(double));

    double sum = 0.0;
    int j, k, l;

    for (i = 0; i < SIZE; i++) {
	for (j = 0; j < SIZE; j++) {
	    a[i][j] = 1.5;
	    b[i][j] = 2.6;
	}
    }

    while (1) {
	
	taskENTER_CRITICAL();
	printf( "matrix mult...\n");
	fflush(stdout);
	taskEXIT_CRITICAL();
	long simulationdelay;
	for (simulationdelay = 0; simulationdelay<1000000000; simulationdelay++){
	    ;
	}

	for (i = 0; i < SIZE; i++) {
	    for (j = 0; j < SIZE; j++) {
		c[i][j] = 0.0;
	    }
	}

	for (i = 0; i < SIZE; i++) {
	    for (j = 0; j < SIZE; j++) {
		sum = 0.0;
		for (k = 0; k < SIZE; k++) {
		    for (l = 0; l<10; l++) {
			sum = sum + a[i][k] * b[k][j];
		    }
		}
		c[i][j] = sum;
	    }
	}
		    
	vTaskDelay(100);
    }
}

static void communication_task()
{
    while (1) {

	taskENTER_CRITICAL();
	printf("Sending data...\n");
	fflush(stdout);
	taskEXIT_CRITICAL();
	vTaskDelay(100);
	
	taskENTER_CRITICAL();
	printf("Data sent!\n");
	fflush(stdout);
	taskEXIT_CRITICAL();

	vTaskDelay(100);
    }
}

void vApplicationMallocFailedHook( void )
{
    vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    struct timespec xTimeToSleep, xTimeSlept;
    xTimeToSleep.tv_sec = 1;
    xTimeToSleep.tv_nsec = 0;
    nanosleep( &xTimeToSleep, &xTimeSlept );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    eTaskState state_comm;
    eTaskState state_matrix;
    UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    state_comm = eTaskGetState( *h_communication );
    state_matrix = eTaskGetState( *h_matrix );
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    //measure period of communication task
    if( state_comm == eReady && state_prev_comm != eReady ){
    	time_diff_comm = ( ticks_total - time_prev_comm ) * 2;
    	time_prev_comm = ticks_total;
	//check for priority switching conditions
    	if( time_diff_comm > ( 1000 * portTICK_RATE_MS) ){ //normalize to its period because the communication task has 2 calls to time delay/resheduling
    	    raise_priority_communication = 1;
    	    lower_priority_communication = 0;
    	}
    	else if( time_diff_comm < ( 200 * portTICK_RATE_MS ) ){
    	    raise_priority_communication = 0;
    	    lower_priority_communication = 1;
    	}
    }

    //measure period of matrix task
    if( state_matrix == eReady && state_prev_matrix != eReady ){
    	time_diff_matrix = ticks_total - time_prev_matrix;
    	time_prev_matrix = ticks_total;
    }
    
    state_prev_comm = state_comm;
    state_prev_matrix = state_matrix;

    ++ticks_total;
}
/*-----------------------------------------------------------*/

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
    taskENTER_CRITICAL();
    {
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
    }
    taskEXIT_CRITICAL();
    exit(-1);
}

static void priority_set_task( void * in )
{
    task_params * params = ( task_params * ) in;
    while (1) {
	if( raise_priority_communication &&
	    *params->_priority_communication_current != *params->_priority_communication_high ){
	    //raise priority
	    raise_priority_communication = 0;
	    *params->_priority_communication_current = *params->_priority_communication_high;
	    vTaskPrioritySet( *params->_h_communication, *params->_priority_communication_current );
	}else if( lower_priority_communication &&
		  *params->_priority_communication_current != *params->_priority_communication_low ){
	    //lower priority
	    lower_priority_communication = 0;
	    *params->_priority_communication_current= *params->_priority_communication_low;
	    vTaskPrioritySet( *params->_h_communication, *params->_priority_communication_current );
	}

	taskENTER_CRITICAL();
	printf( "communication task period: %llums\n", time_diff_comm );
	printf( "matrix task period: %llums\n", time_diff_matrix );
	fflush(stdout);
	taskEXIT_CRITICAL();
	
	vTaskDelay( 500 ); //arbitary delay for this task
    }
}

int main( void )
{
    //global var initialization
    ticks_total = 0;
    time_prev_comm = 0;
    time_prev_matrix = 0;

    raise_priority_communication = 0;
    lower_priority_communication = 0;
    
    int priority_matrix = 3;
    TaskHandle_t matrix_handle;
    if( pdPASS != xTaskCreate((pdTASK_CODE)matrix_task, "Matrix", 1000, NULL, priority_matrix, &matrix_handle) ){
	printf( "could not create task\n\r");
	return 1;
    }

    h_matrix = & matrix_handle;
	
    int priority_communication_initial = 1;
    int priority_communication_high = 4;
    int priority_communication_low = 2;
    TaskHandle_t communication_handle;
    if( pdPASS != xTaskCreate((pdTASK_CODE)communication_task, "Communication", configMINIMAL_STACK_SIZE, NULL, priority_communication_initial, &communication_handle)){
	printf( "could not create task\n\r");
	return 1;
    }

    h_communication = & communication_handle;
    
    task_params params = { &communication_handle, &priority_communication_initial, &priority_communication_low, &priority_communication_high };
    TaskHandle_t prioritysettask_handle;
    if( pdPASS != xTaskCreate((pdTASK_CODE)priority_set_task, "Priority Set Task", configMINIMAL_STACK_SIZE, &params, 5, &prioritysettask_handle)){ //set its priorty to an arbitarily high number since it needs to act as a feedback loop controller
    	printf( "could not create task\n\r");
    	return 1;
    }

    vTaskStartScheduler();

    //should not get here

    return 1;
}

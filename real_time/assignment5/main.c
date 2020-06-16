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
#include "timers.h"

static const short sUsingPreemption = configUSE_PREEMPTION;

void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );
void vTimerCallback(TimerHandle_t pxTimer);

#define SIZE 10
#define ROW SIZE
#define COL SIZE
static void matrix_task( void * data )
{
    assert( data );
    QueueHandle_t * queue = ( QueueHandle_t * ) data;

    int i;
    double **a = (double **)pvPortMalloc(ROW * sizeof(double*));
    for (i = 0; i < ROW; i++) a[i] = (double *)pvPortMalloc(COL * sizeof(double));
    double **b = (double **)pvPortMalloc(ROW * sizeof(double*));
    for (i = 0; i < ROW; i++) b[i] = (double *)pvPortMalloc(COL * sizeof(double));
    double **c = (double **)pvPortMalloc(ROW * sizeof(double*));
    for (i = 0; i < ROW; i++) c[i] = (double *)pvPortMalloc(COL * sizeof(double));

    double sum = 0.0;
    int j, k;

    for (i = 0; i < SIZE; i++) {
	for (j = 0; j < SIZE; j++) {
	    a[i][j] = 1.5;
	    b[i][j] = 2.6;
	}
    }
    while (1) {
	volatile long simulationdelay; //avoid loop optimization
	for (simulationdelay = 0; simulationdelay<1000000000; simulationdelay++);
	for (i = 0; i < SIZE; i++) {
	    for (j = 0; j < SIZE; j++) {
		c[i][j] = 0.0;
	    }
	}
	for (i = 0; i < ROW; i++) {
	    for (j = 0; j < COL; j++) {
		sum = 0.0;
		for (k = 0; k < SIZE; k++) {
		    /* for (l = 0; l<10; l++) { */ //don't really need this extra loop in mat mult
		    sum = sum + a[i][k] * b[k][j];
		    /* } */
		}
		c[i][j] = sum;
	    }
	}
	//copy data to a contiguous buffer for sending
	double buf[ROW][COL];
	for (i = 0; i < SIZE; i++) {
	    for (j = 0; j < SIZE; j++) {
		buf[i][j] = c[i][j];
	    }
	}
	xQueueSend( *queue, &buf[0][0], 10 ); //try block for a small amount of time
	vTaskDelay(100); //reschedule
    }
}

static void reader_task( void * data )
{
    assert( data );
    QueueHandle_t * queue = ( QueueHandle_t * ) data;    
    double buf[ROW][COL];
    while (1) {
	if( pdTRUE == xQueueReceive( *queue, buf, 0 ) ){ //non-blocking return if fail
	    printf( "reader task received:\n" );
	    //read matrix elements
	    for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
		    printf( "%.f ", buf[i][j] );
		}
		printf( "\n" );
	    }
	    fflush(stdout);
	}
	vTaskDelay(2); //arbitrary small time relative to matrix task
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

int main( void )
{
    QueueHandle_t myqueue = xQueueCreate( 20, ROW * COL * sizeof(double) ); //init queue for storing a reasonable number of matrices
    if( myqueue == NULL ){
	printf( "could not create queue\n\r");
	return 1;
    }

    int priority_matrix = 3;
    TaskHandle_t matrix_handle;
    if( pdPASS != xTaskCreate((pdTASK_CODE)matrix_task, "Matrix", 1000, &myqueue, priority_matrix, &matrix_handle) ){
	printf( "could not create matrix task\n\r");
	return 1;
    }

    int priority_reader = 3; //set the priority to be same as matrix task so they are cooperative
    TaskHandle_t reader_handle;
    if( pdPASS != xTaskCreate((pdTASK_CODE)reader_task, "Reader", 1000, &myqueue, priority_reader, &reader_handle) ){
	printf( "could not create reader task\n\r");
	return 1;
    }
    
    vTaskStartScheduler();

    //should not get here
    vQueueDelete( myqueue );
    
    return 1;
}

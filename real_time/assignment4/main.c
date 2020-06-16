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

TaskHandle_t aperiodic_handle;

unsigned long long my_tick;
unsigned long long aperiodic_release_time;
unsigned long long aperiodic_response_time;

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
	/*
	 * In an embedded systems, matrix multiplication would block the CPU for a long time
	 * but since this is a PC simulator we must add one additional dummy delay.
	 */
	printf("matrix task started!\n");
	fflush(stdout);
	long long simulationdelay;
	volatile int dummy = 0;
	for (simulationdelay = 0; simulationdelay<1000000000; simulationdelay++)
	{
	    ++dummy;
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
    ++my_tick;
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

static void aperiodic_task()
{
    printf("Aperiodic task started!\n");
    fflush(stdout);
    unsigned long long i;
    volatile int f = 0; //added to suppress compiler optimization of the loop below
    for (i = 0; i<1000000000; i++) //Dummy workload
    {
	f++;
    }
    printf("Aperiodic task done!\n");
    
    aperiodic_response_time = my_tick - aperiodic_release_time; //added for measurement
    printf("Aperiodic task response time: %llu\n", aperiodic_response_time );

    fflush(stdout);
    vTaskDelete(aperiodic_handle);
}

/* A variable to hold a count of the number of times the timer expires. */
long lExpireCounters = 0;
void vTimerCallback(TimerHandle_t pxTimer)
{
    printf("Timer callback!\n");

    aperiodic_release_time = my_tick; //added for measurement purpose
    
    xTaskCreate((pdTASK_CODE)aperiodic_task, (const char * const)"Aperiodic", configMINIMAL_STACK_SIZE, NULL, 4, &aperiodic_handle); //this aperiodic task has higher priorty to let it execute right away when it is released
    
    /* long lArrayIndex; */
    const long xMaxExpiryCountBeforeStopping = 10;
    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT(pxTimer);
    /* Increment the number of times that pxTimer has expired. */
    lExpireCounters += 1;
    /* If the timer has expired 10 times then stop it from running. */
    if (lExpireCounters == xMaxExpiryCountBeforeStopping) {
	/* Do not use a block time if calling a timer API function from a
	   timer callback function, as doing so could cause a deadlock! */
	xTimerStop(pxTimer, 0);
    }
}

int main( void )
{    
    int priority_matrix = 3;
    TaskHandle_t matrix_handle;
    if( pdPASS != xTaskCreate((pdTASK_CODE)matrix_task, "Matrix", 1000, NULL, priority_matrix, &matrix_handle) ){
	printf( "could not create task\n\r");
	return 1;
    }

    TimerHandle_t pxTimer = xTimerCreate( "tick tok tick tok...",
					  pdMS_TO_TICKS( 5000 ),
					  pdTRUE,
					  NULL,
					  vTimerCallback );

    if( pxTimer == NULL ){
	printf( "could not create timer\n\r");
	return 1;	
    }
    printf( "created timer\n\r");

    if( pdPASS != xTimerStart( pxTimer, 0 ) ){
	printf( "could not start timer\n\r");
	return 1;	
    }
    printf( "started timer\n\r");

    vTaskStartScheduler();

    //should not get here

    return 1;
}

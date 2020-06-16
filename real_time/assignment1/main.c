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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "partest.h"

#define mainTASK1_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define mainTASK2_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainGENERIC_QUEUE_PRIORITY	( tskIDLE_PRIORITY ) // default 0

#define INCLUDE_vTaskDelay 1

/* Constant definition used to turn on/off the pre-emptive scheduler. */
static const short sUsingPreemption = configUSE_PREEMPTION;

void vMainQueueSendPassed( void ){}

void vApplicationIdleHook( void )
{
//taken from sample program
#ifdef __GCC_POSIX__
    struct timespec xTimeToSleep, xTimeSlept;
    xTimeToSleep.tv_sec = 1;
    xTimeToSleep.tv_nsec = 0;
    nanosleep( &xTimeToSleep, &xTimeSlept );
#endif
}

typedef struct task_params {
    long period_ms;
    char const * msg;
} task_params;

static void vEcho( void *pvParameters )
{
    task_params * params = ( task_params * ) pvParameters;
    portTickType period = params->period_ms / portTICK_RATE_MS;
    char * msg = params->msg;
    for( ;; )
    {
	vTaskSuspendAll();
	printf( "%s", msg );
	fflush( stdout );
	xTaskResumeAll();	
	vTaskDelay( period );
    }
}

int main( void )
{	
    unsigned short task1_stack_size = 1000;
    task_params task1_params = { 100, "This is task1\n\r" };
	
    if( pdPASS != xTaskCreate( vEcho, "Task1", task1_stack_size, &task1_params, mainTASK1_TASK_PRIORITY, NULL ) ){
	printf( "could not create task\n\r");
	return 1;
    }

    unsigned short task2_stack_size = 100;
    task_params task2_params = { 500, "This is task2\n\r" };
    
    if( pdPASS != xTaskCreate( vEcho, "Task2", task2_stack_size, &task2_params, mainTASK2_TASK_PRIORITY, NULL ) ){
	printf( "could not create task\n\r");
	return 1;
    }
	
    vTaskStartScheduler();

    //should not get here

    return 1;
}

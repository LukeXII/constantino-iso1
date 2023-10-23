/*
 * osSemaphore.c
 *
 *  Created on: Oct 3, 2023
 *      Author: Lucas
 */

#include "osSemaphore.h"

void osSemaphoreInit(osSemaphoreObject * semaphore, const uint32_t maxCount, const uint32_t count)
{
	osTaskObject task;

	if( (semaphore != NULL) && (maxCount >= count) && (maxCount >= 0) )
	{
		semaphore->count = count;
		semaphore->maxCount = maxCount;

		osQueueInit(&(semaphore->waitingTask), sizeof(&task));
	}
}

void osSemaphoreGive(osSemaphoreObject * semaphore)
{
	osTaskObject * dummyTask = NULL;

	if(semaphore != NULL)
	{
		if((semaphore->waitingTask).qLength != 0)
		{
			__pullItem(&(semaphore->waitingTask), &dummyTask);
			osUnblockTask(dummyTask);
		}
		else
		{
			if(semaphore->count > 0)
				semaphore->count--;
		}
	}
}

bool osSemaphoreTake(osSemaphoreObject * semaphore)
{
	osTaskObject * dummyTask;
	bool semaphoreTaken = false;

	if(semaphore != NULL)
	{
		if(semaphore->count < semaphore->maxCount)
		{
			semaphore->count++;
			semaphoreTaken = true;
		}
		else
		{
			dummyTask = osGetCurrentTask();
			__pushItem(&(semaphore->waitingTask), &dummyTask);
			osBlockTask(dummyTask);
		}
	}

	return semaphoreTaken;
}

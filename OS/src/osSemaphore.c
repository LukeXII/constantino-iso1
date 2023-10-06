/*
 * osSemaphore.c
 *
 *  Created on: Oct 3, 2023
 *      Author: Lucas
 */

#include "osSemaphore.h"

void osSemaphoreInit(osSemaphoreObject * semaphore, const uint32_t maxCount, const uint32_t count)
{
	if( (semaphore != NULL) && (maxCount >= count) && (maxCount > 0) )
	{
		semaphore->count = count;
		semaphore->maxCount = maxCount;
	}
}

bool osSemaphoreTake(osSemaphoreObject * semaphore)
{
	bool ret = false;

	if(semaphore != NULL)
	{
		if(semaphore->count > 0)
		{
			semaphore->count--;
			ret = true;
		}
		else
		{
			semaphore->waitingTask = osGetCurrentTask();
			osBlockTask(semaphore->waitingTask);
		}
	}

	return ret;
}

void osSemaphoreGive(osSemaphoreObject * semaphore)
{
	if(semaphore != NULL)
	{
		if(semaphore->count < semaphore->maxCount)
			semaphore->count++;

		if(semaphore->waitingTask != NULL)
			osUnblockTask(semaphore->waitingTask);
	}
}

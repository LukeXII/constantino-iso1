/*
 * osQueue.c
 *
 *  Created on: Oct 3, 2023
 *      Author: Lucas
 */

#include "osQueue.h"
#include <stdlib.h>

bool osQueueInit(osQueueObject * queue, const uint32_t dataSize)
{
	bool queueCreated = false;

	if( (queue != NULL) && (dataSize != 0) )
	{
		queue->qLength = 0;
		queue->qWaitingTask = NULL;
		queue->qItemSizeBytes = dataSize;
		queue->qBuffer = malloc(dataSize * MAX_SIZE_QUEUE);
	    queueCreated = true;
	}

	return queueCreated;
}

bool osQueueSend(osQueueObject * queue, const void * data, const uint32_t timeout)
{
	bool itemPushed = false;

	if( (queue != NULL) && (data != NULL) )
	{
		// Si la cola esta llena, espera que alguna otra tarea saque un item entrando en delay
		if(queue->qLength == MAX_SIZE_QUEUE)
		{
			queue->qWaitingTask = osGetCurrentTask();

			if(timeout == OS_MAX_DELAY)
				osBlockTask(queue->qWaitingTask);
			else
				osDelay(timeout);

			if(queue->qLength < MAX_SIZE_QUEUE)
			{
				queue->qWaitingTask = NULL;
				__pushItem(queue, data);
				itemPushed = true;
			}
		}
		else
		{
			__pushItem(queue, data);
			itemPushed = true;
			// Si habia una tarea esperando para pullear de una cola vacia, cancela el timeout
			if( queue->qWaitingTask != NULL )
				osUnblockTask(queue->qWaitingTask);
		}
	}

	return itemPushed;
}

bool osQueueReceive(osQueueObject * queue, void * buffer, const uint32_t timeout)
{
	bool itemPulled = false;

	if( (queue != NULL) && (buffer != NULL) )
	{
		// Si la cola esta vacia, espera que alguna otra tarea pushee un item entrando en delay
		if(queue->qLength == 0)
		{
			queue->qWaitingTask = osGetCurrentTask();

			if(timeout == OS_MAX_DELAY)
				osBlockTask(queue->qWaitingTask);
			else
				osDelay(timeout);

			if(queue->qLength > 0)
			{
				queue->qWaitingTask = NULL;
				__pullItem(queue, buffer);
				itemPulled = true;
			}
		}
		else
		{
			__pullItem(queue, buffer);
			itemPulled = true;
			// Si habia una tarea esperando para pushear a una cola llena, cancela el timeout
			if( queue->qWaitingTask != NULL )
				osUnblockTask(queue->qWaitingTask);
		}
	}

	return itemPulled;
}

void __pushItem(osQueueObject * queue, const void * item)
{
	memcpy(queue->qBuffer + (queue->qLength * queue->qItemSizeBytes), item, queue->qItemSizeBytes);
	queue->qLength++;
}

void __pullItem(osQueueObject * queue, const void * item)
{
	uint8_t i;

	queue->qLength--;
	memcpy(item, queue->qBuffer, queue->qItemSizeBytes);

	for(i = 0; i < queue->qLength ;i++)
		memcpy(queue->qBuffer + (i * queue->qItemSizeBytes), queue->qBuffer + ((i+1) * queue->qItemSizeBytes), queue->qItemSizeBytes);

}

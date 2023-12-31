/*
 * osQueue.h
 *
 *  Created on: Oct 3, 2023
 *      Author: Lucas
 */

#ifndef INC_OSQUEUE_H_
#define INC_OSQUEUE_H_

#include <osKernel.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE_QUEUE  5	     	// Maximum buffer size of the queue
#define OS_MAX_DELAY    0xFFFFFFFF  // Macro where the queue is locked forever. It ignores the timeout variable in the implementation

/**
 * @brief Data structure queue.
 */
typedef struct
{
    osTaskObject * qWaitingTask;	// Task waiting for this queue
	uint8_t qLength;
    uint8_t qItemSizeBytes;
    uint8_t * qBuffer;
} osQueueObject;

/**
 * @brief Initialize the queue.
 *
 * @param[in, out]  queue       Queue object
 * @param[in]       dataSize    Data size of the queue
 *
 * @return Returns true if was success in otherwise false
 */
bool osQueueInit(osQueueObject * queue, const uint32_t dataSize);

/**
 * @brief Send data to the queue.
 *
 * @param[in, out]  queue   Queue object
 * @param[in, out]  data    Data sent to the queue
 * @param[in]       timeout Number of ticks to wait before unblocking the task
 *
 * @return Returns true if it could be put in the queue
 * in otherwise false.
 */
bool osQueueSend(osQueueObject * queue, const void * data, const uint32_t timeout);

/**
 * @brief Receive data to the queue.
 *
 * @param[in, out]  queue   Queue object
 * @param[in, out]  buffer  Buffer to  save the data read from the queue
 * @param[in]       timeout Number of ticks to wait before unblocking the task
 *
 * @return Returns true if it was possible to take it out in the queue
 * in otherwise false.
 */
bool osQueueReceive(osQueueObject * queue, void * buffer, const uint32_t timeout);

void __pushItem(osQueueObject * queue, const void * item);
void __pullItem(osQueueObject * queue, const void * item);

#endif /* INC_OSQUEUE_H_ */

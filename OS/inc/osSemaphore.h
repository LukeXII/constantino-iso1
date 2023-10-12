/*
 * osSemaphore.h
 *
 *  Created on: Oct 3, 2023
 *      Author: Lucas
 */

#ifndef INC_OSSEMAPHORE_H_
#define INC_OSSEMAPHORE_H_

#include <osKernel.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "osQueue.h"

typedef struct
{
	uint8_t count;
	uint8_t maxCount;
	osQueueObject waitingTask;
} osSemaphoreObject;

/**
 * @brief Initializes semaphore binary or not.
 *
 * @param[in,out]   semaphore   Semaphore handler.
 * @param[in]       maxCount    Maximum count value that can be reached.
 * @param[in]       count       The count value assigned to the semaphore when it is created.
 */
void osSemaphoreInit(osSemaphoreObject * semaphore, const uint32_t maxCount, const uint32_t count);

/**
 * @brief Take semaphore.
 *
 * @param[in,out]   semaphore   Semaphore handler.
 *
 * @return Returns true if the semaphore could be taken.
 */
bool osSemaphoreTake(osSemaphoreObject * semaphore);

/**
 * @brief Give semaphore.
 *
 * @param[in,out]   semaphore   Semaphore handler.
 */
void osSemaphoreGive(osSemaphoreObject * semaphore);

#endif /* INC_OSSEMAPHORE_H_ */

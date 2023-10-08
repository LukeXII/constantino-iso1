/*
 * stm32f429.h
 *
 *  Created on: Oct 7, 2023
 *      Author: Lucas
 */

#ifndef INC_PORT_STM32F429_H_
#define INC_PORT_STM32F429_H_

/**
 * @note THe macros, types definition, variables and method of this file must be used internaly in the OS.
 */
#include "stm32f429xx.h"
#include "osCore.h"

#define IRQ_NUMBER      91                  /* Number of interrupts supported by the MCU */

typedef IRQn_Type   osIRQnType;             /* STM32F4XX interrupt number definition */
typedef void (*IRQHandler)(void* data);     /* Protorype of function */

// Data type of IRQ vector.
typedef struct
{
	IRQHandler  handler;    // Function served by the IRQ.
	void*       data;		// Data that is passed to the function that services the IRQ.
}osIRQVector;


extern osIRQVector irqVector[IRQ_NUMBER];

/**
 * @brief Function used to execute the interrupt logic and
 * clear the interrupt trigger bit.
 *
 * @param[in]	irqType		IRQ number on the interrupts vector.
 */
void osIRQHandler(osIRQnType irqType);

#endif /* INC_PORT_STM32F429_H_ */

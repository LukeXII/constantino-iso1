/*
 * osCore.h
 *
 *  Created on: Sep 15, 2023
 *      Author: Lucas
 */

#ifndef INC_OSCORE_H_
#define INC_OSCORE_H_

/* ************************************************************************* */
/*                                  Includes                                 */
/* ************************************************************************* */
#include <stdint.h>
#include <stddef.h>
#include "cmsis_gcc.h"
#include "stm32f429xx.h"

/* ************************************************************************* */
/*                           Preprocessor Constants                          */
/* ************************************************************************* */
#define OS_MAX_TASKS			8U
#define OS_MAX_STACK_SIZE 		256U
#define OS_STACK_FRAME_SIZE 	17U
#define SYSTICK_PERIOD_MS		1U          					// Systick period time ms
#define OS_MAX_TASK_NAME_CHAR	10

/* Registers positions on Stack Frame */
#define XPSR_VALUE              1 << 24     // xPSR.T = 1
#define EXEC_RETURN_VALUE       0xFFFFFFF9  // EXEC_RETURN value. Return to thread mode with MSP
#define XPSR_REG_POSITION       1
#define PC_REG_POSITION         2
#define LR_REG_POSITION         3
#define R12_REG_POSITION        4
#define R3_REG_POSITION         5
#define R2_REG_POSITION         6
#define R1_REG_POSITION         7
#define R0_REG_POSITION         8
#define LR_PREV_VALUE_POSITION  9
#define R4_REG_POSITION         10
#define R5_REG_POSITION         11
#define R6_REG_POSITION         12
#define R7_REG_POSITION         13
#define R8_REG_POSITION         14
#define R9_REG_POSITION         15
#define R10_REG_POSITION        16
#define R11_REG_POSITION        17

/* ************************************************************************* */
/*                                   Types                                   */
/* ************************************************************************* */

typedef enum
{
    OS_OK,
	OS_ERR_MAX_TASKS,
	OS_ERR_INVALID_PTR
} osError_t;

// OS status
typedef enum
{
    OS_STATUS_RUNNING,
    OS_STATUS_RESET
} osStatus_t;

// Task status
typedef enum
{
    OS_TASK_READY,
    OS_TASK_RUNNING,
    OS_TASK_BLOCKED,
    OS_TASK_SUSPENDED
} osTaskStatus_t;

// Task control structure
typedef struct
{
    uint32_t taskStack[OS_MAX_STACK_SIZE/4];    	// Memory Size
    uint32_t taskStackPointer;                   	// Store the task SP
    void * ptrTaskEntryPoint;                   	// Task entry point address
    osTaskStatus_t taskExecStatus;   				// Task current execution status
    uint8_t taskID;                             	// Task ID
    char * ptrTaskName[OS_MAX_TASK_NAME_CHAR];  	// Task name (for debug purposes)
} osTaskObject_t;

/* ************************************************************************* */
/*                              Public Functions                             */
/* ************************************************************************* */

osError_t osTaskCreate(osTaskObject_t * ptrTaskHandler, void * ptrTaskCallback);

void osStart(void);

#endif /* INC_OSCORE_H_ */

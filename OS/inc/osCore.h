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
#define WEAK					__attribute__((weak))

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

// Task priorities
typedef enum
{
    TASK_PRIORITY_0,
	TASK_PRIORITY_1,
	TASK_PRIORITY_2,
	TASK_PRIORITY_3
} osTaskPriority_t;

// Task control structure
typedef struct
{
    uint32_t taskStack[OS_MAX_STACK_SIZE/4];    	// Memory Size
    uint32_t taskStackPointer;                   	// Store the task SP
    void * ptrTaskEntryPoint;                   	// Task entry point address
    osTaskStatus_t taskExecStatus;   				// Task current execution status
    uint8_t taskID;                             	// Task ID
    char * ptrTaskName[OS_MAX_TASK_NAME_CHAR];  	// Task name (for debug purposes)
    osTaskPriority_t taskPriority;					// Task assigned priority
} osTaskObject_t;

/* ************************************************************************* */
/*                              Public Functions                             */
/* ************************************************************************* */

osError_t osTaskCreate(osTaskObject_t * ptrTaskHandler, osTaskPriority_t taskPriority, void * ptrTaskCallback);
void osStart(void);
/**
 * @brief Execute a delay for the current task.
 *
 * @param[in]   tick Number ticks delayed.
 */
void osDelay(const uint32_t tick);
/**
 * @brief Function used as default when some task return for a problem.
 */
void osReturnTaskHook(void);
/**
 * @brief Function used if user would like to do something on systick hander interrupt.
 * It has a default implementation that do anything.
 *
 * @warning The function used to perform operations on each Systick in the system. It
 * be as short as possible because it is called in the Systick interrupt handler.
 *
 * @warning The function shouldn't call an OS API in any case because a new scheduler
 * could occur.
 */
void osSysTickHook(void);

/**
 * @brief Function used when happen error on OS
 *
 * @param[in]   caller  Function pointer where error happened.
 */
void osErrorHook(void* caller);

/**
 * @brief Idle task of the operation system.
 */
void osIdleTask(void);

#endif /* INC_OSCORE_H_ */

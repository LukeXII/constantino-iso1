/*
 * osCore.c
 *
 *  Created on: Sep 15, 2023
 *      Author: Lucas
 */

/* ************************************************************************* */
/*                                  Includes                                 */
/* ************************************************************************* */
#include "osCore.h"

typedef struct{
	osStatus_t execStatus;                			// OS execution status (Reset, Running, IRQ)
    uint32_t osScheduleExec;                    	// Execution flag
    osTaskObject_t * ptrCurrTask;					// Current task executing
    osTaskObject_t * ptrNextTask;        			// Next task to be executed
    osTaskObject_t * ptrTaskList[OS_MAX_TASKS];   	// List of tasks
    uint8_t tasksCounter;
} osCoreCtrl_t;

/* ================== Private variables declaration ================= */
static osCoreCtrl_t osCore;

/* ================== Private functions declaration ================= */

static uint32_t getNextContext(uint32_t currentStaskPointer);
static void scheduler(void);

/* ================ Public functions implementation ================ */

osError_t osTaskCreate(osTaskObject_t * ptrTaskHandler, void * ptrTaskCallback)
{

    // Check that arguments are not NULL
    if ( (ptrTaskHandler == NULL) || (ptrTaskCallback == NULL) )
    {
    	return OS_ERR_INVALID_PTR;
    }

    // Check if there's available tasks to allocate
    if (osCore.tasksCounter >= OS_MAX_TASKS)
    {
        return OS_ERR_MAX_TASKS;
    }

    /*
        Arrange the STACK Frame for the first time:
        1) Set bit 24 of xPSR to make sure we are executing THUMB instructions
        2) PC must contain the task entry point (ptrTaskCallback)
        3) Set the link register to EXEC_RETURN_VALUE to trigger
    */
    ptrTaskHandler->taskStack[OS_MAX_STACK_SIZE/4 - XPSR_REG_POSITION]      = XPSR_VALUE;
    ptrTaskHandler->taskStack[OS_MAX_STACK_SIZE/4 - PC_REG_POSITION]        = (uint32_t)ptrTaskCallback;
    ptrTaskHandler->taskStack[OS_MAX_STACK_SIZE/4 - LR_PREV_VALUE_POSITION] = EXEC_RETURN_VALUE;


    ptrTaskHandler->taskStackPointer = (uint32_t)(ptrTaskHandler->taskStack + OS_MAX_STACK_SIZE/4 - OS_STACK_FRAME_SIZE);
    ptrTaskHandler->ptrTaskEntryPoint = ptrTaskCallback;
    ptrTaskHandler->taskExecStatus = OS_TASK_READY;
    ptrTaskHandler->taskID = osCore.tasksCounter;

    // Fill controls OS structure
    osCore.ptrTaskList[osCore.tasksCounter] = ptrTaskHandler;
    osCore.tasksCounter++;

    if (osCore.tasksCounter < OS_MAX_TASKS)
	{
    	osCore.ptrTaskList[osCore.tasksCounter] = NULL;
	}

    return OS_OK;
}


void osStart(void)
{
	// Disable Systick and PendSV interrupts
    NVIC_DisableIRQ(SysTick_IRQn);
    NVIC_DisableIRQ(PendSV_IRQn);

    osCore.execStatus = OS_STATUS_RESET;		// Set the system to RESET for the first time
    osCore.ptrCurrTask = NULL;      			// Set the current task to NULL the first time
    osCore.ptrNextTask = NULL;      			// Set the next task to NULL the first time

    // Is mandatory to set the PendSV priority as lowest as possible */
    NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);

    // Activate and configure the Systick exception
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / (1000U * SYSTICK_PERIOD_MS));

    // Enable Systick and PendSV interrupts
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);
}
/* ================ Private functions implementation ================ */

static uint32_t getNextContext(uint32_t currentStackPointer)
{
    if (osCore.execStatus != OS_STATUS_RUNNING)
    {
    	osCore.ptrCurrTask->taskExecStatus = OS_TASK_RUNNING;
    	osCore.execStatus = OS_STATUS_RUNNING;
    }
    else
    {
        // Storage last stack pointer used on current task and change state to ready.
        osCore.ptrCurrTask->taskStackPointer = currentStackPointer;
        osCore.ptrCurrTask->taskExecStatus = OS_TASK_READY;

        // Switch address memory points on current task for next task and change state of task
        osCore.ptrCurrTask = osCore.ptrNextTask;
        osCore.ptrCurrTask->taskExecStatus = OS_TASK_RUNNING;
    }

    return osCore.ptrCurrTask->taskStackPointer;
}

static void scheduler(void)
{
	uint8_t index = 0;

    // Check if this is the first scheduler execution
    if (osCore.execStatus != OS_STATUS_RUNNING)
    {
    	osCore.ptrCurrTask = osCore.ptrTaskList[0];				// If the OS wasn't running load the first task to be run
    }
    else
    {
    	index = osCore.ptrCurrTask->taskID + 1;					// Computes next task id to be run

    	osCore.ptrNextTask = osCore.ptrTaskList[index];			// Load next task to be run

    	// Check if this is the last task to be run. If so, reset index for next scheduler execution
        if ( (index >= OS_MAX_TASKS) || (osCore.ptrNextTask == NULL) )
            index = 0;
    }
}

/* ========== Processor Interruption and Exception Handlers ========= */

void SysTick_Handler(void)
{
    scheduler();

    /*
     * Set up bit corresponding exception PendSV
     */
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;

    /*
     * Instruction Synchronization Barrier; flushes the pipeline and ensures that
     * all previous instructions are completed before executing new instructions
     */
    __ISB();

    /*
     * Data Synchronization Barrier; ensures that all memory accesses are
     * completed before next instruction is executed
     */
    __DSB();
}

__attribute__ ((naked)) void PendSV_Handler(void)
{
    /**
     * Cuando se ingresa al handler de PendSV lo primero que se ejecuta es un push para
	 * guardar los registros R4-R11 y el valor de LR, que en este punto es EXEC_RETURN
	 * El push se hace al reves de como se escribe en la instruccion, por lo que LR
	 * se guarda en la posicion 9 (luego del stack frame). Como la funcion getNextContext
	 * se llama con un branch con link, el valor del LR es modificado guardando la direccion
	 * de retorno una vez se complete la ejecucion de la funcion
	 * El pasaje de argumentos a getContextoSiguiente se hace como especifica el AAPCS siendo
	 * el unico argumento pasado por RO, y el valor de retorno tambien se almacena en R0
	 *
	 * NOTA: El primer ingreso a este handler (luego del reset) implica que el push se hace sobre el
	 * stack inicial, ese stack se pierde porque no hay seguimiento del MSP en el primer ingreso
     */
    __ASM volatile ("push {r4-r11, lr}");
    __ASM volatile ("mrs r0, msp");
    __ASM volatile ("bl %0" :: "i"(getNextContext));
    __ASM volatile ("msr msp, r0");
    __ASM volatile ("pop {r4-r11, lr}");    //Recuperados todos los valores de registros

    /* Se hace un branch indirect con el valor de LR que es nuevamente EXEC_RETURN */
    __ASM volatile ("bx lr");
}

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
    uint8_t taskPriorityTable[4][OS_MAX_TASKS];
} osCoreCtrl_t;

/* ================== Private variables declaration ================= */
static osCoreCtrl_t osCore;
static osTaskObject_t idleTask;

/* ================== Private functions declaration ================= */

static uint32_t getNextContext(uint32_t currentStaskPointer);
static void scheduler(void);

/* ================ Public functions implementation ================ */

osError_t osTaskCreate(osTaskObject_t * ptrTaskHandler, osTaskPriority_t taskPriority, void * ptrTaskCallback)
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
    osCore.tasksCounter++;
    ptrTaskHandler->taskID = osCore.tasksCounter;
    ptrTaskHandler->taskPriority = taskPriority;

    // Fill controls OS structure
    osCore.ptrTaskList[osCore.tasksCounter - 1] = ptrTaskHandler;

    if (osCore.tasksCounter < OS_MAX_TASKS)
	{
    	osCore.ptrTaskList[osCore.tasksCounter] = NULL;
	}

    return OS_OK;
}


void osStart(void)
{
	uint8_t i, row, col;

	// Disable Systick and PendSV interrupts
    NVIC_DisableIRQ(SysTick_IRQn);
    NVIC_DisableIRQ(PendSV_IRQn);

    osCore.execStatus = OS_STATUS_RESET;		// Set the system to RESET for the first time
    osCore.ptrCurrTask = NULL;      			// Set the current task to NULL the first time
    osCore.ptrNextTask = NULL;      			// Set the next task to NULL the first time

    osTaskCreate(&idleTask, TASK_PRIORITY_3, osIdleTask);

    for(i = 0; i < osCore.tasksCounter;i++)
    {
    	switch(osCore.ptrTaskList[i]->taskPriority)
    	{
    	case TASK_PRIORITY_0:

    		row = 0;
    		break;

    	case TASK_PRIORITY_1:

    		row = 1;
    		break;

    	case TASK_PRIORITY_2:

    		row = 2;
    		break;

    	case TASK_PRIORITY_3:

    		row = 3;
    		break;
    	}

    	col = 0;
    	while(osCore.taskPriorityTable[row][col] != 0)
    		col++;

    	osCore.taskPriorityTable[row][col] = osCore.ptrTaskList[i]->taskID;

    }

    // Is mandatory to set the PendSV priority as lowest as possible */
    NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);

    // Activate and configure the Systick exception
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / (1000U * SYSTICK_PERIOD_MS));

    // Enable Systick and PendSV interrupts
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);
}

void osDelay(const uint32_t tick)
{
    //(void)tick;
}

WEAK void osReturnTaskHook(void)
{
    while(1)
    {
        __WFI();
    }
}

WEAK void osSysTickHook(void)
{
    __ASM volatile ("nop");
}

WEAK void osErrorHook(void* caller)
{
    while(1)
    {
    }
}

WEAK void osIdleTask(void)
{
    while(1)
    {
    	__WFI();
    }
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
        // Storage last stack pointer used on current task and change state to ready
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
	uint8_t row, col, id;

    // Check if this is the first scheduler execution
    if (osCore.execStatus != OS_STATUS_RUNNING)
    {
    	osCore.ptrCurrTask = osCore.ptrTaskList[osCore.tasksCounter - 1];				// If the OS wasn't running load the first task to be run
    	osCore.ptrCurrTask->taskExecStatus = OS_TASK_RUNNING;
    }

	row = 0;
	do
	{
		col = 0;
		do
		{
			id = osCore.taskPriorityTable[row][col];
			col++;
		}
		while( (osCore.ptrTaskList[id - 1]->taskExecStatus == OS_TASK_BLOCKED) &&
				(osCore.taskPriorityTable[row][col] != 0) );
		row++;
	}
	while( osCore.ptrTaskList[id - 1]->taskExecStatus == OS_TASK_BLOCKED );

	if(osCore.ptrCurrTask->taskPriority == osCore.ptrTaskList[id - 1]->taskPriority )
	{
		col--;
		row--;
		do
		{
			id = osCore.taskPriorityTable[row][col];
			col++;
		}
		while(osCore.ptrTaskList[id - 1]->taskExecStatus != OS_TASK_RUNNING);

		do
		{
			if(osCore.taskPriorityTable[row][col] == 0)
				col = 0;

			id = osCore.taskPriorityTable[row][col];
			col++;
		}
		while(osCore.ptrTaskList[id - 1]->taskExecStatus == OS_TASK_BLOCKED);

	}

	osCore.ptrNextTask = osCore.ptrTaskList[id - 1];			// Load next task to be run

}

/* ========== Processor Interruption and Exception Handlers ========= */

void SysTick_Handler(void)
{
    scheduler();
    osSysTickHook();

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

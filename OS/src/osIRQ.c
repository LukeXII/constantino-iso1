/*
 * osIRQ.c
 *
 *  Created on: Oct 7, 2023
 *      Author: Lucas
 */

#include "osIRQ.h"
#include <stddef.h>

bool osRegisterIRQ(osIRQnType irqType, IRQHandler function, void * data)
{
	bool registered = false;

	if( (data != NULL) && (function != NULL) )
	{
		if (irqVector[irqType].handler == NULL)
		{
			irqVector[irqType].handler = function;
			irqVector[irqType].data = data;

			NVIC_ClearPendingIRQ(irqType);
			NVIC_EnableIRQ(irqType);

			registered = true;
		}
	}

	return registered;
}

bool osUnregisterIRQ(osIRQnType irqType)
{
	bool unregistered = false;

	if (irqVector[irqType].handler != NULL)
	{
		irqVector[irqType].handler = NULL;

		NVIC_ClearPendingIRQ(irqType);
		NVIC_DisableIRQ(irqType);

		unregistered = true;
	}

	return unregistered;
}

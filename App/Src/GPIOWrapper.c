#include "GPIOWrapper.h"

void gpioSetLevel(uint16_t pin, uint32_t port, bool value)
{
	HAL_GPIO_WritePin(port, pin, value);
}


bool gpioGetLevel(uint16_t pin, uint32_t port)
{
	if(HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET)
		return true;
	else
		return false;
}

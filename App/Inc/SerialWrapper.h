#ifndef INC_SERIALWRAPPER_H_
#define INC_SERIALWRAPPER_H_

#include <stdint.h>
#include "main.h"

extern UART_HandleTypeDef huart3;

void serialPrint(char* buffer);

#endif /* INC_SERIALWRAPPER_H_ */

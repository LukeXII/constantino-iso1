################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../OS/src/osIRQ.c \
../OS/src/osKernel.c \
../OS/src/osQueue.c \
../OS/src/osSemaphore.c 

OBJS += \
./OS/src/osIRQ.o \
./OS/src/osKernel.o \
./OS/src/osQueue.o \
./OS/src/osSemaphore.o 

C_DEPS += \
./OS/src/osIRQ.d \
./OS/src/osKernel.d \
./OS/src/osQueue.d \
./OS/src/osSemaphore.d 


# Each subdirectory must supply rules for building sources it contributes
OS/src/%.o OS/src/%.su OS/src/%.cyclo: ../OS/src/%.c OS/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"../OS/inc" -I../OS/inc/Port -I../App/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-OS-2f-src

clean-OS-2f-src:
	-$(RM) ./OS/src/osIRQ.cyclo ./OS/src/osIRQ.d ./OS/src/osIRQ.o ./OS/src/osIRQ.su ./OS/src/osKernel.cyclo ./OS/src/osKernel.d ./OS/src/osKernel.o ./OS/src/osKernel.su ./OS/src/osQueue.cyclo ./OS/src/osQueue.d ./OS/src/osQueue.o ./OS/src/osQueue.su ./OS/src/osSemaphore.cyclo ./OS/src/osSemaphore.d ./OS/src/osSemaphore.o ./OS/src/osSemaphore.su

.PHONY: clean-OS-2f-src


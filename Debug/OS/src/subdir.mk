################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../OS/src/osCore.c 

OBJS += \
./OS/src/osCore.o 

C_DEPS += \
./OS/src/osCore.d 


# Each subdirectory must supply rules for building sources it contributes
OS/src/%.o OS/src/%.su OS/src/%.cyclo: ../OS/src/%.c OS/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"../OS/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-OS-2f-src

clean-OS-2f-src:
	-$(RM) ./OS/src/osCore.cyclo ./OS/src/osCore.d ./OS/src/osCore.o ./OS/src/osCore.su

.PHONY: clean-OS-2f-src


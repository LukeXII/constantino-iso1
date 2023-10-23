################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Src/GPIOWrapper.c \
../App/Src/SerialWrapper.c \
../App/Src/application.c \
../App/Src/inithardware.c 

OBJS += \
./App/Src/GPIOWrapper.o \
./App/Src/SerialWrapper.o \
./App/Src/application.o \
./App/Src/inithardware.o 

C_DEPS += \
./App/Src/GPIOWrapper.d \
./App/Src/SerialWrapper.d \
./App/Src/application.d \
./App/Src/inithardware.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/%.o App/Src/%.su App/Src/%.cyclo: ../App/Src/%.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"../OS/inc" -I../OS/inc/Port -I../App/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Src

clean-App-2f-Src:
	-$(RM) ./App/Src/GPIOWrapper.cyclo ./App/Src/GPIOWrapper.d ./App/Src/GPIOWrapper.o ./App/Src/GPIOWrapper.su ./App/Src/SerialWrapper.cyclo ./App/Src/SerialWrapper.d ./App/Src/SerialWrapper.o ./App/Src/SerialWrapper.su ./App/Src/application.cyclo ./App/Src/application.d ./App/Src/application.o ./App/Src/application.su ./App/Src/inithardware.cyclo ./App/Src/inithardware.d ./App/Src/inithardware.o ./App/Src/inithardware.su

.PHONY: clean-App-2f-Src


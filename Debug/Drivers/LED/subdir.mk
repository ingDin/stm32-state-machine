################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LED/led.c 

OBJS += \
./Drivers/LED/led.o 

C_DEPS += \
./Drivers/LED/led.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LED/%.o Drivers/LED/%.su Drivers/LED/%.cyclo: ../Drivers/LED/%.c Drivers/LED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Core/Inc" -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Drivers/LED" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-LED

clean-Drivers-2f-LED:
	-$(RM) ./Drivers/LED/led.cyclo ./Drivers/LED/led.d ./Drivers/LED/led.o ./Drivers/LED/led.su

.PHONY: clean-Drivers-2f-LED


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Delay/Src/delay.c 

OBJS += \
./Drivers/Delay/Src/delay.o 

C_DEPS += \
./Drivers/Delay/Src/delay.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Delay/Src/%.o Drivers/Delay/Src/%.su Drivers/Delay/Src/%.cyclo: ../Drivers/Delay/Src/%.c Drivers/Delay/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Core/Inc" -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Drivers/GPIO/Inc" -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Drivers/Delay/Inc" -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Drivers/UserButton/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Delay-2f-Src

clean-Drivers-2f-Delay-2f-Src:
	-$(RM) ./Drivers/Delay/Src/delay.cyclo ./Drivers/Delay/Src/delay.d ./Drivers/Delay/Src/delay.o ./Drivers/Delay/Src/delay.su

.PHONY: clean-Drivers-2f-Delay-2f-Src


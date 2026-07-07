################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/UserButton/Src/user_button.c 

OBJS += \
./Drivers/UserButton/Src/user_button.o 

C_DEPS += \
./Drivers/UserButton/Src/user_button.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/UserButton/Src/%.o Drivers/UserButton/Src/%.su Drivers/UserButton/Src/%.cyclo: ../Drivers/UserButton/Src/%.c Drivers/UserButton/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Core/Inc" -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Drivers/GPIO/Inc" -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Drivers/Delay/Inc" -I"D:/STM32CubeIDE/workspace_2.1.1/stm32-state-machine/Drivers/UserButton/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-UserButton-2f-Src

clean-Drivers-2f-UserButton-2f-Src:
	-$(RM) ./Drivers/UserButton/Src/user_button.cyclo ./Drivers/UserButton/Src/user_button.d ./Drivers/UserButton/Src/user_button.o ./Drivers/UserButton/Src/user_button.su

.PHONY: clean-Drivers-2f-UserButton-2f-Src


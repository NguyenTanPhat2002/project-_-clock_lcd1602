################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mylib/LiquidCrystal_I2C.c \
../Mylib/RTC.c \
../Mylib/btn.c 

OBJS += \
./Mylib/LiquidCrystal_I2C.o \
./Mylib/RTC.o \
./Mylib/btn.o 

C_DEPS += \
./Mylib/LiquidCrystal_I2C.d \
./Mylib/RTC.d \
./Mylib/btn.d 


# Each subdirectory must supply rules for building sources it contributes
Mylib/LiquidCrystal_I2C.o: C:/Users/LAPTOP/Desktop/Embedded\ System/project_dongho/Mylib/LiquidCrystal_I2C.c Mylib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/LAPTOP/Desktop/Embedded System/project_dongho/Mylib" -I"C:/Users/LAPTOP/Desktop/Embedded System/project_dongho/Mylib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Mylib/LiquidCrystal_I2C.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Mylib/RTC.o: C:/Users/LAPTOP/Desktop/Embedded\ System/project_dongho/Mylib/RTC.c Mylib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/LAPTOP/Desktop/Embedded System/project_dongho/Mylib" -I"C:/Users/LAPTOP/Desktop/Embedded System/project_dongho/Mylib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Mylib/RTC.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Mylib/btn.o: C:/Users/LAPTOP/Desktop/Embedded\ System/project_dongho/Mylib/btn.c Mylib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/LAPTOP/Desktop/Embedded System/project_dongho/Mylib" -I"C:/Users/LAPTOP/Desktop/Embedded System/project_dongho/Mylib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Mylib/btn.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Mylib

clean-Mylib:
	-$(RM) ./Mylib/LiquidCrystal_I2C.cyclo ./Mylib/LiquidCrystal_I2C.d ./Mylib/LiquidCrystal_I2C.o ./Mylib/LiquidCrystal_I2C.su ./Mylib/RTC.cyclo ./Mylib/RTC.d ./Mylib/RTC.o ./Mylib/RTC.su ./Mylib/btn.cyclo ./Mylib/btn.d ./Mylib/btn.o ./Mylib/btn.su

.PHONY: clean-Mylib


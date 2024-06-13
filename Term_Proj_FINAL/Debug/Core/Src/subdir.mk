################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/calibrate.c \
../Core/Src/controller.c \
../Core/Src/encoder_reader.c \
../Core/Src/main.c \
../Core/Src/motor_driver.c \
../Core/Src/myo.c \
../Core/Src/radio.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c 

OBJS += \
./Core/Src/calibrate.o \
./Core/Src/controller.o \
./Core/Src/encoder_reader.o \
./Core/Src/main.o \
./Core/Src/motor_driver.o \
./Core/Src/myo.o \
./Core/Src/radio.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o 

C_DEPS += \
./Core/Src/calibrate.d \
./Core/Src/controller.d \
./Core/Src/encoder_reader.d \
./Core/Src/main.d \
./Core/Src/motor_driver.d \
./Core/Src/myo.d \
./Core/Src/radio.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/calibrate.cyclo ./Core/Src/calibrate.d ./Core/Src/calibrate.o ./Core/Src/calibrate.su ./Core/Src/controller.cyclo ./Core/Src/controller.d ./Core/Src/controller.o ./Core/Src/controller.su ./Core/Src/encoder_reader.cyclo ./Core/Src/encoder_reader.d ./Core/Src/encoder_reader.o ./Core/Src/encoder_reader.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/motor_driver.cyclo ./Core/Src/motor_driver.d ./Core/Src/motor_driver.o ./Core/Src/motor_driver.su ./Core/Src/myo.cyclo ./Core/Src/myo.d ./Core/Src/myo.o ./Core/Src/myo.su ./Core/Src/radio.cyclo ./Core/Src/radio.d ./Core/Src/radio.o ./Core/Src/radio.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su

.PHONY: clean-Core-2f-Src


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/periph/adc.c \
../Src/periph/crc.c \
../Src/periph/dac.c \
../Src/periph/dma.c \
../Src/periph/gpio.c \
../Src/periph/interrupt.c \
../Src/periph/rcc.c \
../Src/periph/system_stm32f7xx.c \
../Src/periph/timer.c \
../Src/periph/uart.c 

OBJS += \
./Src/periph/adc.o \
./Src/periph/crc.o \
./Src/periph/dac.o \
./Src/periph/dma.o \
./Src/periph/gpio.o \
./Src/periph/interrupt.o \
./Src/periph/rcc.o \
./Src/periph/system_stm32f7xx.o \
./Src/periph/timer.o \
./Src/periph/uart.o 

C_DEPS += \
./Src/periph/adc.d \
./Src/periph/crc.d \
./Src/periph/dac.d \
./Src/periph/dma.d \
./Src/periph/gpio.d \
./Src/periph/interrupt.d \
./Src/periph/rcc.d \
./Src/periph/system_stm32f7xx.d \
./Src/periph/timer.d \
./Src/periph/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/periph/%.o Src/periph/%.su: ../Src/periph/%.c Src/periph/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCONTROL_MASTER -DARM_MATH_CM7 -D__FPU_PRESENT -DSTM32F746xx -DSTM32 -DSTM32F746ZGTx -DSTM32F7 -c -I../Inc -I"D:/STM_Lessons/Drivers/CMSIS/Include" -I"D:/STM_Lessons/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"D:/STM_Lessons/Src" -I"D:/STM_Lessons/Inc/periph" -I"D:/STM_Lessons/Src/periph" -I"D:/STM_Lessons/Drivers/CMSIS/DSP/Source/CommonTables" -I"D:/STM_Lessons/Drivers/CMSIS/DSP/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-periph

clean-Src-2f-periph:
	-$(RM) ./Src/periph/adc.d ./Src/periph/adc.o ./Src/periph/adc.su ./Src/periph/crc.d ./Src/periph/crc.o ./Src/periph/crc.su ./Src/periph/dac.d ./Src/periph/dac.o ./Src/periph/dac.su ./Src/periph/dma.d ./Src/periph/dma.o ./Src/periph/dma.su ./Src/periph/gpio.d ./Src/periph/gpio.o ./Src/periph/gpio.su ./Src/periph/interrupt.d ./Src/periph/interrupt.o ./Src/periph/interrupt.su ./Src/periph/rcc.d ./Src/periph/rcc.o ./Src/periph/rcc.su ./Src/periph/system_stm32f7xx.d ./Src/periph/system_stm32f7xx.o ./Src/periph/system_stm32f7xx.su ./Src/periph/timer.d ./Src/periph/timer.o ./Src/periph/timer.su ./Src/periph/uart.d ./Src/periph/uart.o ./Src/periph/uart.su

.PHONY: clean-Src-2f-periph


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/c_math.c \
../Src/control.c \
../Src/dsp.c \
../Src/main.c 

OBJS += \
./Src/c_math.o \
./Src/control.o \
./Src/dsp.o \
./Src/main.o 

C_DEPS += \
./Src/c_math.d \
./Src/control.d \
./Src/dsp.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCONTROL_MASTER -DARM_MATH_CM7 -D__FPU_PRESENT -DSTM32F746xx -DSTM32 -DSTM32F746ZGTx -DSTM32F7 -c -I../Inc -I"D:/STM_Lessons/Drivers/CMSIS/Include" -I"D:/STM_Lessons/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"D:/STM_Lessons/Src" -I"D:/STM_Lessons/Inc/periph" -I"D:/STM_Lessons/Src/periph" -I"D:/STM_Lessons/Drivers/CMSIS/DSP/Source/CommonTables" -I"D:/STM_Lessons/Drivers/CMSIS/DSP/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCONTROL_MASTER -DARM_MATH_CM7 -D__FPU_PRESENT -DSTM32F746xx -DSTM32 -DSTM32F746ZGTx -DSTM32F7 -c -I../Inc -I"D:/STM_Lessons/Drivers/CMSIS/Include" -I"D:/STM_Lessons/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"D:/STM_Lessons/Src" -I"D:/STM_Lessons/Inc/periph" -I"D:/STM_Lessons/Src/periph" -I"D:/STM_Lessons/Drivers/CMSIS/DSP/Source/CommonTables" -I"D:/STM_Lessons/Drivers/CMSIS/DSP/Include" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/c_math.d ./Src/c_math.o ./Src/c_math.su ./Src/control.d ./Src/control.o ./Src/control.su ./Src/dsp.d ./Src/dsp.o ./Src/dsp.su ./Src/main.d ./Src/main.o ./Src/main.su

.PHONY: clean-Src


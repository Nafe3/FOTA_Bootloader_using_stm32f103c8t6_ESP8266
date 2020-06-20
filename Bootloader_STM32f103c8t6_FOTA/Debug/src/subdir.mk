################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Bootloader.c \
../src/CRC.c \
../src/Debug.c \
../src/Delay_program.c \
../src/Flash.c \
../src/GPIO.c \
../src/HUART_program.c \
../src/NVIC_program.c \
../src/RCC_program.c \
../src/UART_program.c \
../src/WIFI_program.c \
../src/main.c 

OBJS += \
./src/Bootloader.o \
./src/CRC.o \
./src/Debug.o \
./src/Delay_program.o \
./src/Flash.o \
./src/GPIO.o \
./src/HUART_program.o \
./src/NVIC_program.o \
./src/RCC_program.o \
./src/UART_program.o \
./src/WIFI_program.o \
./src/main.o 

C_DEPS += \
./src/Bootloader.d \
./src/CRC.d \
./src/Debug.d \
./src/Delay_program.d \
./src/Flash.d \
./src/GPIO.d \
./src/HUART_program.d \
./src/NVIC_program.d \
./src/RCC_program.d \
./src/UART_program.d \
./src/WIFI_program.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



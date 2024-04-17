################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/DS3231_HAL.c \
../src/adc_temperature.c \
../src/gpt_pwm.c \
../src/hal_entry.c \
../src/main_thread0_entry.c \
../src/sci_uart.c \
../src/spi_w25q32.c 

C_DEPS += \
./src/DS3231_HAL.d \
./src/adc_temperature.d \
./src/gpt_pwm.d \
./src/hal_entry.d \
./src/main_thread0_entry.d \
./src/sci_uart.d \
./src/spi_w25q32.d 

OBJS += \
./src/DS3231_HAL.o \
./src/adc_temperature.o \
./src/gpt_pwm.o \
./src/hal_entry.o \
./src/main_thread0_entry.o \
./src/sci_uart.o \
./src/spi_w25q32.o 

SREC += \
BRS_2964_PracticeFSP.srec 

MAP += \
BRS_2964_PracticeFSP.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM4 -D_RA_ORDINAL=1 -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/src" -I"." -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/fsp/inc" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/fsp/inc/api" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/fsp/inc/instances" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/fsp/src/rm_freertos_port" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/aws/FreeRTOS/FreeRTOS/Source/include" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra_gen" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra_cfg/fsp_cfg/bsp" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra_cfg/fsp_cfg" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra_cfg/aws" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"


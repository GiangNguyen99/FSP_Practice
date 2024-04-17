################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/main_thread0.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/main_thread0.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/main_thread0.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

SREC += \
BRS_2964_PracticeFSP.srec 

MAP += \
BRS_2964_PracticeFSP.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM4 -D_RA_ORDINAL=1 -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/src" -I"." -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/fsp/inc" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/fsp/inc/api" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/fsp/inc/instances" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/fsp/src/rm_freertos_port" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/aws/FreeRTOS/FreeRTOS/Source/include" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra_gen" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra_cfg/fsp_cfg/bsp" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra_cfg/fsp_cfg" -I"C:/Users/giang.nguyen-hoang/e2_studio/workspace/BRS_2964_PracticeFSP/ra_cfg/aws" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"


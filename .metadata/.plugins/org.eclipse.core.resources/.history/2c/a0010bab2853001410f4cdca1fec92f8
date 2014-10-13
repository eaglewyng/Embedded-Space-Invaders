################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bitmaps.c \
../src/display.c \
../src/gameplay.c \
../src/globals.c \
../src/input.c \
../src/interrupts.c \
../src/movement.c \
../src/platform.c \
../src/score.c \
../src/space_invaders.c \
../src/utils.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/bitmaps.o \
./src/display.o \
./src/gameplay.o \
./src/globals.o \
./src/input.o \
./src/interrupts.o \
./src/movement.o \
./src/platform.o \
./src/score.o \
./src/space_invaders.o \
./src/utils.o 

C_DEPS += \
./src/bitmaps.d \
./src/display.d \
./src/gameplay.d \
./src/globals.d \
./src/input.d \
./src/interrupts.d \
./src/movement.d \
./src/platform.d \
./src/score.d \
./src/space_invaders.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../space_invaders_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '



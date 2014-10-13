################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
space_invaders.elf.size \

ELFCHECK += \
space_invaders.elf.elfcheck \


# All Target
all: space_invaders.elf secondary-outputs

# Tool invocations
space_invaders.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: MicroBlaze gcc linker
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../space_invaders_bsp_0/microblaze_0/lib -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -o"space_invaders.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

space_invaders.elf.size: space_invaders.elf
	@echo Invoking: MicroBlaze Print Size
	mb-size space_invaders.elf  |tee "space_invaders.elf.size"
	@echo Finished building: $@
	@echo ' '

space_invaders.elf.elfcheck: space_invaders.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck space_invaders.elf -hw ../../SpaceInvaderTestHW2_hw_platform/system.xml -pe microblaze_0  |tee "space_invaders.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) space_invaders.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

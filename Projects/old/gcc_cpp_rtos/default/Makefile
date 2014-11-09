###############################################################################
# Makefile for the project gcc_cpp_rtos
###############################################################################

## General Flags
PROJECT = gcc_cpp_rtos
MCU = atmega16a
TARGET = gcc_cpp_rtos.elf
CC = 

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=gcc_cpp_rtos.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Andrii\Micro\Projects\gcc_cpp_rtos\." 

## Objects that must be built in order to link
OBJECTS = mainTest.o libRTOS.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) gcc_cpp_rtos.hex gcc_cpp_rtos.eep gcc_cpp_rtos.lss## Compile
mainTest.o: ../mainTest.cpp
	$(CPP) $(INCLUDES) $(CFLAGS) -c  $<

libRTOS.o: ../rtos/libRTOS.cpp
	$(CPP) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CPP) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) gcc_cpp_rtos.elf dep/* gcc_cpp_rtos.hex gcc_cpp_rtos.eep gcc_cpp_rtos.lss gcc_cpp_rtos.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)


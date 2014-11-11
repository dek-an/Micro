###############################################################################
# Makefile for the project GCC-RTOS
###############################################################################

## General Flags
PROJECT = GCC-RTOS
MCU = atmega16
TARGET = GCC-RTOS.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=GCC-RTOS.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\adekar\avr\rtos\GCC-RTOS\." 

## Objects that must be built in order to link
OBJECTS = GCC-RTOS.o EERTOS.o HAL.o EERTOSHAL.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) GCC-RTOS.hex GCC-RTOS.eep GCC-RTOS.lss## Compile
GCC-RTOS.o: ../GCC-RTOS.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

EERTOS.o: ../EERTOS.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

HAL.o: ../HAL.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

EERTOSHAL.o: ../EERTOSHAL.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) GCC-RTOS.elf dep/* GCC-RTOS.hex GCC-RTOS.eep GCC-RTOS.lss GCC-RTOS.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)


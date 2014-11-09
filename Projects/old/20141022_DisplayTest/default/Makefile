###############################################################################
# Makefile for the project 20141022_DisplayTest
###############################################################################

## General Flags
PROJECT = 20141022_DisplayTest
MCU = atmega16a
TARGET = 20141022_DisplayTest.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99     -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=20141022_DisplayTest.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Andrii\Micro\Projects\20141022_DisplayTest\." 

## Objects that must be built in order to link
OBJECTS = 20141022_DisplayTest_.o lcdWH1602b.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) 20141022_DisplayTest.hex 20141022_DisplayTest.eep 20141022_DisplayTest.lss## Compile
20141022_DisplayTest_.o: ../20141022_DisplayTest_.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcdWH1602b.o: ../lcdWH1602b/lcdWH1602b.c
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
	-rm -rf $(OBJECTS) 20141022_DisplayTest.elf dep/* 20141022_DisplayTest.hex 20141022_DisplayTest.eep 20141022_DisplayTest.lss 20141022_DisplayTest.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)


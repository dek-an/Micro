###############################################################################
# Makefile for the project Test_4_clock
###############################################################################

## General Flags
PROJECT = Test_4_clock
MCU = atmega16a
TARGET = Test_4_clock.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Test_4_clock.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Andrii\Micro\Projects\AVRStudio4\Test_4_clock\..\.." 

## Objects that must be built in order to link
OBJECTS = Test_4_clock.o lcdWH1602b.o rtos.o LCD.o clock.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Test_4_clock.hex Test_4_clock.eep Test_4_clock.lss## Compile
Test_4_clock.o: ../Test_4_clock.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcdWH1602b.o: ../../../common/lcdWH1602b/lcdWH1602b.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rtos.o: ../../../common/rtos/rtos.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCD.o: ../../../thirdParty/Atmega16_LCD_LIB/LCD.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

clock.o: ../../../common/clock/clock.c
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
	-rm -rf $(OBJECTS) Test_4_clock.elf dep/* Test_4_clock.hex Test_4_clock.eep Test_4_clock.lss Test_4_clock.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)


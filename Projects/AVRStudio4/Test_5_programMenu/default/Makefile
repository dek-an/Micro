###############################################################################
# Makefile for the project Test_5_programMenu
###############################################################################

## General Flags
PROJECT = Test_5_programMenu
MCU = atmega16
TARGET = Test_5_programMenu.elf
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
LDFLAGS +=  -Wl,-Map=Test_5_programMenu.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Andrii\Micro\Projects\AVRStudio4\Test_5_programMenu\..\.." 

## Objects that must be built in order to link
OBJECTS = Test_5_programMenu.o LCD.o clock.o keyboard.o lcdWH1602b.o menu.o rtos.o program.o eepromConstants.o OWIcrc.o OWIUARTBitFunctions.o OWIHighLevelFunctions.o OWISWBitFunctions.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Test_5_programMenu.hex Test_5_programMenu.eep Test_5_programMenu.lss## Compile
Test_5_programMenu.o: ../Test_5_programMenu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCD.o: ../../../thirdParty/Atmega16_LCD_LIB/LCD.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

clock.o: ../../../common/clock/clock.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

keyboard.o: ../../../common/keyboard/keyboard.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcdWH1602b.o: ../../../common/lcdWH1602b/lcdWH1602b.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

menu.o: ../../../common/menu/menu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rtos.o: ../../../common/rtos/rtos.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

program.o: ../program.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

eepromConstants.o: ../eepromConstants.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

OWIcrc.o: ../../../thirdParty/oneWire/common_files/OWIcrc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

OWIUARTBitFunctions.o: ../../../thirdParty/oneWire/OWIUARTBitFunctions.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

OWIHighLevelFunctions.o: ../../../thirdParty/oneWire/OWIHighLevelFunctions.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

OWISWBitFunctions.o: ../../../thirdParty/oneWire/OWISWBitFunctions.c
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
	-rm -rf $(OBJECTS) Test_5_programMenu.elf dep/* Test_5_programMenu.hex Test_5_programMenu.eep Test_5_programMenu.lss Test_5_programMenu.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)


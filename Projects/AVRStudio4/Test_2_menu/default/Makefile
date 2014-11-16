###############################################################################
# Makefile for the project Test_2_menu
###############################################################################

## General Flags
PROJECT = Test_2_menu
MCU = atmega16
TARGET = Test_2_menu.elf
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
LDFLAGS +=  -Wl,-Map=Test_2_menu.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Andrii\Micro\Projects\AVRStudio4\Test_2_menu\..\.." 

## Objects that must be built in order to link
OBJECTS = Test_2_menu.o LCD.o lcdWH1602b.o libRtos.o menu.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Test_2_menu.hex Test_2_menu.eep Test_2_menu.lss## Compile
Test_2_menu.o: ../Test_2_menu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCD.o: ../../../thirdParty/Atmega16_LCD_LIB/LCD.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcdWH1602b.o: ../../../common/lcdWH1602b/lcdWH1602b.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

libRtos.o: ../../../common/rtos/libRtos.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

menu.o: ../../../common/menu/menu.c
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
	-rm -rf $(OBJECTS) Test_2_menu.elf dep/* Test_2_menu.hex Test_2_menu.eep Test_2_menu.lss Test_2_menu.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)


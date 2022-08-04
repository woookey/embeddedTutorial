TARGET = tut4
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -Wall -g --specs=nosys.specs -mthumb -mcpu=cortex-m4 -Os
CFLAGS += -DSTM32F407xx -DUSE_FULL_LL_DRIVER
LFLAGS =  -L../Linker -TSTM32F407VGTx_FLASH.ld 

INC = -I../CMSIS/Device/ST/STM32F4xx/Include
INC += -I../CMSIS/Include
INC += -I../STM32F4xx_HAL_Driver/Inc/
INC += -I./

DEPENDENCIES_PART1 = main_part1.o
DEPENDENCIES_PART1 += startup_stm32f407xx.o
DEPENDENCIES_PART1 += system_stm32f4xx.o
DEPENDENCIES_PART1 += clocks/clocks.o
DEPENDENCIES_PART1 += comms_driver/comms_driver.o
DEPENDENCIES_PART1 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o
DEPENDENCIES_PART1 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o
DEPENDENCIES_PART1 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o
DEPENDENCIES_PART1 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o
DEPENDENCIES_PART1 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.o
DEPENDENCIES_PART1 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.o
DEPENDENCIES_PART1 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.o
DEPENDENCIES_PART1 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o

DEPENDENCIES_PART2 = main_part2.o
DEPENDENCIES_PART2 += startup_stm32f407xx.o
DEPENDENCIES_PART2 += system_stm32f4xx.o
DEPENDENCIES_PART2 += clocks/clocks.o
DEPENDENCIES_PART2 += comms_driver/comms_driver.o
DEPENDENCIES_PART2 += comms/comms.o
DEPENDENCIES_PART2 += comms/comms_cmd.o
DEPENDENCIES_PART2 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o
DEPENDENCIES_PART2 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o
DEPENDENCIES_PART2 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o
DEPENDENCIES_PART2 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o
DEPENDENCIES_PART2 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.o
DEPENDENCIES_PART2 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.o
DEPENDENCIES_PART2 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.o
DEPENDENCIES_PART2 += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o

all: tut4_part1 tut4_part2

tut4_part1: $(DEPENDENCIES_PART1)
	$(CC) $(CFLAGS) $(DEPENDENCIES_PART1) $(INC) $(LFLAGS) -o $(TARGET)_part1.axf;
	$(OBJCOPY) -O binary $(TARGET)_part1.axf $(TARGET)_part1.bin

tut4_part2: $(DEPENDENCIES_PART2)
	$(CC) $(CFLAGS) $(DEPENDENCIES_PART2) $(INC) $(LFLAGS) -o $(TARGET)_part2.axf;
	$(OBJCOPY) -O binary $(TARGET)_part2.axf $(TARGET)_part2.bin

%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
	
%.o : %.s
	$(AS) $(INC) -c $< -o $@
	
stlink_texane_reflash_part1:
	st-flash write $(TARGET)_part1.bin 0x8000000

stlink_texane_reflash_part2:
	st-flash write $(TARGET)_part2.bin 0x8000000

clean:
	rm -f *.axf *.bin
	find ../ -type f -name '*.o' -delete

.PHONY: clean all

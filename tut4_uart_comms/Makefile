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

DEPENDENCIES = main.o 
DEPENDENCIES += startup_stm32f407xx.o
DEPENDENCIES += system_stm32f4xx.o
DEPENDENCIES += clocks/clocks.o
DEPENDENCIES += comms_driver/comms_driver.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o
#DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o

all: $(DEPENDENCIES)
	$(CC) $(CFLAGS) $(DEPENDENCIES) $(INC) $(LFLAGS) -o $(TARGET).axf;
	$(OBJCOPY) -O binary $(TARGET).axf $(TARGET).bin

%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
	
%.o : %.s
	$(AS) $(INC) -c $< -o $@
	
stlink_texane_reflash:
	st-flash write $(TARGET).bin 0x8000000

stlink_v2_connect:
	openocd --file $(shell pwd)/../tools/stlink-v2-swd.cfg

stlink_v2_reflash:
	openocd --file $(shell pwd)/../tools/stlink-v2-swd.cfg -c "program tut4.bin 0x08000000 exit"

stlink_v2_1_connect:
	openocd --file $(shell pwd)/../tools/stlink-v2-1-swd.cfg

stlink_v2_1_reflash:
	openocd --file $(shell pwd)/../tools/stlink-v2-1-swd.cfg -c "program tut4.bin 0x08000000 exit"

olimex_connect:
	openocd --file $(shell pwd)/../tools/olimex-jtag.cfg

olimex_reflash:
	openocd --file $(shell pwd)/../tools/olimex-jtag.cfg -c "program tut4.bin 0x08000000 exit"

clean:
	rm -f *.axf *.bin
	find ../ -type f -name '*.o' -delete

.PHONY: clean all

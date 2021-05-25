TARGET = tut2
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -Wall -g --specs=nosys.specs -mthumb -mcpu=cortex-m4 -Os
CFLAGS += -DSTM32F407xx
LFLAGS =  -L../Linker -TSTM32F407VGTx_FLASH.ld 

INC = -I../CMSIS/Device/ST/STM32F4xx/Include
INC += -I../CMSIS/Include
INC += -I../STM32F4xx_HAL_Driver/Inc/

DEPENDENCIES = main.o 
DEPENDENCIES += startup_stm32f407xx.o
DEPENDENCIES += system_stm32f4xx.o
DEPENDENCIES += clocks/clocks.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o

all: $(DEPENDENCIES)
	$(CC) $(CFLAGS) $(DEPENDENCIES) $(INC) $(LFLAGS) -o $(TARGET).axf;
	$(OBJCOPY) -O binary $(TARGET).axf $(TARGET).bin
	
%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
	
%.o : %.s
	$(AS) $(INC) -c $< -o $@
	
reflash:
	st-flash write $(TARGET).bin 0x8000000

clean:
	rm -f *.axf *.bin
	find ../ -type f -name '*.o' -delete
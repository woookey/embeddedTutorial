CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
CFLAGS = -Wall -g --specs=nosys.specs -mthumb -mcpu=cortex-m4
CFLAGS += -DSTM32F407xx
LFLAGS = -TSTM32F407VGTx_FLASH.ld -LLinker

INC = -I./CMSIS/Device/ST/STM32F4xx/Include
INC += -I./CMSIS/Include

DEPENDENCIES = main.o 
DEPENDENCIES += startup_stm32f407xx.o
DEPENDENCIES += system_stm32f4xx.o


all: $(DEPENDENCIES)
	$(CC) $(CFLAGS) $(DEPENDENCIES) $(LFLAGS) -o tut1.axf 
	
%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf *.o *.axf *.elf

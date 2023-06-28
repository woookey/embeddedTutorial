TARGET = tut5
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -Wall -g --specs=nosys.specs -mthumb -mcpu=cortex-m4 -Og -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DSTM32F407xx -DUSE_FULL_LL_DRIVER
LFLAGS =  -L../Linker -TSTM32F407VGTx_FLASH.ld 

INC = -I../CMSIS/Device/ST/STM32F4xx/Include
INC += -I../CMSIS/Include
INC += -I../STM32F4xx_HAL_Driver/Inc/
INC += -I./

DEPENDENCIES = main.o
DEPENDENCIES += startup_stm32f407xx.o
DEPENDENCIES += system_stm32f4xx.o
DEPENDENCIES += clocks/clocks.o
DEPENDENCIES += comms_driver/comms_driver.o
DEPENDENCIES += comms/comms.o
DEPENDENCIES += comms/comms_command.o
DEPENDENCIES += motion_sensor/motion_sensor.o
DEPENDENCIES += motion_sensor_driver/motion_sensor_driver.o
DEPENDENCIES += filters/MAFilter/MAFilter.o
DEPENDENCIES += filters/RMAFilter/RMAFilter.o
DEPENDENCIES += filters/LPFilter/LPFilter.o
DEPENDENCIES += filters/RCFilter/RCFilter.o
DEPENDENCIES += filters/KalmanFilter1D/KalmanFilter1D.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o
DEPENDENCIES += ../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.o

all: tut5

tut5: $(DEPENDENCIES)
	$(CC) $(CFLAGS) $(DEPENDENCIES) $(INC) $(LFLAGS) -o $(TARGET).axf;
	$(OBJCOPY) -O binary $(TARGET).axf $(TARGET).bin

%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
	
%.o : %.s
	$(AS) $(INC) -c $< -o $@
	
reflash:
	st-flash write $(TARGET).bin 0x8000000

debug: clean tut5
	arm-none-eabi-gdb tut5.axf -ex "target extended-remote localhost:4242" -ex "monitor reset" -ex "load" -ex "monitor reset init"

clean:
	rm -f *.axf *.bin
	find ../ -type f -name '*.o' -delete

.PHONY: clean all
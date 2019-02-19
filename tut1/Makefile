CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
CFLAGS = -c -mcpu=cortex-m4 -mthumb
LD=arm-none-eabi-ld
LIBS = -L/opt/gcc-arm-none-eabi-6-2017-q2-update/arm-none-eabi/lib/thumb/ \
-L/opt/gcc-arm-none-eabi-6-2017-q2-update/lib/gcc/arm-none-eabi/6.3.1/thumb/
LSCRIPT=-TSTM32F407VGTx_FLASH.ld
CRT0 = /opt/gcc-arm-none-eabi-6-2017-q2-update/arm-none-eabi/lib/crt0.o

build: main.o startup.o
	$(LD) $(LIBS) $(LSCRIPT) $(CRT0) main.o startup.o

main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

startup.o: startup_stm32f407xx.s
	$(AS) $(CFLAGS) startup_stm32f407xx.s -o startup.o

clean:
	rm -rf *.o

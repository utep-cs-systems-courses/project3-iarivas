# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:Lab3.elf

#additional rules for files
Lab3.elf: ${COMMON_OBJECTS} wdt_handler.o switch_advance.o buzzer.o switches.o main.o ../lib/libTimer.a
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: Lab3.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf *.*~

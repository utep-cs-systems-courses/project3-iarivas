# makfile configuration
COMMON_OBJECTS  = 
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h
LDFLAGS		= -L/opt/ti/msp430_gcc/include -L../lib

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as -mmcu=${CPU} -c
AR              = msp430-elf-ar

all: Toy-c.elf libLcd.a

libLcd.a: font-11x16.o font-5x7.o font-8x12.o lcdutils.o lcddraw.o
	$(AR) crs $@ $^

lcddraw.o: lcddraw.c lcddraw.h lcdutils.h
lcdutils.o: lcdutils.c lcdutils.h

#additional rules for files
Toy-c.elf: ${COMMON_OBJECTS} Main.o led.o wdInterruptHandler.o p1_interrupt_handler.o switches.o buzzer.o libLcd.a stateMachines.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer


#Toy-s.elf: ${COMMON_OBJECTS} blinkMain.o stateMachines.o led.o wdInterruptHandler.o lib/libTimer.a p1_interrupt_handler.o switches.o buzzer.o state_reset-s.o
#	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $

load-c: Toy-c.elf
	msp430loader.sh Toy-c.elf

#load-s: Toy-s.elf
#	msp430loader.sh Toy.elf

clean:
	rm -f *.o *.elf

wdInterruptHandler.o: stateMachines.h led.h switches.h buzzer.h

stateMachines.o: stateMachines.h led.h switches.h buzzer.h lcddraw.h lcdutils.h lcdtypes.h

lcddraw.o: lcddraw.h lcdutils.h lcdtypes.h

lcdutils.o: lcdutils.h lcdtypes.h


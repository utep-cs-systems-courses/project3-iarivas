#ifndef switches_included
#define switches_included

#define SW1 BIT0 //Left -> right       
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3

#define SWITCHES ( SW1 | SW2 | SW3 | SW4 )   /* Uses all switch on the board */

void switch_init();
void switch_interrupt_handler();

extern char switch_state;
#endif

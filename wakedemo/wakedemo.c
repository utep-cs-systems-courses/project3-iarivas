#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

short drawPos[2] = {10,10}, controlPos[2] = {10,10};
short velocity[2] = {3,8}, limits[2] = {screenWidth-35, screenHeight-8};

short redrawScreen = 1;
u_int fontFgColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount == 75) {		/* 4/sec */
    secCount = 0;
    for (char axis = 0; axis < 2; axis++) {
      short newVal = controlPos[axis] + velocity[axis];
      if (newVal < 10 || newVal > limits[axis])
	velocity[axis] = -velocity[axis];
      else
	controlPos[axis] = newVal;
    }
    fontFgColor = (fontFgColor == COLOR_GREEN) ? COLOR_BLACK : COLOR_GREEN;
    redrawScreen = 1;
  }
}
  

void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_BLUE);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      drawString5x7(drawPos[0], drawPos[1],
		    "hello", COLOR_BLUE, COLOR_BLUE); /* erase old message */

      and_sr(~8);		/* disable interrupts */
      for (char axis = 0; axis < 2; axis++) 
	drawPos[axis] = controlPos[axis];
      drawString5x7(drawPos[0], drawPos[1], 
		    "hello", fontFgColor, COLOR_BLUE); /* draw new message */
      or_sr(8);			/* enable interrupts */
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

    
    




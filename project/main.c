#include <msp430.h>
#include "libTimer.h"
#include "switches.h"
#include "buzzer.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "switch_advance.h"

#define LED BIT6
short shapes[2] = {0,1};
short drawPos[2] = {10,10}, controlPos[2] = {10,10}, prvPos[2] = {10,10};
short velocity[2] = {0,0}, limits[2] = {screenWidth-35, screenHeight-8};
u_int Colors[5] = {COLOR_AQUAMARINE , COLOR_FOREST_GREEN, COLOR_SEA_GREEN, COLOR_FIREBRICK, COLOR_GOLD};
char switch_state = 4;
int current_color = 0, lastShape = 0, BSP = 1000; 
short redrawScreen = 1;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount++;
  if (secCount == 50) {
    secCount = 0;
    current_color++;
    if(current_color > 5)
      current_color = 0;
    for (char axis = 0; axis < 2; axis++) {
      short newVal = controlPos[axis] + velocity[axis];
      if (newVal < 10 || newVal > limits[axis])
	velocity[axis] = -velocity[axis];
      else
	controlPos[axis] = newVal;
    }
    redrawScreen = 1;
  }
}

void draw_shape()
{
  for (char axis = 0; axis < 2; axis++)
	prvPos[axis] = drawPos[axis];
  switch(lastShape){
  case 0:
    fillRectangle(drawPos[0], drawPos[1], 40, 40, Colors[current_color]);
  case 1:
    for(int i = 0; i < 64; i++)
      {
	int startCol = drawPos[0] - 1;
	int endCol = drawPos[1] + 1;
	int width = 1 + endCol - startCol;
	fillRectangle(startCol, drawPos[1], width, 1, Colors[current_color]);
      }
  }
}

void clear_shape()
{
  switch(lastShape){
  case 0:
    for(int i = 0; i < 64; i++)
      {
	int startCol = prvPos[0] - 1;
	int endCol = prvPos[1] + 1;
	int width = 1 + endCol - startCol;
	fillRectangle(startCol, prvPos[1], width, 1, COLOR_WHITE);
      }

  case 1:
    fillRectangle(prvPos[0], prvPos[1], 40, 40, COLOR_WHITE);
  }
}

void main()
{
  
  P1DIR |= LED;
  P1OUT |= LED;
  configureClocks();
  lcd_init();

  enableWDTInterrupts();
  or_sr(0x8);  

  switch_init();
  clearScreen(COLOR_WHITE);
  
  while (1) {
    if(redrawScreen){
      redrawScreen = 0;
      and_sr(~8);
      P1OUT &= ~LED;
      for (char axis = 0; axis < 2; axis++)
	drawPos[axis] = controlPos[axis];
      if(switch_state != 4){
	P1OUT |= LED;
	switch(switch_state){
	case 0:
	  lastShape = (shapes[lastShape] == 0) ? 1 : 0;
	  draw_shape();
	case 1:
	  if(BSP >= 3000){
	    BSP = 3000;
	  }else{
	    BSP += 250;
	  }
	  buzzer_set_period(BSP);
	  if(velocity[0] <= 8){
	    velocity[0] = velocity[0] + 2;
	  }else{
	    velocity[0] = 0;
	  }
	case 2:
	  if(BSP <= 1000){
	    BSP = 1000;
	  }else{
	    BSP -= 250;
	  }
	  buzzer_set_period(BSP);
	  if(velocity[1] <= 8){
	    velocity[1] = velocity[0] + 2;
	  }else{
	    velocity[1] = 0;
	  }
	case 3:
	  clearScreen(COLOR_WHITE);
	  buzzer_set_period(0);
	  velocity[0] = 0;
	  velocity[1] = 0;
	}
      }
    }
    else{
      P1OUT |= LED;
    }
    or_sr(8);
  }
  
  or_sr(0x10);
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;      
    switch_interrupt_handler();
  }
}

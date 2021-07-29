#include <msp430.h>
#include "libTimer.h"
#include "switch_advance.h"
#include "switches.h"
#include "buzzer.h"
#include "lcdutils.h"
#include "lcddraw.h"

#define LED BIT6
short shapes[2] = {0,1};
short drawPos[2] = {30,30}, controlPos[2] = {30,30}, prvPos[2] = {30,30};
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
      if (newVal <10 || newVal > limits[axis])
	velocity[axis] = -velocity[axis];
      else
	controlPos[axis] = newVal;
    }
    redrawScreen = 1;
  }
}

void draw_shape()
{
  //for (char axis = 0; axis < 2; axis++)
    //	prvPos[axis] = drawPos[axis];
  switch(lastShape){
  case 0:
    fillRectangle(drawPos[0]-25, drawPos[0], 50, 25, Colors[current_color]);
  case 1:
    for(int i = 0; i < 25; i++)
      {
	int startCol = drawPos[0] - i;
	int endCol = drawPos[0] + i;
	int width = 1 + endCol - startCol;
	fillRectangle(startCol, drawPos[0]+i, width, 1, Colors[current_color]);
      }
  }
}

void clear_shape()
{
  switch(lastShape){
  case 0:
    for(int i = 0; i < 25; i++)
      {
	int startCol = prvPos[0] - i;
	int endCol = prvPos[0] + i;
	int width = 1 + endCol - startCol;
	fillRectangle(startCol, prvPos[0]+i, width, 1, COLOR_WHITE);
      }

  case 1:
    fillRectangle(prvPos[0]-25, prvPos[0], 50, 25, COLOR_WHITE);
  }
}

void main()
{
  
  P1DIR |= LED;
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  buzzer_init();
  enableWDTInterrupts();
  or_sr(0x8);  

  switch_init();
  clearScreen(COLOR_WHITE);
  
  while (1) {
    if(redrawScreen){
      redrawScreen = 0;
      and_sr(~8);
      P1OUT &= ~LED;
      for (char axis = 0; axis < 2; axis++){
	prvPos[axis] = drawPos[axis];
	drawPos[axis] = controlPos[axis];
      }
	if(switch_state != 4){
	switch(switch_state){
	case 0:
	  lastShape = (shapes[lastShape] == 0) ? 1 : 0;
	  clear_shape();
	  draw_shape();
	  switch_state = 4;
	  break; 
	case 1:
	  if(BSP >= 3000){
	    BSP = 1000;
	  }else{
	    BSP += 250;
	  }
	  buzzer_set_period(BSP);
	  if(velocity[0] <= 3){
	    velocity[0] = velocity[0] + 1;
	  }else{
	    velocity[0] = 0;
	  }
	  clear_shape();
	  draw_shape();
	  break;
	case 2:
	  if(BSP <= 1000){
	    BSP = 3000;
	  }else{
	    BSP -= 250;
	  }
	  buzzer_set_period(BSP);
	  if(velocity[1] <= 6){
	    velocity[1] = velocity[1] + 2;
	  }else{
	    velocity[1] = 2;
	  }
	  clear_shape();
	  draw_shape();
	  break;
	case 3:
	  clearScreen(COLOR_WHITE);
	  buzzer_set_period(0);
	  velocity[0] = 0;
	  velocity[1] = 0;
	  drawPos[0] = 30; drawPos[1] = 30;
	  controlPos[0] = 30; controlPos[1] = 30;
	  prvPos[0] = 30; prvPos[1] = 30;
	  break;
	}
      }
    }
    or_sr(8);
    or_sr(0x10);
    P1OUT |= LED;
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;      
    switch_interrupt_handler();
  }
}

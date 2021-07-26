#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"
#include "lcddraw.h"
#include "lcdutils.h"

#define LED BIT6

static char gcount = 0;
static char rcount = 0;
static char siren = 0; 
static char note = 0;
int song[] = {3824,3405,3033,2863,2551,2272,2026,0,0,0,0};
short drawPos[4] = {30,30,40,40}, controlPos[4] = {30,30,40,40}, last[4] = {10,10,20,20};
short velocity[4] = {3,0,3,0}, limits[4] = {screenWidth-35, screenHeight-8, screenWidth-35, screenHeight-8};
u_int fontFgColor = COLOR_RED;

void toggle_red()		/* always toggle! */
{
  switch (rcount) {
  case 0:
    red_on = 1;
    rcount = 1;
    break;
  case 1:
    red_on = 0;
    rcount = 0;
    break;
  }
  led_update();
}

void toggle_red_dim()
{
  switch (rcount) {
  case 0:
    red_on = 1;
    rcount = 1;
    break;
  case 1:
    red_on = 0;
    rcount = 2;
    break;
  case 2:
    red_on = 0;
    rcount = 0;
    break;
  }
  led_update();
}

void toggle_red_bright()
{
  switch(rcount) {
  case 0:
    red_on = 1;
    rcount = 1;
    break;
  case 1:
    red_on = 1;
    rcount = 2;
    break;
  case 2:
    red_on = 0;
    rcount = 0;
    break;
  }
  led_update();
}

void toggle_red_off()
{
  red_on = 0;
  led_update();
}

void red_dim(int level)
{
  switch(level){
  case 0:
    toggle_red_dim();
    break;
  case 1:
    toggle_red();
    break;
  case 2:
    toggle_red_bright();
    break;
  case 3:
    toggle_red_off();
    break;
  }
}

void toggle_green()
{
  switch(gcount){
  case 0:
    green_on = 1;
    gcount = 1;
    break;
  case 1:
    green_on = 0;
    gcount = 0;
    break;
  }
  led_update();
}

void toggle_green_dim()
{
  switch(gcount){
  case 0:
    green_on = 1;
    gcount = 1;
    break;
  case 1:
    green_on = 0;
    gcount = 2;
    break;
  case 2:
    green_on = 0;
    gcount = 0;
    break;
  }
  led_update();
}

void toggle_green_bright()
{
  switch(gcount){
    case 0:
      green_on = 1;
      gcount = 1;
      break;
    case 1:
      green_on = 1;
      gcount = 2;
      break;
    case 2:
      green_on = 0;
      gcount = 0;
      break;
  }
  led_update();
}

void toggle_green_off()
{
  green_on = 0;
  led_update();
}

void green_dim(int level)
{
  switch(level){
  case 0:
    toggle_green_dim();
    break;
  case 1:
    toggle_green();
    break;
  case 2:
    toggle_green_bright();
    break;
  case 3:
    toggle_green_off();
    break;
  }
}

void toy_off(){ toggle_red_off(); toggle_green_off(); buzzer_set_period(0); }

void police_siren()
{
  switch(siren){
  case 0:
    toggle_red();
    buzzer_set_period(5000);
    siren = 1;
    break;
  case 1:
    toggle_green();
    buzzer_set_period(1000);
    siren = 0;
    update_loc();
    break; 
  }
}

void first_tune()
{
  if(note == 10){ note = 0; }
  else if((note % 2) == 0){
    buzzer_set_period(0);
  }
  else if((note % 3) == 0){
    update_loc();
    drawString5x7(drawPos[0], drawPos[1],
		  "00000000", fontFgColor, COLOR_YELLOW);
    drawString5x7(drawPos[2], drawPos[3],
		  "00000000", fontFgColor, COLOR_YELLOW);
  }
  drawString5x7(last[0], last[1],
		  "00000000", COLOR_BLUE, COLOR_BLUE);
  drawString5x7(last[2], last[3],
		  "00000000", COLOR_BLUE, COLOR_BLUE);
  buzzer_set_period(song[note]);
  note++;
}

void first_song()
{
  if(note == 12){ note = 0; }
  else if((note % 2) == 0){
    buzzer_set_period(0);
    toggle_green_bright();
  }
  if(song[note] != 0){
    toggle_red();    
  }
  buzzer_set_period(song[note]);
  note++;
}

void update_loc()
{
  for (char axis = 0; axis < 4; axis++) {
    short newVal = controlPos[axis] + velocity[axis];
    if (newVal < 10 || newVal > limits[axis])
      velocity[axis] = -velocity[axis];
    else
      controlPos[axis] = newVal;
  }
  fontFgColor = (fontFgColor == COLOR_RED) ? COLOR_RED : COLOR_BLACK;
  for (char axis = 0; axis < 4; axis++)
  {
    last[axis] = drawPos[axis];
    drawPos[axis] = controlPos[axis];
  }
}

void state_reset(){ gcount = 0; rcount = 0; siren = 0; note = 0; }
  

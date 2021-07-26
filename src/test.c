#include "stateMachines.h"
#include "led.h"

void toggle_green_off()
{
  green_on = 0;
  led_update();
}

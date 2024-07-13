#include "led.hpp"

#include "gpio.h"

extern "C" {
void LED_RED()
{
  while (1) {
    HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_12);
    HAL_Delay(1000);
  }
}
}
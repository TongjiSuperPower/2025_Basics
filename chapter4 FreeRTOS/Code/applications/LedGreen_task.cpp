#include "LedGreen_task.hpp"

#include "cmsis_os.h"
#include "gpio.h"
#include "main.h"

LedGreen::LedGreen() { HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET); }

LedGreen::~LedGreen() {}

extern "C" {
void LED_Green_Task(void)
{
  LedGreen ledgreen;
  while (true) {
    HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_11);
    vTaskDelay(200);
  }
}
}

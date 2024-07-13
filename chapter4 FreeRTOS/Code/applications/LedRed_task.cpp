#include "LedRed_task.hpp"

#include "cmsis_os.h"
#include "gpio.h"
#include "main.h"

LedRed::LedRed() { HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_SET); }

LedRed::~LedRed() {}

extern "C" {
void LED_Red_Task(void)
{
  LedRed ledred;
  vTaskDelay(1000);
  while (true) {
    HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_12);
    vTaskDelay(300);
  }
}
}
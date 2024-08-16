#include "cmsis_os.h"
#include "gpio.h"

extern "C" void led_red_task(void)
{
  while (true) {
    HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_12);
    vTaskDelay(500);
  }
}
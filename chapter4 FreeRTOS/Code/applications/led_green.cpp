#include "cmsis_os.h"
#include "gpio.h"

extern "C" void led_green_task(void)
{
  while (true) {
    HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_11);
    vTaskDelay(200);
  }
}
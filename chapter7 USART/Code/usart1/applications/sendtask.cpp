#include "sendtask.hpp"

#include "cmsis_os.h"
#include "main.h"
#include "usart.h"

extern "C" {

void send_task()
{
  Send send;
  while (1) {
    send.send_data++;
    if (send.send_data > 5) {
      send.send_data = 0;
    }
    HAL_UART_Transmit(&huart1, &send.send_data, 1, 200);
    vTaskDelay(1);
  }
}
}

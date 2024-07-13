#include "sendtask.hpp"

#include "cmsis_os.h"
#include "main.h"
#include "usart.h"

Send send;

extern "C" {
void send_task()
{
  HAL_UART_Receive_IT(&huart1, &send.rece_data, 5);
  while (1) {
    vTaskDelay(1);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
  if (huart->Instance == USART1) {
    HAL_UART_Transmit_IT(&huart1, &send.rece_data, 5);
  }
  HAL_UART_Receive_IT(&huart1, &send.rece_data, 5);
}
}
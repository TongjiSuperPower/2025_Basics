#include "cmsis_os.h"
#include "usart.h"

#pragma pack(1)  // ×Ö½Ú¶ÔÆë
struct PlotFrame
{
  uint8_t start[2] = {0xAA, 0xBB};
  uint8_t size;
  float data;
};
#pragma pack()

PlotFrame plot_frame;

uint8_t i = 0;
extern "C" void uart_send_task()
{
  while (1) {
    plot_frame.size = 4 * 1;
    plot_frame.data = i;

    HAL_UART_Transmit_DMA(
      &huart1, (uint8_t *)&plot_frame,
      sizeof(plot_frame.start) + sizeof(plot_frame.size) + plot_frame.size);

    i = i + 1;
    if (i >= 5) i = 0;

    vTaskDelay(10);
  }
}
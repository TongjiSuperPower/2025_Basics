#include "cmsis_os.h"
#include "dbus.hpp"
#include "usart.h"

#pragma pack(1)
struct PlotFrame
{
  uint8_t start[2] = {0xAA, 0xBB};
  uint8_t size;
  float data[6];
};
#pragma pack()

PlotFrame plot_frame;

extern Dbus remote;

extern "C" void uart_send_task(void * pvParameters)
{
  vTaskDelay(1000);
  remote_restart();

  while (1) {
    plot_frame.size = 4 * 4;
    plot_frame.data[0] = remote.stick_lv;
    plot_frame.data[1] = remote.stick_lh;
    plot_frame.data[2] = remote.stick_rv;
    plot_frame.data[3] = remote.stick_rh;

    HAL_UART_Transmit_DMA(
      &huart1, (uint8_t *)&plot_frame,
      sizeof(plot_frame.start) + sizeof(plot_frame.size) + plot_frame.size);

    vTaskDelay(10);
  }
}
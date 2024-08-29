#include "usart.h"

enum class DBusSwitchMode
{
  DOWN,
  MID,
  UP
};

struct Dbus
{
  float stick_rh;  // [-1, 1] right horizontal
  float stick_rv;  // [-1, 1] right vertical
  float stick_lh;  // [-1, 1] left horizontal
  float stick_lv;  // [-1, 1] left vertical

  DBusSwitchMode switch_r;
  DBusSwitchMode switch_l;
} remote;

constexpr size_t DBUS_BUFF_SIZE = 18;
uint8_t buff[DBUS_BUFF_SIZE];

float get_stick(uint16_t raw) { return (static_cast<int16_t>(raw) - 1024) / 660.0f; }

DBusSwitchMode get_switch(uint8_t raw)
{
  if (raw == 1)
    return DBusSwitchMode::UP;
  else if (raw == 3)
    return DBusSwitchMode::MID;
  else
    return DBusSwitchMode::DOWN;
}

extern "C" void remote_restart()
{
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, buff, DBUS_BUFF_SIZE);
  __HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);
}

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef * huart, uint16_t Size)
{
  if (huart == &huart3) {
    remote.stick_rh = get_stick((buff[0] | (buff[1] << 8)) & 0x07ff);
    remote.stick_rv = get_stick(((buff[1] >> 3) | (buff[2] << 5)) & 0x07ff);
    remote.stick_lh = get_stick(((buff[2] >> 6) | (buff[3] << 2) | (buff[4] << 10)) & 0x07ff);
    remote.stick_lv = get_stick(((buff[4] >> 1) | (buff[5] << 7)) & 0x07ff);

    remote.switch_r = get_switch((buff[5] >> 4) & 0x0003);
    remote.switch_l = get_switch(((buff[5] >> 4) & 0x000C) >> 2);

    remote_restart();
  }
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
  if (huart == &huart3) {
    remote_restart();
  }
}
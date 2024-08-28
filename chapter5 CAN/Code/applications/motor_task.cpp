#include "cmsis_os.h"
#include "main.h"
#include "motor_protocol/rm_motor/rm_motor.hpp"

motor_protocol::RM_Motor motor_6020;
extern CAN_HandleTypeDef hcan1;

extern "C" {

// 电机任务
void motor_task()
{
  while (1) {
    motor_6020.motor_cmd(&hcan1, 0x1FF, 2000, 2000, 2000, 2000);
    vTaskDelay(1);
  }
}

// CAN回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
  CAN_RxHeaderTypeDef rx_header;
  uint8_t rx_data[8];

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

  if (rx_header.StdId == 0x205) {
    motor_6020.decode_motor_measure(0, rx_data);
  }
}
}

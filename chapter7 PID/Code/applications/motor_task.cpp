#include "cmsis_os.h"
#include "controller/pid.hpp"
#include "main.h"
#include "motor_protocol/rm_motor/rm_motor.hpp"

extern CAN_HandleTypeDef hcan1;
motor_protocol::RM_Motor motor_6020;

constexpr double PI = 3.14159265357;

static const float motor_6020_pid_config[3] = {1000.0f, 2.0f, 100.0f};
static const float motor_6020_maxout = 16000.0f;
static const float motor_6020_maxiout = 1000.0f;
static const float motor_6020_alpha = 0.1f;

controller::PID pid_6020(
  controller::PIDMode::POSITION, motor_6020_pid_config, motor_6020_maxout, motor_6020_maxiout,
  motor_6020_alpha);

float pos_set = 3.0f;
int num = 0;
float angle, give_current;

extern "C" {

// 电机任务
void motor_task()
{
  while (1) {
    angle = (motor_6020.get_ecd(0) / 4096.f) * PI;
    if (pos_set > 2 * PI && angle < PI)
      pos_set -= 2 * PI;
    else if (pos_set > 1.5f * PI && angle < PI)
      angle += 2 * PI;

    give_current = pid_6020.pid_calc(pos_set, angle);
    motor_6020.motor_cmd(&hcan1, 0X1FE, give_current, 0, 0, 0);

    num++;
    if (num >= 3000) {
      pos_set = angle + PI / 2;
      num = 0;
    }

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

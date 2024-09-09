#include "can.h"
#include "cmsis_os.h"
#include "pid.hpp"

struct motor_t
{
  uint16_t angle_ecd;
  int16_t speed_rpm;
  int16_t current_raw;
  int16_t temperate;
};

CAN_TxHeaderTypeDef motor_tx_message;
uint8_t motor_can_send_data[8];

HAL_StatusTypeDef cmd_motor(
  uint32_t stdid, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
  uint32_t send_mail_box;
  motor_tx_message.StdId = stdid;
  motor_tx_message.IDE = CAN_ID_STD;
  motor_tx_message.RTR = CAN_RTR_DATA;
  motor_tx_message.DLC = 0x08;

  motor_can_send_data[0] = motor1 >> 8;
  motor_can_send_data[1] = motor1;
  motor_can_send_data[2] = motor2 >> 8;
  motor_can_send_data[3] = motor2;
  motor_can_send_data[4] = motor3 >> 8;
  motor_can_send_data[5] = motor3;
  motor_can_send_data[6] = motor4 >> 8;
  motor_can_send_data[7] = motor4;

  return HAL_CAN_AddTxMessage(&hcan1, &motor_tx_message, motor_can_send_data, &send_mail_box);
}

extern "C" void can_filter_init(void)
{
  CAN_FilterTypeDef can_filter_st;
  can_filter_st.FilterActivation = ENABLE;
  can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
  can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
  can_filter_st.FilterIdHigh = 0x0000;
  can_filter_st.FilterIdLow = 0x0000;
  can_filter_st.FilterMaskIdHigh = 0x0000;
  can_filter_st.FilterMaskIdLow = 0x0000;
  can_filter_st.FilterBank = 0;
  can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
  HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
  HAL_CAN_Start(&hcan1);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

constexpr double PI = 3.14159265357;

motor_t motor_6020;
pid motor_6020_pid;
static const float motor_6020_control_frenquency = 0.001f;
static const float motor_6020_kp = 3000.0f;
static const float motor_6020_ki = 20.0f;
static const float motor_6020_kd = 100.0f;
static const float motor_6020_maxout = 16000.0f;
static const float motor_6020_maxiout = 2000.0f;

float pos_set = 3.0f;
int num = 0;
float angle, give_current;

extern "C" void motor_task(void const * argument)
{
  can_filter_init();
  pid_init(
    &motor_6020_pid, motor_6020_control_frenquency, motor_6020_kp, motor_6020_ki, motor_6020_kd,
    motor_6020_maxout, motor_6020_maxiout);

  while (1) {
    num++;
    if (num >= 1000) {
      pos_set = angle + PI / 2;
      num = 0;
    }

    angle = (motor_6020.angle_ecd / 4096.f) * PI;

    if (pos_set > 2 * PI && angle < PI)
      pos_set -= 2 * PI;
    else if (pos_set > 1.5f * PI && angle < PI)
      angle += 2 * PI;

    give_current = pid_calc(&motor_6020_pid, pos_set, angle);
    cmd_motor(0X1FF, give_current, give_current, give_current, give_current);

    osDelay(1);
  }
}

void decode_motor_measure(motor_t * motor, uint8_t * data)
{
  motor->angle_ecd = (data[0] << 8) | data[1];
  motor->speed_rpm = (data[2] << 8) | data[3];
  motor->current_raw = (data[4] << 8) | data[5];
  motor->temperate = data[6];
  return;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
  CAN_RxHeaderTypeDef rx_header;
  uint8_t rx_data[8];

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

  if (rx_header.StdId == 0x208) {
    decode_motor_measure(&motor_6020, rx_data);
  }
}

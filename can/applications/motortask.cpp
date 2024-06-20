#include "motortask.h"

#include "cmsis_os.h"
#include "main.h"

extern "C" {

extern CAN_HandleTypeDef hcan1;
static CAN_TxHeaderTypeDef motor_tx_message;
static uint8_t motor_can_send_data[8];

Motor::Motor() {}

Motor::~Motor() {}

motor_measure_t * Motor::get_motor_measure() { return &motor_meature; }

void Motor::motor_cmd(int16_t motor)
{
  uint32_t send_mail_box;
  motor_tx_message.StdId = 0x200;
  motor_tx_message.IDE = CAN_ID_STD;
  motor_tx_message.RTR = CAN_RTR_DATA;
  motor_tx_message.DLC = 0x08;
  motor_can_send_data[0] = (motor >> 8);
  motor_can_send_data[1] = motor;
  motor_can_send_data[2] = 0;
  motor_can_send_data[3] = 0;
  motor_can_send_data[4] = 0;
  motor_can_send_data[5] = 0;
  motor_can_send_data[6] = 0;
  motor_can_send_data[7] = 0;
  HAL_CAN_AddTxMessage(&hcan1, &motor_tx_message, motor_can_send_data, &send_mail_box);
}

void Motor::decode_motor_measure(motor_measure_t * ptr, uint8_t data[8])
{
  (ptr)->last_ecd = (ptr)->ecd;
  (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);
  (ptr)->speed_rpm = (int16_t)((data)[2] << 8 | (data)[3]);
  (ptr)->given_current = (int16_t)((data)[4] << 8 | (data)[5]);
  (ptr)->temperate = (uint16_t)(data)[6];
}

// -------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------- //

Motor motor_6020;
void motor_task()
{
  while (1) {
    motor_6020.motor_cmd(200);
    vTaskDelay(1);
  }
}

// -------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------- //

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
  CAN_RxHeaderTypeDef rx_header;
  uint8_t rx_data[8];

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

  if (rx_header.StdId == 0x201) {
    motor_6020.decode_motor_measure(motor_6020.get_motor_measure(), rx_data);
  }
}
}

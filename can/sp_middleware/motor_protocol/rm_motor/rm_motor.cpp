#include "rm_motor.hpp"

rm_motor::rm_motor() {}

rm_motor::~rm_motor() {}

extern CAN_HandleTypeDef hcan1;
// extern CAN_HandleTypeDef hcan2;

HAL_StatusTypeDef rm_motor::motor_cmd(
  uint8_t can_id, uint32_t stdid, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
  uint32_t send_mail_box;
  motor_tx_message_.StdId = stdid;
  motor_tx_message_.IDE = CAN_ID_STD;
  motor_tx_message_.RTR = CAN_RTR_DATA;
  motor_tx_message_.DLC = 0x08;
  motor_can_send_data_[0] = motor1 >> 8;
  motor_can_send_data_[1] = motor1;
  motor_can_send_data_[2] = motor2 >> 8;
  motor_can_send_data_[3] = motor2;
  motor_can_send_data_[4] = motor3 >> 8;
  motor_can_send_data_[5] = motor3;
  motor_can_send_data_[6] = motor4 >> 8;
  motor_can_send_data_[7] = motor4;
  if (can_id == 1)
    return HAL_CAN_AddTxMessage(&hcan1, &motor_tx_message_, motor_can_send_data_, &send_mail_box);
  // else if (can_id == 2)
  //   return HAL_CAN_AddTxMessage(&hcan2, &motor_tx_message_, motor_can_send_data_, &send_mail_box);
  return HAL_ERROR;
}

void rm_motor::decode_motor_measure(uint8_t motor_id, uint8_t data[8])
{
  motor_measure_[motor_id].last_ecd = motor_measure_[motor_id].ecd;
  motor_measure_[motor_id].ecd = (uint16_t)((data)[0] << 8 | (data)[1]);
  motor_measure_[motor_id].speed_rpm = (int16_t)((data)[2] << 8 | (data)[3]);
  motor_measure_[motor_id].given_current = (int16_t)((data)[4] << 8 | (data)[5]);
  motor_measure_[motor_id].temperate = (uint8_t)(data)[6];
  return;
}
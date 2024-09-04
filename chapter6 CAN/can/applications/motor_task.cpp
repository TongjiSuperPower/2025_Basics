#include "can.h"
#include "cmsis_os.h"

struct motor_t
{
  uint16_t angle_ecd;
  int16_t speed_rpm;
  int16_t current_raw;
  int16_t temperate;
};

motor_t motor_6020;

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

extern "C" void motor_task(void const * argument)
{
  can_filter_init();
  while (1) {
    cmd_motor(0X1FF, 2000, 2000, 2000, 2000);
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

  if (rx_header.StdId == 0x205) {
    decode_motor_measure(&motor_6020, rx_data);
  }
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
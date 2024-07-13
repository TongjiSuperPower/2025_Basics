#ifndef _RM_MOTOR_HPP_
#define _RM_MOTOR_HPP_

#include "cstdint"
#include "main.h"

namespace motor_protocol
{
class RM_Motor
{
private:
  // �������
  static const uint8_t RM_MOTOR_NUM = 1;
  // �������
  struct motor_measure_t
  {
    uint16_t ecd;           // ���������λ��
    int16_t speed_rpm;      // ���ת�٣�ת/���ӣ�
    int16_t given_current;  // ���ʵ�ʵ���
    uint8_t temperate;      // ����¶�
  } motor_measure_[RM_MOTOR_NUM];
  // ���CAN��������
  CAN_TxHeaderTypeDef motor_tx_message_;
  uint8_t motor_can_send_data_[8];

public:
  RM_Motor() {}
  ~RM_Motor() {}
  // ���͵������ָ��
  HAL_StatusTypeDef motor_cmd(
    CAN_HandleTypeDef * hcan, uint32_t stdid, int16_t motor1, int16_t motor2, int16_t motor3,
    int16_t motor4);
  // ����������
  void decode_motor_measure(uint8_t motor_id, uint8_t data[8]);
};

}  // namespace motor_protocol

#endif  // _RM_MOTOR_HPP_
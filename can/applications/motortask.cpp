#include "motortask.h"
#include "main.h"
#include "cmsis_os.h"

extern "C"
{

extern CAN_HandleTypeDef hcan1;
static CAN_TxHeaderTypeDef  motor_tx_message;
static uint8_t              motor_can_send_data[8];


Motor::Motor()
{}

Motor::~Motor()
{}

motor_measure_t* Motor::get_motor_measure()
{
    return &motor_meature;
}

void Motor::motor_cmd(int16_t motor)
{
    uint32_t send_mail_box;
    motor_tx_message.StdId = 0x1ff;
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


void motor_task()
{
    Motor motor_6020;
    while(1)
    {
        motor_6020.motor_cmd(2000);

        vTaskDelay(1);
    }
}

}



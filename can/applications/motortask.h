#include <cstdint>
#include "main.h"
#ifndef MOTOR_TASK_H
#define MOTOR_TASK_H

extern "C"
{

typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
	
} motor_measure_t;


class Motor
{
public:
    Motor();
    ~Motor();
    void motor_cmd(int16_t motor);
    motor_measure_t *get_motor_measure();
    void decode_motor_measure(motor_measure_t *ptr, uint8_t data[8]);

private:

    motor_measure_t motor_meature;

};


void motor_task();

}
#endif // MOTOR_TASK_H



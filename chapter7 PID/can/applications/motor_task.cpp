#include "can.h"
#include "cmsis_os.h"
#include "pid.hpp"
#include "motor_info.hpp"


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

extern HAL_StatusTypeDef cmd_motor(
  uint32_t stdid, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

extern "C" void motor_task(void const * argument)
{
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

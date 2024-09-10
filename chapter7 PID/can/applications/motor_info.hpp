#include "cstdint"

struct motor_t
{
  uint16_t angle_ecd;
  int16_t speed_rpm;
  int16_t current_raw;
  int16_t temperate;
};
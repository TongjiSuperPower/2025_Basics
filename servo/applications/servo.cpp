#include "servo.h"

#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

void Servo_rotate()
{
  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 25);
  HAL_Delay(1000);
  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 50);
  HAL_Delay(1000);
}

#ifdef __cplusplus
}
#endif

#include "tim.h"

extern "C" void servo_rotate()
{
  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 25);
  HAL_Delay(1000);
  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 50);
  HAL_Delay(1000);
}
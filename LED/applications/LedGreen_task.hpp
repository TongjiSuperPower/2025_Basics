#ifndef LED_GREEN_TASK_HPP
#define LED_GREEN_TASK_HPP

class LedGreen
{
public:
  LedGreen();
  ~LedGreen();
};

extern "C" {
void Led_Green_Task(void const * argument);
}

#endif
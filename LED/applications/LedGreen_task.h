#ifndef LED_GREEN_TASK_H
#define LED_GREEN_TASK_H

extern "C" {

class LedGreen
{
public:
  LedGreen();
  ~LedGreen();

};


void Led_Green_Task(void const * argument);
}

#endif
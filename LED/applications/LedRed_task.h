#ifndef LED_RED_TASK_H
#define LED_RED_TASK_H



extern "C"
{
class LedRed
{
public:
  LedRed();
  ~LedRed();

};


void Led_Red_Task(void const * argument);

}


#endif
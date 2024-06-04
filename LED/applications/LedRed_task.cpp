


#include "LedRed_task.h"
#include "gpio.h"
#include "main.h"

#include "cmsis_os.h"

extern "C"
{

LedRed::LedRed()
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12,GPIO_PIN_SET); 
}

LedRed::~LedRed()
{}



void LED_Red_Task(void const * argument)
{
    
    LedRed LedRed;
    while(true)
    {
        HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_12); 
        vTaskDelay(300); 
    }

}


}
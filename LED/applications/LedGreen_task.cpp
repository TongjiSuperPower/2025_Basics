


#include "LedGreen_task.h"
#include "gpio.h"
#include "main.h"

#include "cmsis_os.h"


extern "C"
{

LedGreen::LedGreen()
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11,GPIO_PIN_SET); 
}

LedGreen::~LedGreen()
{}



void LED_Green_Task(void const * argument)
{
    
    LedGreen LedGreen;
    while(true)
    {
        HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_11); 
        vTaskDelay(200); 
    }

}

}


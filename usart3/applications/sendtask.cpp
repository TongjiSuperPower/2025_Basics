#include "sendtask.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"

extern "C"
{

Send::Send()
{
    Send::send_data = 0;
}
Send::~Send()
{}


Send send;

void send_task()
{
    HAL_UART_Receive_DMA(&huart1,&send.rece_data,5);
    while(1)
    {
        vTaskDelay(1);
    }

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        HAL_UART_Transmit_DMA(&huart1, &send.rece_data, 5);
    }
    HAL_UART_Receive_DMA(&huart1,&send.rece_data,5);

}


}




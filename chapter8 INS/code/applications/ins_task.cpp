

extern "C" 
{

#include "ins_task.h"
#include "cmsis_os.h"
#include "main.h"


extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
static TaskHandle_t INS_task_local_handler;

INS::INS()
{
}

INS::~INS()
{
}

void INS::SPI1_DMA_init(uint32_t tx_buf, uint32_t rx_buf, uint16_t num)
{
    SET_BIT(hspi1.Instance->CR2, SPI_CR2_TXDMAEN);
    SET_BIT(hspi1.Instance->CR2, SPI_CR2_RXDMAEN);

    __HAL_SPI_ENABLE(&hspi1);


    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_spi1_rx);
    
    while(hdma_spi1_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_rx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_rx, DMA_LISR_TCIF2);

    hdma_spi1_rx.Instance->PAR = (uint32_t) & (SPI1->DR);
    //memory buffer 1
    //内存缓冲区1
    hdma_spi1_rx.Instance->M0AR = (uint32_t)(rx_buf);
    //data length
    //数据长度
    __HAL_DMA_SET_COUNTER(&hdma_spi1_rx, num);

    __HAL_DMA_ENABLE_IT(&hdma_spi1_rx, DMA_IT_TC);


    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_spi1_tx);
    
    while(hdma_spi1_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_tx);
    }


    __HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_LISR_TCIF3);

    hdma_spi1_tx.Instance->PAR = (uint32_t) & (SPI1->DR);
    //memory buffer 1
    //内存缓冲区1
    hdma_spi1_tx.Instance->M0AR = (uint32_t)(tx_buf);
    //data length
    //数据长度
    __HAL_DMA_SET_COUNTER(&hdma_spi1_tx, num);

}


void INS::SPI1_DMA_enable(uint32_t tx_buf, uint32_t rx_buf, uint16_t ndtr)
{
    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_spi1_rx);
    __HAL_DMA_DISABLE(&hdma_spi1_tx);
    while(hdma_spi1_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_rx);
    }
    while(hdma_spi1_tx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_spi1_tx);
    }
    //clear flag
    //清除标志位
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_HT_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_TE_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_DME_FLAG_INDEX(hspi1.hdmarx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmarx, __HAL_DMA_GET_FE_FLAG_INDEX(hspi1.hdmarx));

    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_HT_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_TE_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_DME_FLAG_INDEX(hspi1.hdmatx));
    __HAL_DMA_CLEAR_FLAG (hspi1.hdmatx, __HAL_DMA_GET_FE_FLAG_INDEX(hspi1.hdmatx));
    //set memory address
    //设置数据地址
    hdma_spi1_rx.Instance->M0AR = rx_buf;
    hdma_spi1_tx.Instance->M0AR = tx_buf;
    //set data length
    //设置数据长度
    __HAL_DMA_SET_COUNTER(&hdma_spi1_rx, ndtr);
    __HAL_DMA_SET_COUNTER(&hdma_spi1_tx, ndtr);
    //enable DMA
    //使能DMA
    __HAL_DMA_ENABLE(&hdma_spi1_rx);
    __HAL_DMA_ENABLE(&hdma_spi1_tx);
}



INS ins;
void ins_task(void const *pvParameters)
{

    //wait a time
    osDelay(INS_TASK_INIT_TIME);
    while(BMI088_init())
    {
        osDelay(100);
    }
    BMI088_read(ins.bmi088_real_data.gyro, ins.bmi088_real_data.accel, &ins.bmi088_real_data.temp);
    //get the handle of task
    //获取当前任务的任务句柄，
    INS_task_local_handler = xTaskGetHandle(pcTaskGetName(NULL));

    //set spi frequency
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }


    ins.SPI1_DMA_init((uint32_t)ins.gyro_dma_tx_buf, (uint32_t)ins.gyro_dma_rx_buf, SPI_DMA_GYRO_LENGHT);

    ins.imu_start_dma_flag = 1;


    while(1)
    {
        while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)
        {
        }


        if(ins.gyro_update_flag & (1 << IMU_NOTIFY_SHFITS))
        {
            ins.gyro_update_flag &= ~(1 << IMU_NOTIFY_SHFITS);
            BMI088_gyro_read_over(ins.gyro_dma_rx_buf + BMI088_GYRO_RX_BUF_DATA_OFFSET, ins.bmi088_real_data.gyro);
        }

        if(ins.accel_update_flag & (1 << IMU_UPDATE_SHFITS))
        {
            ins.accel_update_flag &= ~(1 << IMU_UPDATE_SHFITS);
            BMI088_accel_read_over(ins.accel_dma_rx_buf + BMI088_ACCEL_RX_BUF_DATA_OFFSET, ins.bmi088_real_data.accel, &ins.bmi088_real_data.time);
        }
    }


}




static void imu_cmd_spi_dma(void)
{
    UBaseType_t uxSavedInterruptStatus;
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

    //开启陀螺仪的DMA传输
    if( (ins.gyro_update_flag & (1 << IMU_DR_SHFITS) ) && !(hspi1.hdmatx->Instance->CR & DMA_SxCR_EN) && !(hspi1.hdmarx->Instance->CR & DMA_SxCR_EN)
    && !(ins.accel_update_flag & (1 << IMU_SPI_SHFITS)))
    {
        ins.gyro_update_flag &= ~(1 << IMU_DR_SHFITS);
        ins.gyro_update_flag |= (1 << IMU_SPI_SHFITS);

        HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
        ins.SPI1_DMA_enable((uint32_t)ins.gyro_dma_tx_buf, (uint32_t)ins.gyro_dma_rx_buf, SPI_DMA_GYRO_LENGHT);
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
        return;
    }
    //开启加速度计的DMA传输
    if((ins.accel_update_flag & (1 << IMU_DR_SHFITS)) && !(hspi1.hdmatx->Instance->CR & DMA_SxCR_EN) && !(hspi1.hdmarx->Instance->CR & DMA_SxCR_EN)
    && !(ins.gyro_update_flag & (1 << IMU_SPI_SHFITS)))
    {
        ins.accel_update_flag &= ~(1 << IMU_DR_SHFITS);
        ins.accel_update_flag |= (1 << IMU_SPI_SHFITS);

        HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
        ins.SPI1_DMA_enable((uint32_t)ins.accel_dma_tx_buf, (uint32_t)ins.accel_dma_rx_buf, SPI_DMA_ACCEL_LENGHT);
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
        return;
    }
    

    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}







void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == INT1_ACCEL_Pin)
    {
        ins.accel_update_flag |= 1 << IMU_DR_SHFITS;
        if(ins.imu_start_dma_flag)
        {
            imu_cmd_spi_dma();
        }
    }
    else if(GPIO_Pin == INT1_GYRO_Pin)
    {
        ins.gyro_update_flag |= 1 << IMU_DR_SHFITS;
        if(ins.imu_start_dma_flag)
        {
            imu_cmd_spi_dma();
        }
    }
    
    else if(GPIO_Pin == GPIO_PIN_0)
    {
        //wake up the task
        //唤醒任务
        if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
        {
            static BaseType_t xHigherPriorityTaskWoken;
            vTaskNotifyGiveFromISR(INS_task_local_handler, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }


}




void DMA2_Stream2_IRQHandler(void)
{

    if(__HAL_DMA_GET_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx)) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx));

        //gyro read over
        //陀螺仪读取完毕
        if(ins.gyro_update_flag & (1 << IMU_SPI_SHFITS))
        {
            ins.gyro_update_flag &= ~(1 << IMU_SPI_SHFITS);
            ins.gyro_update_flag |= (1 << IMU_UPDATE_SHFITS);

            HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
            
        }

        //accel read over
        //加速度计读取完毕
        if(ins.accel_update_flag & (1 << IMU_SPI_SHFITS))
        {
            ins.accel_update_flag &= ~(1 << IMU_SPI_SHFITS);
            ins.accel_update_flag |= (1 << IMU_UPDATE_SHFITS);

            HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
        }

        imu_cmd_spi_dma();

        if(ins.gyro_update_flag & (1 << IMU_UPDATE_SHFITS))
        {
            ins.gyro_update_flag &= ~(1 << IMU_UPDATE_SHFITS);
            ins.gyro_update_flag |= (1 << IMU_NOTIFY_SHFITS);
            __HAL_GPIO_EXTI_GENERATE_SWIT(GPIO_PIN_0);
        }
    }
}








}

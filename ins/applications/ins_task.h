#ifndef INS_TASK_H
#define INS_TASK_H

extern "C"
{

#include "struct_typedef.h"
#include "BMI088driver.h"
#include <cstdint>

#define SPI_DMA_GYRO_LENGHT       8
#define SPI_DMA_ACCEL_LENGHT      9

#define IMU_DR_SHFITS        0
#define IMU_SPI_SHFITS       1
#define IMU_UPDATE_SHFITS    2
#define IMU_NOTIFY_SHFITS    3

#define BMI088_GYRO_RX_BUF_DATA_OFFSET  1
#define BMI088_ACCEL_RX_BUF_DATA_OFFSET 2

#define INS_TASK_INIT_TIME 7 //任务开始初期 delay 一段时间


class INS
{
public:
    INS();
    ~INS();
    void SPI1_DMA_init(uint32_t tx_buf, uint32_t rx_buf, uint16_t num);
    void SPI1_DMA_enable(uint32_t tx_buf, uint32_t rx_buf, uint16_t ndtr);

    uint8_t gyro_dma_rx_buf[SPI_DMA_GYRO_LENGHT];
    uint8_t gyro_dma_tx_buf[SPI_DMA_GYRO_LENGHT] = {0x82,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

    uint8_t accel_dma_rx_buf[SPI_DMA_ACCEL_LENGHT];
    uint8_t accel_dma_tx_buf[SPI_DMA_ACCEL_LENGHT] = {0x92,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


    volatile uint8_t gyro_update_flag = 0;
    volatile uint8_t accel_update_flag = 0;
    volatile uint8_t imu_start_dma_flag = 0;

    bmi088_real_data_t bmi088_real_data;

};

extern void ins_task(void const *pvParameters);






}

#endif // INS_TASK_H
#include "stm32f1xx_hal.h"
#include "main.h"
#include "pn532_stm32f1.h"

#define _SPI_STATREAD                   0x02
#define _SPI_DATAWRITE                  0x01
#define _SPI_DATAREAD                   0x03
#define _SPI_READY                      0x01

#define _SPI_TIMEOUT                    10


extern SPI_HandleTypeDef hspi1;

/**************************************************************************
 * RESET
 **************************************************************************/
int PN532_Reset(void) {
    //HAL_GPIO_WritePin(PN532_RST_GPIO_Port, PN532_RST_Pin, GPIO_PIN_SET); //hero
    HAL_Delay(100);
    //HAL_GPIO_WritePin(PN532_RST_GPIO_Port, PN532_RST_Pin, GPIO_PIN_RESET); //hero
    HAL_Delay(500);
    //HAL_GPIO_WritePin(PN532_RST_GPIO_Port, PN532_RST_Pin, GPIO_PIN_SET); // hero
    HAL_Delay(100);
    return PN532_STATUS_OK;
}
/**************************************************************************
 * INIT
 **************************************************************************/
void PN532_Init(PN532* pn532) {
    PN532_SPI_Init(pn532);
}

/**************************************************************************
 * SPI READ/WRITE
 **************************************************************************/
void spi_rw(uint8_t* data, uint8_t count) {
    HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_SPI_TransmitReceive(&hspi1, data, data, count, _SPI_TIMEOUT);
    HAL_Delay(1);
    HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_SET);
}
/**************************************************************************
 *PN532 SPI READ
 **************************************************************************/
int PN532_SPI_ReadData(uint8_t* data, uint16_t count) {
    uint8_t frame[count + 1];
    frame[0] = _SPI_DATAREAD;
    HAL_Delay(5);
    spi_rw(frame, count + 1);
    for (uint8_t i = 0; i < count; i++) {
        data[i] = frame[i + 1];
    }
    return PN532_STATUS_OK;
}
/**************************************************************************
 *PN532 SPI WRITE
 **************************************************************************/
int PN532_SPI_WriteData(uint8_t *data, uint16_t count) {
    uint8_t frame[count + 1];
    frame[0] = _SPI_DATAWRITE;
    for (uint8_t i = 0; i < count; i++) {
        frame[i + 1] = data[i];
    }
    spi_rw(frame, count + 1);
    return PN532_STATUS_OK;
}
/**************************************************************************
 *PN532 SPI WAIT READY
 **************************************************************************/
bool PN532_SPI_WaitReady(uint32_t timeout) {
    uint8_t status[] = {_SPI_STATREAD, 0x00};
    uint32_t tickstart = HAL_GetTick();
    while (HAL_GetTick() - tickstart < timeout) {
        HAL_Delay(10);
        spi_rw(status, sizeof(status));
        if (status[1] == _SPI_READY) {
            return true;
        } else {
            HAL_Delay(5);
        }
    }
    return false;
}
/**************************************************************************
 *PN532 SPI WAKEUP
 **************************************************************************/
int PN532_SPI_Wakeup(void) {
    // Send any special commands/data to wake up PN532
    uint8_t data[] = {0x00};
    HAL_Delay(1000);
    HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_RESET);
    HAL_Delay(2); // T_osc_start
    spi_rw(data, 1);
    HAL_Delay(1000);
    return PN532_STATUS_OK;
}
/**************************************************************************
 *PN532 SPI INIT
 **************************************************************************/
void PN532_SPI_Init(PN532* pn532) {
    // init the pn532 functions
    pn532->reset =  PN532_Reset;
    pn532->read_data = PN532_SPI_ReadData;
    pn532->write_data = PN532_SPI_WriteData;
    pn532->wait_ready = PN532_SPI_WaitReady;
    pn532->wakeup = PN532_SPI_Wakeup;
    //hardware wakeup
    pn532->wakeup();
}
/**************************************************************************
 * End: SPI
 **************************************************************************/

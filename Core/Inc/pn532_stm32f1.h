/*
 *  File name: pn532.h
 *  File type: H
 *  Created on: May 7, 2023
 *  Author: Phan Van Hoang
 */
#ifndef PN532_STM32F1
#define PN532_STM32F1

#include "pn532.h"

void PN532_Init(PN532* dev);
int PN532_Reset(void);

int PN532_SPI_ReadData(uint8_t* data, uint16_t count);
int PN532_SPI_WriteData(uint8_t *data, uint16_t count);
bool PN532_SPI_WaitReady(uint32_t timeout);
int PN532_SPI_Wakeup(void);
void PN532_SPI_Init(PN532* dev);

#endif  /* PN532_STM32F1 */

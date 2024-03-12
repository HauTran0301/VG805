/*
 * Dwin.h
 *
 *  Created on: Dec 31, 2021
 *      Author: User
 */

#ifndef SRC_DWIN_H_
#define SRC_DWIN_H_
#include "AllHeader.h"
#ifdef _DWIN_SEND_

int Dwin_Write_VP(uint16_t Addr,uint8_t *data,uint16_t len);
uint8_t dwinWriteData(uint16_t Addr,uint8_t *data,uint16_t len);

#else
extern int Dwin_Write_VP(uint16_t Addr,uint8_t *data,uint16_t len);
extern uint8_t dwinWriteData(uint16_t Addr,uint8_t *data,uint16_t len);

#endif
#endif /* SRC_DWIN_H_ */

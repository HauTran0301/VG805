/*
 * Dwin_App.h
 *
 *  Created on: Dec 31, 2021
 *      Author: User
 */

#ifndef SRC_DWIN_APP_H_
#define SRC_DWIN_APP_H_

#include "AllHeader.h"

#define DATE_VP 0x1006
#define DATE_VP_SIZE 8

/** ------------- **/
#define DWIN_POSITION 1

#define FLOORNAME_VP_TEN_MAIN 0x1000
#define FLOORNAME_SP_TEN_MAIN 0x5000
#define FLOORNAME_VP_TEN 0x1002
#define FLOORNAME_SP_TEN 0x5020
#define FLOORNAME_VP_UNIT 0x1004
#define FLOORNAME_SP_UNIT 0x5040

#define FLOORNAME_VP_SIZE 8
#define WEEKDAY_VP 0x1010
#define WEEKDAY_VP_SIZE 10
#define TIME_VP 0x1002
#define TIME_VP_SIZE 6

#define ARROW_ICON_SP 0x5200

#define ARROW_UP_STOP 122
#define ARROW_UP_START 0
#define ARROW_UP_END 59


#define ARROW_DOWN_STOP 122
#define ARROW_DOWN_START 60
#define ARROW_DOWN_END 119

#define ARROW_STOP 122

#define TOTALFLOORNAME_VP 0x1300
#define TOTALOORNAME_FLASH_ADDR 0x000002
#define TOTALFLOORNAME_LENGH 60

/* Roll Text */
#define SP_ROLLTEXT 0x5300
#define VP_ROLLTEXT 0x1100

/* Icon WARNING Variable Address*/

#define ICON_WARNING    0x1020
#define ICON_WARNING_SP 0x5080

/* Message Variable Address*/
#define MESS_WARNING    0x1010
#define MESS_WARNING_SP 0x5060

/* Floor name */
#define DEAFAULT_FLOOR_NAME_X_TEN_MAIN 42
#define DEAFAULT_FLOOR_NAME_Y_TEN_MAIN 330
#define DEAFAULT_FLOOR_NAME_X_TEN 140
#define DEAFAULT_FLOOR_NAME_Y_TEN 330
#define DEAFAULT_FLOOR_NAME_X_UINT 270
#define DEAFAULT_FLOOR_NAME_Y_UNIT 330

/* Icon and logo */
#define DEAFAULT_ICON_X 30
#define DEAFAULT_ICON_Y 694


/* Height elevator */
#define POSITION_X 15
#define POSITION_Y 433

#define MAX_PAGE 5


#ifdef _DWIN_C_
typedef struct
{
	char Data[50];
	uint8_t IsChange;
	uint8_t IsActive;
	uint8_t DataSize;
	uint32_t Timeshow;	// show time
	uint32_t DelayTime; // time delay to show

} _Message;

typedef enum 
{
	fireIcon = 0,
	overloadIcon,
	fullloadIcon,
	outOfServiceIcon,
	inspectionIcon,
	emergencyIcon,
	vipRunIcon,
	parkingIcon,
	attendantIcon,
	normalLogo = 10,
	hideModeIcon = 0xFF

}_IconShow_e;

typedef enum 
{
	normalMode = 1,
	hideMode
}messageMode_e;

uint8_t displayMessageA = 0;
uint8_t displayMessageB = 0;
uint16_t volatile display_message = 0;
/* Message for show */
char  Textmessage[27]       = "Welcome To Zeit Elevator!";



void audioFun(void);
void changLocationHeight(uint16_t add);
void dwinSendSpeed(uint16_t add, uint8_t *pData, uint8_t len);
void dwinSendPosition(uint16_t add, uint8_t *pData, uint8_t len);
void Dwin_Change_Current_FloorName(void);
void Dwin_switch_to_next_page(void);
void Dwin_update_time(void);
void DWin_Calltable_Process(void);
void DWIN_Arrow_Process(void);
void DWIN_Message_Process(void);
void DWIN_add_key(uint8_t key);
void DWIN_show_message(char *Message, uint32_t time);
void DWIN_Reset_show_message(void);
void dwinUpdateImageFloor(uint16_t spAddressUint, uint16_t spAddressTen);
#else


extern uint8_t displayMessageA ;
extern uint8_t displayMessageB ;
extern uint16_t volatile display_message;
extern void audioFun(void);
extern void dwinUpdateImageFloor(uint16_t spAddressUint, uint16_t spAddressTen);


extern void dwinSendSpeed(uint16_t add, uint8_t *pData, uint8_t len);
extern void dwinSendPosition(uint16_t add, uint8_t *pData, uint8_t len);
extern void Dwin_Change_Current_FloorName(void);
extern void Dwin_switch_to_next_page(void);
extern void Dwin_update_time(void);
extern void DWin_Calltable_Process(void);
extern void DWIN_Arrow_Process(void);
extern void DWIN_Message_Process(void);
extern void DWIN_add_key(uint8_t key);
extern void DWIN_show_message(char *Message, uint32_t time);
extern void DWIN_Reset_show_message(void);

#endif
#endif /* SRC_DWIN_APP_H_ */

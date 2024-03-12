/*
 * Dwin_App.c
 *
 *  Created on: Dec 31, 2021
 *      Author: User
 */

#define _DWIN_C_

#include "Dwin_App.h"

extern uint8_t Callstatus[8];
extern uint8_t Callstatus_old[8];

extern UART_HandleTypeDef huart1;

uint8_t testCheck = 0;
uint8_t isElementPresent = 0;

uint8_t specificationName[50] = {
  //'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    97 , 98 , 99 , 100, 101, 102, 103, 104, 105, 106,
  //'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
  //'u', 'v', 'w', 'x', 'y', 'z', '!', '#', '$', '%',
    117, 118, 119, 120, 121, 122, 33 , 35 , 36 , 37 ,
  //'&', '(', ')', '*', 0x27,',', ':', ';', '<', '>',
    38 , 40 , 41 , 42 , 39 , 44 , 58 , 59 , 60 , 62 ,
  //'=', '@', '[', ']', '{', '}', '|', 0x00,
    61 , 64 , 91 , 93 , 123, 125, 124
};
uint8_t specificationNumber[50] = {
 //'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
 //'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    20,  21,  22,  23,  24,  25,  26,  27,  28,  29, 
 //'u', 'v', 'w', 'x', 'y', 'z', '!', '#', '$', '%',
    30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
 //'&', '(', ')', '*', 0x27,',', ':', ';', '<', '>',
    40,  41,  42,  43,  44,  45,  46,  47,  48,  49, 
 //'=', '@', '[', ']', '{', '}', '|', 0x00,
    50,  51,  52,  53,  54,  55,  56
};

void flipFloorName(uint8_t Floor, uint8_t *N1, uint8_t *N2) // 
{
    for (uint8_t i = 0; i < 47; i++)
    {
        if (specificationName[i] == Floor)
        {
            *N1 = specificationNumber[i] / 10;
            *N2 = specificationNumber[i] % 10 ;
        }
    }
}
uint8_t checkSpecifyFloor(uint8_t x)
{
		uint8_t arrLen = 47;//sizeof specificationName / sizeof specificationName[0];
    
    for (uint8_t i = 0; i <= arrLen; i++) {
        
        if (specificationName[i] == x) {
            isElementPresent = 1;
            break;
        }
				else if (specificationName[i] == 0) isElementPresent = 0;
				else isElementPresent = 0;
		
    }
		if (isElementPresent) {
        // This is specify floor name
			return 1;
    } else {
        // This is not specify floor name
			return 0;
    }
}
void dwinUpdateImageFloor(uint16_t spAddressTen, uint16_t spAddressUint)
{
	uint8_t bufferTen[2]  = {0};
	uint8_t bufferUint[2] = {0};
	static uint8_t bufferDataOld[2] = {0};
	bufferTen[1] = display[BUF_TEN];
	bufferUint[1] = display[BUF_UNIT];

	if (bufferDataOld[0] != bufferTen[1] || bufferDataOld[1] != bufferUint[1])
	{
		bufferDataOld[0] = bufferTen[1];
		bufferDataOld[1] = bufferUint[1];
		dwinWriteData(spAddressTen, bufferTen, 2);
		dwinWriteData(spAddressUint, bufferUint, 2);
	}
}

void changeLocationTEXT(uint16_t spAddress, uint16_t xAxis, uint16_t yAxis)
{
	uint8_t buffer[4];
	buffer[0] = (uint16_t)xAxis >> 8;
	buffer[1] = (uint8_t)xAxis;
	buffer[2] = (uint16_t)yAxis >> 8;
	buffer[3] = (uint8_t)yAxis;
	dwinWriteData(spAddress, buffer, 4);
}

void messageInCase(messageMode_e mode)
{
	uint8_t bufferData[2] = {0};
	bufferData[0] = 0;
	bufferData[1] = mode;
	dwinWriteData(VP_ROLLTEXT, bufferData, 2);
}

void showMessageWarning(_IconShow_e index)
{
	uint8_t messData[2] = {0};
	messData[0] = 0;
	messData[1] = index;
	if (index == fullloadIcon)
	{
		messData[0] = 0;
		messData[1] = index - 1;
	}
	else if (index == overloadIcon)
	{
		messData[0] = 0;
		messData[1] = index + 1;
	}
	if (index == inspectionIcon)
	{
		messData[0] = 0;
		messData[1] = index - 1;
	}
	else if (index == outOfServiceIcon)
	{
		messData[0] = 0;
		messData[1] = index - 1;
	}
	else if (index == emergencyIcon)
	{
		messData[0] = 0;
		messData[1] = index - 2;
	}
	else if (index == parkingIcon)
	{
		messData[0] = 0;
		messData[1] = index - 2;
	}
	else if (index == attendantIcon)
	{
		messData[0] = 0;
		messData[1] = index - 2;
	}
	dwinWriteData(ICON_WARNING, messData, 2);
	dwinWriteData(MESS_WARNING, messData, 2);
}

void Dwin_Change_Current_FloorName(void)
{
	uint8_t Buf1[5] = {0};
	uint8_t Buf[20] = {0};
	uint8_t Num1 = 0;
	uint8_t Num2 = 0;
	uint16_t relocateUnit = 0;
	uint16_t relocateTen = 0;
	uint16_t relocateMain = 0;
	testCheck = checkSpecifyFloor(floorDisplay[BUF_TEN]);
	if ((testCheck == 1) && (floorDisplay[BUF_TEN] != 0x00))
	{
		/* split specific floor */
		flipFloorName(floorDisplay[BUF_TEN], &Num1, &Num2);
		Buf1[0] = 0;
		Buf1[1] = Num1 + 0x30;
		Buf[0] = 0;
		Buf[1] = Num2 + 0x30;
	}
	else
	{
		Buf1[0] = 0;
		Buf1[1] = 0;
			
		Buf[0] = 0;
		Buf[1] = floorDisplay[BUF_TEN];
	}
	Buf[2] = 0;
	Buf[3] = floorDisplay[BUF_UNIT];
		
	dwinWriteData(FLOORNAME_VP_TEN_MAIN, Buf1, 2);
	dwinWriteData(FLOORNAME_VP_TEN, Buf, 2);
	dwinWriteData(FLOORNAME_VP_UNIT, Buf + 2, 2);
	
	/* relocate the position of floor text*/
	relocateMain 	= DEAFAULT_FLOOR_NAME_X_TEN_MAIN;
	relocateTen 	= DEAFAULT_FLOOR_NAME_X_TEN;
	relocateUnit 	= DEAFAULT_FLOOR_NAME_X_UINT;
	
		//Neu ki tu dau tien la khoang cach hoac null
		if ((Buf1[1] == 0x20) || (Buf1[1] == 0x00)) 
		{
			//Neu ki tu thu 2 la khoang cach hoac null 
			if ((Buf[1] == 0x20) || (Buf[1] == 0x00))
				{
					if(Buf[3] != 0x31)
					 relocateUnit = DEAFAULT_FLOOR_NAME_X_UINT - 110;
					else
					relocateUnit = DEAFAULT_FLOOR_NAME_X_UINT - 70;
				}
			//Neu ki tu thu 2 va thu 3 la 1
		  else if ((Buf[1] == 0x31) && (Buf[3] == 0x31))
				{
					relocateTen 	= DEAFAULT_FLOOR_NAME_X_TEN - 15;
					relocateUnit = DEAFAULT_FLOOR_NAME_X_UINT - 15;
				}
			//Neu ki tu thu 2 la 1 và thu 3 khac 1 
			else if ((Buf[1] == 0x31) && (Buf[3] != 0x31))
				{
					relocateTen 	= DEAFAULT_FLOOR_NAME_X_TEN - 15;
					relocateUnit = DEAFAULT_FLOOR_NAME_X_UINT - 35;
				}
			//Neu ki tu thu 2 va 3 khac 1 >> canh chinh cho deu
			else
				{
					relocateTen 	= DEAFAULT_FLOOR_NAME_X_TEN - 30;
					relocateUnit = DEAFAULT_FLOOR_NAME_X_UINT - 30;
				}
		}
	//Neu ki tu dau tien khong phai la khoang cach hoac null (du 3 ki tu)
	//Truong hop ki tu dau tien la 1
	else if (Buf1[1] == 0x31) //// Character: 1
	{
			//Ki tu thu 2 cung la 1
			if (Buf[1] == 0x31)
				{
					relocateTen 	= DEAFAULT_FLOOR_NAME_X_TEN - 50;
					relocateUnit = DEAFAULT_FLOOR_NAME_X_UINT - 50;
				}
			else
				{
					relocateTen 	= DEAFAULT_FLOOR_NAME_X_TEN - 15;
					relocateUnit = DEAFAULT_FLOOR_NAME_X_UINT - 5;
				}
	}
	//Neu ki tu dau tien khong phai la khoang cach hoac null (du 3 ki tu)
	//Truong hop ki tu dau tien khac 1
	else
		{
			if (Buf[1] == 0x31)
				{
					relocateUnit = DEAFAULT_FLOOR_NAME_X_UINT;
				}
		}
	changeLocationTEXT(FLOORNAME_SP_TEN_MAIN + DWIN_POSITION, relocateMain, DEAFAULT_FLOOR_NAME_Y_TEN_MAIN);
  changeLocationTEXT(FLOORNAME_SP_TEN + DWIN_POSITION, relocateTen, DEAFAULT_FLOOR_NAME_Y_TEN);
	changeLocationTEXT(FLOORNAME_SP_UNIT + DWIN_POSITION, relocateUnit, DEAFAULT_FLOOR_NAME_Y_UNIT);
}

void Dwin_switch_to_next_page(void)
{
	static int PageCnt = 0;
	uint8_t Buf[20] = {0};
	Buf[0] = 0x5A;
	Buf[1] = 0x01;
	Buf[2] = 0x00;
	Buf[3] = PageCnt;
	Dwin_Write_VP(0X0084, Buf, 2);
	PageCnt++;
	if (PageCnt == MAX_PAGE)
	{
		PageCnt = 0;
	}
}

void Dwin_update_time(void)
{
	uint8_t Buf[20] = {0};

	Buf[0] = 0;
	Buf[1] = aBCAN_ReceiveBuf_Clock[2];
	if (Buf[1] >= 12)
	{
		Buf[1]-= 12;
	}
	dwinWriteData(TIME_VP , Buf, 2); // set time
	Buf[0] = 0;
	Buf[1] = aBCAN_ReceiveBuf_Clock[1]  ;
	
	dwinWriteData(TIME_VP+1 , Buf, 2); // set time

	
	// Buf[10] = 0;
	Buf[0] = 0;
	Buf[1] = aBCAN_ReceiveBuf_Clock[3];
	dwinWriteData(0x1280 , Buf, 2); // set time

	Buf[0] = 0;
	Buf[1] = (aBCAN_ReceiveBuf_Clock[4] >> 3)  ;
	dwinWriteData(0x1320 , Buf, 2); // set time
}

void DWIN_Arrow_Process(void)
{
	static uint8_t Arrow_state_old = 1;

	uint8_t Buf[100] = {0};
	if (Arrow_state_old == floorDisplay[BUF_ARROW])
		return;
	Arrow_state_old = floorDisplay[BUF_ARROW];
	Buf[0] = 00;
	Buf[2] = 00;
	Buf[4] = 00;
	switch (Arrow_state_old)
	{
	case 0X00: // hide

		Buf[1] = ARROW_STOP;
		Buf[3] = ARROW_STOP;
		Buf[5] = ARROW_STOP;
		break;
	case 0x80:
		Buf[1] = ARROW_UP_START;
		Buf[3] = ARROW_UP_START;
		Buf[5] = ARROW_UP_START;
		break;
	case 0x40:
		Buf[1] = ARROW_DOWN_START;
		Buf[3] = ARROW_DOWN_START;
		Buf[5] = ARROW_DOWN_START;
		break;
	case 0xA0:
		Buf[1] = ARROW_UP_STOP;
		Buf[3] = ARROW_UP_START;
		Buf[5] = ARROW_UP_END;

		break;
	case 0x60:
		Buf[1] = ARROW_DOWN_STOP;
		Buf[3] = ARROW_DOWN_START;
		Buf[5] = ARROW_DOWN_END;
		break;
	default:
		break;
	}
	Dwin_Write_VP(ARROW_ICON_SP + 6, (uint8_t *)&Buf, 3);
}

static uint16_t display_message_old = 1;
void DWIN_Message_Process(void)
{
	if (display_message_old == display_message)
		return;
	if (display_message_old != display_message)
	{
		display_message_old = display_message;
		if (display_message_old)
		{
			if (display_message_old & ERROR_REMOTEOFF)
			{
				showMessageWarning(parkingIcon);
			}
			// Not yet Check
			if (display_message_old & ERROR_ATTENDANCE)
			{
				showMessageWarning(attendantIcon);
			}
			if (display_message_old & ERROR_FULLLOAD)
			{
				showMessageWarning(fullloadIcon);
			}

			if (display_message_old & ERROR_OVERLOAD)
			{
				showMessageWarning(overloadIcon);
			}
			if (display_message_old & ERROR_INSPECTION)
			{
				showMessageWarning(inspectionIcon);
			}

			if (display_message_old & ERROR_EMERGENCY)
			{
				showMessageWarning(emergencyIcon);
			}
			if (display_message_old & ERROR_FIRECASE)
			{
				showMessageWarning(fireIcon);
			}
		}
		else
		{
			showMessageWarning(normalLogo);
		}
	}
}


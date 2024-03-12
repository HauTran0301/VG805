/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define _MAIN_C_
#include "AllHeader.h"
#include "pn532_stm32f1.h"
#include "stm32f1xx_hal.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define USER_DEBUG 0
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
PN532 pn532;
uint8_t buzzer_flag;
uint8_t ui8_DetectNewCard;
int32_t uid_len;
uint8_t buffR[16];
uint8_t buffR_1[16];
uint8_t buff[255];
uint8_t uid[MIFARE_UID_MAX_LENGTH];
uint8_t old_uid[MIFARE_UID_MAX_LENGTH];
uint8_t compare_uid;
uint8_t period_done ;
uint16_t ui16_Buzzer_On;
uint8_t key_b[6] = {0xff,0xff,0xff,0xff,0xff,0xff}; 
uint8_t data_1;
uint8_t bit_1;
uint8_t bit_2;
uint8_t bit_3;
uint8_t bit_4;
uint8_t bit_5;
uint8_t bit_6;
uint8_t bit_7;
uint8_t bit_8;
uint8_t floor_1 ;
uint8_t floor_2 ;
uint8_t floor_3 ;
uint8_t floor_4 ;
uint8_t floor_5 ;
uint8_t floor_6 ;
uint8_t floor_7 ;
uint8_t floor_8 ;
uint8_t flag;
uint8_t flag_1;
uint8_t flag_2;
uint8_t	frame_1;
uint8_t	frame_2;
uint8_t	frame_3;
uint8_t	frame_4;
uint8_t	frame_5;
uint8_t	frame_6;
uint8_t	frame_7;
uint8_t	frame_8;
uint8_t	frame_9;
uint8_t frame_check_1;
uint8_t frame_check_2;
uint8_t frame_check_3;
uint8_t frame_check_4;
uint8_t frame_check_5;
uint8_t frame_check_6;
uint8_t frame_check_7;
uint8_t frame_check_8;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_IWDG_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void ClrWdt(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void ClrWdt(void)
{
  HAL_IWDG_Refresh(&hiwdg);
}

void init_userpara(void)
{
  uint8_t i;
  inpush = 0;
  outpush = 0;
  backlight_mode = 0;


  for (i = 0; i < MAX_IO_TYPE; i++)
  {
    virt_in[i] = 0;
  }
  in_polarity = 0;
  out_polarity = 0;

  out = 0;
  in = (uint8_t)(GPIOB->IDR >> 11) & 0x0F;
  input[0] = in;
  input[1] = in;
  input[2] = in;
  inold = 0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

void CAN_bus(void)
{
if (merker == RC_MERKER)
  {
    errorregister |= ER_COMMUNICATION;
  }
  if (merker == BS_MERKER)
  {
    errorregister |= ER_COMMUNICATION;
  }
  merker = WD_MERKER;
  node_id = READ_CARD_ID;
  init_userpara();
  Init_Can();
  HAL_Delay(5);
  ClrWdt();          /* reset watchdog timer					*/
}
//Led Run
void RUN_Blink(void)
{
	static uint32_t  Timeblink =0;
	if(HAL_GetTick() > Timeblink)
	{
		HAL_GPIO_TogglePin(RUN_GPIO_Port, RUN_Pin);
	}
}
//Relay
void RELAY(void)
{
    static uint32_t Timeblink0 = 0;

    if ((HAL_GetTick() > Timeblink0) && (Timeblink0 == 0))
    {
        HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
        Timeblink0 = HAL_GetTick() + 3000;
    }
    else if ((HAL_GetTick() > Timeblink0) && (Timeblink0 > 0))
    {
      HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
			if(flag == 1)
			{
				Timeblink0 = 0;
			}
    }
}
//Handle cards type
void frame_att_card (void)
{
	if(frame_1 == 1)
	{
		virt_in[0] =  0x20;
		virt_in[1] =  0x30;
		virt_in[2] =  0x10;
		virt_in[3] =  0xF0;
		virt_in[4] =  buffR[13];
		virt_in[5] =  buffR[14];
		virt_in[6] =  buffR[15];
		virt_in[7]=   0x01;
		node_id 	 = 	0x20;
		transmit_in(virt_in);
		frame_check_1 = 1;
	}
	if(frame_2 == 1)
	{
		virt_in[0] =  0x20;
		virt_in[1] =  0x30;
		virt_in[2] =  0x10;
		virt_in[3] =  0xF1;
		virt_in[4] =  buffR[0];
		virt_in[5] =  buffR[2];
		virt_in[6] =  0x00;
		virt_in[7] =  0x00;
		node_id = 	0x20;
		transmit_in(virt_in);
		frame_check_2 = 1;
	}
	if(frame_3 == 1)
	{
		virt_in[0] =  0x20;
		virt_in[1] =  0x30;
		virt_in[2] =  0x10;
		virt_in[3] =  0xF4;
		virt_in[4] =  0x00;
		virt_in[5] =  0x00;
		virt_in[6] =  0x00;
		virt_in[7] =  0x00;
		transmit_in(virt_in);
		node_id = 	0x20;
	}
	HAL_Delay(50);
	flag = 0;
	flag_2 = 0;
	frame_3 = 0;
}

void frame_card_management(void)
{
	if(frame_1 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF0;
			virt_in[4] =  buffR[13];
			virt_in[5] =  buffR[14];
			virt_in[6] =  buffR[15];
			virt_in[7]=   0x01;
			node_id 	 = 	0x20;
			transmit_in(virt_in);
			frame_check_1 = 1;
		}
		if(frame_2 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF1;
			virt_in[4] =  buffR[0];
			virt_in[5] =  buffR[2];
			virt_in[6] =  0x00;
			virt_in[7] =  0x00;
			node_id 	 = 	0x20;
			transmit_in(virt_in);
			frame_check_2 = 1;
		}
		if(frame_3 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF7;
			virt_in[4] =  0x00;
			virt_in[5] =  0x00;
			virt_in[6] =  0x00;
			virt_in[7] =  0x00;
			transmit_in(virt_in);
			node_id 	 = 	0x20;
		}
		HAL_Delay(50);
		flag = 0;
		flag_2 = 0;
		frame_3 = 0;
}

void frame_el_locking (void)
{
	if(frame_1 == 1)
	{
		virt_in[0] =  0x20;
		virt_in[1] =  0x30;
		virt_in[2] =  0x10;
		virt_in[3] =  0xF0;
		virt_in[4] =  buffR[13];
		virt_in[5] =  buffR[14];
		virt_in[6] =  buffR[15];
		virt_in[7]=   0x01;
		node_id 	 = 	0x20;
		transmit_in(virt_in);
		frame_check_1 = 1;
	}
	if(frame_2 == 1)
	{
		virt_in[0] =  0x20;
		virt_in[1] =  0x30;
		virt_in[2] =  0x10;
		virt_in[3] =  0xF1;
		virt_in[4] =  buffR[0];
		virt_in[5] =  buffR[2];
		virt_in[6] =  0x00;
		virt_in[7] =  0x00;
		node_id 	 = 	0x20;
		transmit_in(virt_in);
		frame_check_2 = 1;
	}
		if(frame_3 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF5;
			if(buffR[3] == 0x01)
			{
				data_1 = 0x01;    					//floor 1
			}
			if(buffR[3] == 0x02)
			{
				data_1 = 0x02;  						// floor 2
			}
			if(buffR[3] == 0x04)
			{
				data_1 =(0x04 >> 1) | 1;  	// floor 3
			}
			if(buffR[3] == 0x08)
			{
				data_1 = 0x08 >> 1;  				//floor 4
			}
			if(buffR[3] == 0x10)
			{
				data_1 =(0x10 >> 2) | 1;  	// floor 5
			}
			if(buffR[3] == 0x20)
			{
				data_1 = (0x20 >> 3) | 2; 	//floor 6
			}
			if(buffR[3] == 0x40)
			{
				data_1 = (0x40 >> 4) | 3;   //floor 7
			}
			if(buffR[3] == 0x80)
			{
				data_1 = 0x80 >> 4; 				//floor 8
			}
			if(buffR[4] == 0x01)
			{
				data_1 = (0x01 << 3) | 1;    //floor 9
			}
			if(buffR[4] == 0x02)
			{
				data_1 = (0x02 << 2) | 2;
			}
			if(buffR[4] == 0x04)
			{
				data_1 = (0x04 << 1) | 3;
			}
			if(buffR[4] == 0x08)
			{
				data_1 = 0x08 | 4;
			}
			virt_in[4] =  data_1;
			virt_in[5] =  0x0E;
			virt_in[6] =  0x81;
			virt_in[7] =  0x01;
			transmit_in(virt_in);
			node_id 	 = 	0x20;
		}
		HAL_Delay(50);
		flag = 0;
		flag_2 = 0;
		frame_3 = 0;
}

void frame_seperate_ctrl (void)
{
		if(frame_1 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF0;
			virt_in[4] =  buffR[13];
			virt_in[5] =  buffR[14];
			virt_in[6] =  buffR[15];
			virt_in[7]=   0x01;
			node_id 	 = 	0x20;
			transmit_in(virt_in);
			frame_check_1 = 1;
		}
		if(frame_2 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF1;
			virt_in[4] =  buffR[0];
			virt_in[5] =  buffR[2];
			virt_in[6] =  0x00;
			virt_in[7] =  0x00;
			transmit_in(virt_in);
			frame_check_2 = 1;
		}
		if(frame_3 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF2;
			virt_in[4] =  buffR[3];
			virt_in[5] =  buffR[4];
			virt_in[6] =  buffR[5];
			virt_in[7] =  buffR[6];
			transmit_in(virt_in);
			frame_check_3 = 1;
		}
		if(frame_4 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF3;
			virt_in[4] =  buffR[7];
			virt_in[5] =  buffR[8];
			virt_in[6] =  buffR[9];
			virt_in[7] =  buffR[10];
			transmit_in(virt_in);
		}
		HAL_Delay(50);
		flag = 0;
		flag_2 = 0;
		frame_4 = 0;
}


void frame_period_time(void)
{
	if(frame_1 == 1)
	{
		virt_in[0] =  0x20;
		virt_in[1] =  0x30;
		virt_in[2] =  0x10;
		virt_in[3] =  0xF0;
		virt_in[4] =  buff[13];
		virt_in[5] =  buff[14];
		virt_in[6] =  buffR[15];
		virt_in[7]=   0x01;
		node_id 	 = 	0x20;
		transmit_in(virt_in);
		frame_check_1 = 1;
	}
		if(frame_2 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF1;
			virt_in[4] =  buffR[0];
			virt_in[5] =  buffR[2];
			virt_in[6] =  0x00;
			virt_in[7] =  0x00;
			node_id 	 = 	0x20;
			transmit_in(virt_in);
			frame_check_2 = 1;
		}
		if(frame_3 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF2;
			virt_in[4] =  buffR[3];
			virt_in[5] =  buffR[4];
			virt_in[6] =  buffR[5];
			virt_in[7] =  buffR[6];
			transmit_in(virt_in);
			node_id 	 = 	0x20;
			frame_check_3 = 1;
		}
		if(frame_4 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF3;
			virt_in[4] =  buff[7];
			virt_in[5] =  buff[8];
			virt_in[6] =  buff[9];
			virt_in[7] =  buff[10];
			transmit_in(virt_in);
			node_id 	 = 	0x20;
			frame_check_4 = 1;
		}
		if(frame_5 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF8;
			virt_in[4] =  0x00;
			virt_in[5] =  0x00;
			virt_in[6] =  0x00;
			virt_in[7] =  0x00;
			transmit_in(virt_in);
			node_id 	 = 	0x20;
			frame_check_5 = 1;
		}
		if(frame_6 == 1)
		{
			virt_in[0] =  0x20;
			virt_in[1] =  0x30;
			virt_in[2] =  0x10;
			virt_in[3] =  0xF9;
			virt_in[4] =  buffR_1[3];
			virt_in[5] =  buffR_1[4];
			virt_in[6] =  buffR_1[5];
			virt_in[7] =  buffR_1[6];
			transmit_in(virt_in);
			node_id 	 = 	0x20;
		}
		HAL_Delay(50);
		flag = 0;
		flag_2 = 0;
		frame_6 = 0;
}

void check_bit(void)
{
	if(buffR[3] & 0x01)
	{
		bit_1 = buffR[3] & 0x01;
		if(bit_1 !=0)
		{
			bit_1 = 1;
		}
	}
	if(buffR[3] & 0x02)
	{
		bit_2 = buffR[3] & 0x02;
		if(bit_2 !=0)
		{
			bit_2 = 1;
		}
	}
	if(buffR[3] & 0x04)
	{
		bit_3 = buffR[3] & 0x04;
		if(bit_3 !=0)
		{
			bit_3 = 1;
		}
	}
	if(buffR[3] & 0x08)
	{
		bit_4 = buffR[3] & 0x08;
		if(bit_4 !=0)
		{
			bit_4 = 1;
		}
	}
	if(buffR[3] & 0x10)
	{
		bit_5 = buffR[3] & 0x10;
		if(bit_5 !=0)
		{
			bit_5 = 1;
		}
	}
	if(buffR[3] & 0x20)
	{
		bit_6 = buffR[3] & 0x20;
		if(bit_6 !=0)
		{
			bit_6 = 1;
		}
	}
	if(buffR[3] & 0x40)
	{
		bit_7 = buffR[3] & 0x40;
		if(bit_7 !=0)
		{
			bit_7 = 1;
		}
	}
	if(buffR[3] & 0x80)
	{
		bit_8 = buffR[3] & 0x80;
		if(bit_8 !=0)
		{
			bit_8 = 1;
		}
	}
}

void floor_control(void)
{
	check_bit();
	if(bit_1 == 1)
	{
		HAL_GPIO_WritePin(GPIOB, OUT1_Pin, GPIO_PIN_SET);
		RELAY();
	}
	if(bit_2 == 1)
	{
		HAL_GPIO_WritePin(GPIOB, OUT2_Pin, GPIO_PIN_SET);
	}
	if(bit_3 == 1)
	{
		HAL_GPIO_WritePin(GPIOB, OUT3_Pin, GPIO_PIN_SET);
	}
	if(bit_4 == 1)
	{
		HAL_GPIO_WritePin(GPIOB, OUT4_Pin, GPIO_PIN_SET);
	}
	if(bit_5 == 1)
	{
		HAL_GPIO_WritePin(GPIOB, OUT5_Pin, GPIO_PIN_SET);
	}
	if(bit_6 == 1)
	{
		HAL_GPIO_WritePin(GPIOB, OUT6_Pin, GPIO_PIN_SET);
	}
	if(bit_7 == 1)
	{
		HAL_GPIO_WritePin(GPIOB, OUT7_Pin, GPIO_PIN_SET);
	}
	if(bit_8 == 1)
	{
		HAL_GPIO_WritePin(GPIOB, OUT8_Pin, GPIO_PIN_SET);
	}
}


int main(void)
 {
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_IWDG_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
	PN532 pn532;
	PN532_SPI_Init(&pn532);
	if (PN532_GetFirmwareVersion(&pn532, buff) == PN532_STATUS_OK) {
    PN532_SamConfiguration(&pn532);
  } else {
    return -1;
  }
  ClrWdt(); // reset watchdog timer
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  /*----------------------------*/
  CAN_bus();
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		RUN_Blink();
	 //Doc thong tin the: UID - LENGTH
		uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 100);
	  for (uint8_t j=0; j < uid_len; j++)
	  {
		  if(uid[j] != old_uid[j])
		  {
			  old_uid[j] = uid[j];
		  }
	  }
	//Trong che do ranh ==> Phat hien 1 the moi
		if((uid_len >= 4) && (!ui8_DetectNewCard))
		{
			ui8_DetectNewCard = 1;
		}
		//Chap nhan xu li 1 the moi
		if(ui8_DetectNewCard == 1)
		{	
			flag = 1;
			PN532_MifareClassicAuthenticateBlock(&pn532, uid, uid_len, 3, MIFARE_CMD_AUTH_B, key_b);
			PN532_MifareClassicReadBlock(&pn532,buffR, 1);
			PN532_MifareClassicReadBlock(&pn532,buffR_1, 2);
			if(((buffR[1] >= 0x01) && (buffR[1] <= 0x07)) && ((buffR[2]> 0) && (buffR[2]<=255)))
			{
				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
				buzzer_flag = 1;
				HAL_Delay(200);
				HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
				HAL_Delay(200);
			}
			ui8_DetectNewCard = 0;
		}
		/*********************************************************************************************/
		if(buffR[0] == 0x01 || buffR[0] == 0x21 || buffR[0] == 0x11 || buffR[0] == 0x31)
		{
			if(flag == 1)
			{
				flag_2 = 1;
				if(flag_2)
				{
					frame_1 = 1;
					if(frame_1)
					{
						frame_card_management();
					}
					HAL_Delay(2);
					if(frame_check_1 == 1)
					{
						frame_1 = 0;
						frame_2 = 1;
						frame_check_1 = 0;
						frame_card_management();
					}
					if(frame_check_2 == 1)
					{
						frame_1 = 0;
						frame_2 = 0;
						frame_3	= 1;
						frame_check_2 = 0;
						frame_card_management();
					}
				 }
			  }
		  }
		if(buffR[0] == 0x02 || buffR[0] == 0x12 || buffR[0] == 0x22 || buffR[0] == 0x32)
		{
			if(flag == 1)
			{
				flag_2 = 1;
				if(flag_2)
				{
					frame_1 = 1;
					if(frame_1)
					{
						frame_seperate_ctrl();
					}
					HAL_Delay(10);
					if(frame_check_1 == 1)
					{
						frame_1 = 0;
						frame_2 = 1;
						frame_check_1 = 0;
						frame_seperate_ctrl();
					}
					HAL_Delay(10);
					if(frame_check_2 == 1)
					{
						frame_1 = 0;
						frame_2 = 0;
						frame_3	= 1;
						frame_check_2 = 0;
						frame_seperate_ctrl();
					}	
					HAL_Delay(10);
					if(frame_check_3 == 1)
					{
						frame_1 = 0;
						frame_2 = 0;
						frame_3	= 0;
						frame_4	= 1;
						frame_check_3 = 0;
						frame_seperate_ctrl();
					}	
				 }
			  }
		  }
		 if(buffR[0] == 0x05 || buffR[0] == 0x15 || buffR[0] == 0x25 || buffR[0] == 0x35)
		 {
			 if(flag == 1)
			 {
				flag_2 = 1;
				if(flag_2)
				{
					frame_1 = 1;
					if(frame_1)
					{
						frame_period_time();
					}
					HAL_Delay(2);
					if(frame_check_1 == 1)
					{
						frame_1 = 0;
						frame_2 = 1;
						frame_check_1 = 0;
						frame_period_time();
					}
					HAL_Delay(10);
					if(frame_check_2 == 1)
					{
						frame_1 = 0;
						frame_2 = 0;
						frame_3 = 1;
						frame_check_2 = 0;
						frame_period_time();
					}
					HAL_Delay(10);
					if(frame_check_3 == 1)
					{
						frame_1 = 0;
						frame_2 = 0;
						frame_3 = 0;
						frame_4 = 1;
						frame_check_3 = 0;
						frame_period_time();
					}
					HAL_Delay(10);
					if(frame_check_4 == 1)
					{
						frame_1 = 0;
						frame_2 = 0;
						frame_3 = 0;
						frame_4 = 0;
						frame_5 = 1;
						frame_check_4 = 0;
						frame_period_time();
					}
					HAL_Delay(10);
					if(frame_check_5 == 1)
					{
						frame_1 = 0;
						frame_2 = 0;
						frame_3 = 0;
						frame_4 = 0;
						frame_5 = 0;
						frame_6 = 1;
						frame_check_5 = 0;
						frame_period_time();
					}
				 }
			  }
		   }
			if(buffR[0] == 0x06 || buffR[0] == 0x16 || buffR[0] == 0x26 || buffR[0] == 0x36)
			{
				if(flag == 1)
				{
					flag_2 = 1;
					if(flag_2)
					{
						frame_1 = 1;
						if(frame_1)
						{
							frame_att_card();
						}
						if(frame_check_1 == 1)
						{
							frame_1 = 0;
							frame_2 = 1;
							frame_check_1 = 0;
							frame_att_card();
						}
						if(frame_check_2 == 1)
						{
							frame_1 = 0;
							frame_2 = 0;
							frame_3	= 1;
							frame_check_1 = 0;
							frame_att_card();
						}
					}
				}
			}
		if(buffR[0] == 0x07 || buffR[0] == 0x17 || buffR[0] == 0x27 || buffR[0] == 0x37)
		{
			if(flag == 1)
			{
				flag_2 = 1;
				if(flag_2)
				{
					frame_1 = 1;
					if(frame_1)
					{
						frame_el_locking();
					}
					if(frame_check_1 == 1)
					{
						frame_1 = 0;
						frame_2 = 1;
						frame_el_locking();
					}
					if(frame_check_2 == 1)
					{
						frame_1 = 0;
						frame_2 = 0;
						frame_3	= 1;
						frame_el_locking();
					}
				 }
			  }
			}
	ClrWdt();
	}
}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
 
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_15TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_LSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7199;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7199;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 29;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 7199;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 19;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RUN_Pin|SPI1_SS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RELAY_Pin|BUZZER_Pin|OUT3_Pin|OUT4_Pin
                          |OUT5_Pin|OUT6_Pin|OUT7_Pin|OUT8_Pin
                          |OUT2_Pin|OUT1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RUN_Pin SPI1_SS_Pin */
  GPIO_InitStruct.Pin = RUN_Pin|SPI1_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RELAY_Pin BUZZER_Pin OUT3_Pin OUT4_Pin
                           OUT5_Pin OUT6_Pin OUT7_Pin OUT8_Pin
                           OUT2_Pin OUT1_Pin */
  GPIO_InitStruct.Pin = RELAY_Pin|BUZZER_Pin|OUT3_Pin|OUT4_Pin
                          |OUT5_Pin|OUT6_Pin|OUT7_Pin|OUT8_Pin
                          |OUT2_Pin|OUT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SW1_1_Pin SW1_2_Pin SW1_3_Pin SW1_4_Pin
                           SW2_1_Pin SW2_2_Pin */
  GPIO_InitStruct.Pin = SW1_1_Pin|SW1_2_Pin|SW1_3_Pin|SW1_4_Pin
                          |SW2_1_Pin|SW2_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


#include "AllHeader.h"

/************************************************************************************************/
/* Low priority interrupt routine																*/
/************************************************************************************************/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern uint8_t buzzer_flag;
	

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint8_t i, j;
	static uint8_t timer_100ms = 0;
	static uint8_t count = 0;
	static uint8_t timer_cnt = 0;
	if (htim->Instance == htim2.Instance)
	{

		bTime.Time_500ms = 1;
		if (sdo_timer) /* check for SDO transfer time out		*/
			sdo_timer--;
		if (hsetime)
			hsetime--;
		if (!hsetime)	  /* HSE heartbeat time out 				*/
			hsecheck = 1; /* HSE check necessary					*/
		if (can_inittime)
			--can_inittime;
		if (heartbeat) /* decrement heartbeat timer			*/
			heartbeat--;
		if (flashtimer)
			flashtimer--;
		if (landingcalltimer) /* timer for misuse of landing calls	*/
			landingcalltimer--;
		if (display_timer)
			display_timer--;
		if (can_passive_time)
			--can_passive_time;
		com_can_work++;
		timer_cnt++;
		if ((timer_cnt % 2) == 0)
		{
			timer_1S5 = 1;
			bTime.Time_1s = 1;
		}
		if ((timer_cnt % 20) == 0)
		{
			bTime.Time_5s = 1;
		}
		bTime.dwin_500ms = 1;
	}
	if (htim->Instance == htim3.Instance)
	{
		if (!input_detect)
		{
			input[2] = input[1]; /* shift input variables				*/
			input[1] = input[0];
			input[0] = (uint8_t)(GPIOB->IDR >> 11) & 0x0F;
			input[0] = (input[0] ^ 0x0f) & 0x0F; //
			for (i = 0; i < 8; i++)
			{
				j = 0x01 << i;
				if (((input[2] & j) == (input[1] & j)) &&
					((input[1] & j) == (input[0] & j)))
				{
					in &= ~j; /* set input valid						*/
					in |= (input[2] & j);
				}
			}
		}
		++input_detect;
	}
	//timer ?ms
	if (htim->Instance == htim4.Instance)
	{
		if ((timer_100ms) && (buzzer_flag))
		{
			HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin,  GPIO_PIN_RESET);
			buzzer_flag = 0;
			timer_100ms = 0;
		}
	}
}


#define _CAN_C_
#include "AllHeader.h"


/************************************************************************************************/
/* High priority interrupt routine																*/
/************************************************************************************************/
extern CAN_HandleTypeDef hcan;
extern uint8_t Callstatus[8];
extern uint8_t Callstatus_old[8];
extern uint8_t Arrow_state;
uint8_t Can1RxData[8] = {0};
CAN_RxHeaderTypeDef Can1RxHeader;

void Init_Can(void)
{

	uint8_t i, j;

	rc = 0; // clear all CAN variables
	ri = 0;
	ro = 0;
	tc = 0;
	ti = 0;
	to = 0;
	for (i = 0; i < RX_SIZE; i++) // clear rx buffer
	{
		for (j = 0; j < 10; j++)
			rx[i][j] = 0;
	}
	for (i = 0; i < TX_SIZE; i++) // clear tx buffer
	{
		for (j = 0; j < 10; j++)
			tx[i][j] = 0;
	}

	CAN_FilterTypeDef sFilterConfig;
	/*------------filter bank 1----------*/
	sFilterConfig.FilterBank = 1;
	sFilterConfig.FilterIdHigh = PDO_OUT << 8;
	sFilterConfig.FilterIdLow = 0x00;
	sFilterConfig.FilterMaskIdHigh = 0xF000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO1;
	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO1;
	sFilterConfig.FilterBank = 2;
	sFilterConfig.FilterIdHigh = ((RSDO | (node_id >> 3)) << 8) + (node_id << 5);
	// sFilterConfig.FilterIdHigh = RSDO << 8;
	sFilterConfig.FilterIdLow = 0x00;
	sFilterConfig.FilterMaskIdHigh = 0xF000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
	}
	sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO1;
	sFilterConfig.FilterBank = 3;
	sFilterConfig.FilterIdHigh = NMT << 8;
	sFilterConfig.FilterIdLow = 0x00;
	sFilterConfig.FilterMaskIdHigh = 0xF000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
	}

	sFilterConfig.FilterBank = 4;
	sFilterConfig.FilterIdHigh = (MPDO << 8) + (EMS_ID << 5);
	sFilterConfig.FilterIdLow = 0x00;
	sFilterConfig.FilterMaskIdHigh = 0xF000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO1;

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO1;
	sFilterConfig.FilterBank = 5;
	sFilterConfig.FilterIdHigh = LSS << 8;
	sFilterConfig.FilterIdLow = 0x00;
	sFilterConfig.FilterMaskIdHigh = 0xF000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
	}

	sFilterConfig.FilterBank = 6;
	sFilterConfig.FilterIdHigh = FC_D << 8;
	sFilterConfig.FilterIdLow = 0x00;
	sFilterConfig.FilterMaskIdHigh = 0xF000;
	sFilterConfig.FilterMaskIdLow = 0x0000;

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
	}

	/* -----------------Filter bank 0---------------*/
	sFilterConfig.FilterBank = 10;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = ((HEARTBEAT << 3) + HSE_ID) << 5;
	sFilterConfig.FilterIdLow = 0x00;
	sFilterConfig.FilterMaskIdHigh = ((HEARTBEAT << 3) + HSE_ID) << 5;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);

	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
	}
	HAL_CAN_Start(&hcan);
}


void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &Can1RxHeader, Can1RxData);

	if ((rc == RX_SIZE) && (!can_passive_time)) /* software buffer data overrun 		*/
	{
		errorregister |= ER_COMMUNICATION; /* set error bits 					*/
		can_passive_time = ERROR_TIME;
	}
	else
	{
		rx[ri][0] = (Can1RxHeader.StdId >> 3) & 0xF0; // read function code
		rx[ri][1] = Can1RxHeader.StdId & 0x7F;		  //(RXB1SIDL >> 5) + ((RXB1SIDH & 0x0F) << 3);	// node ID
		if ((rx[ri][0] != RSDO) || (rx[ri][1] == node_id))
		{
			memcpy((void *)&rx[ri][2], Can1RxData, Can1RxHeader.DLC);
			if (ri == (RX_SIZE - 1)) // increment RX message write pointer
				ri = 0;
			else
				ri++;
			rc++; // increment message counter
		}
	}
}

uint8_t CanRxData[8] = {0};
CAN_RxHeaderTypeDef CanRxHeader;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CanRxHeader, CanRxData);

	if (((CanRxHeader.StdId >> 3) & 0xF0) == HEARTBEAT)
	{
		if (((CanRxHeader.StdId & 0x7F) - 1) < MAX_LIFT) // Heartbeat from HSE
			hsetime = HSETIME;							 // reset HSE supervisor time
		else
		{
			// hsetime = HSETIME - 10;
		}
	}
	hse_heartbeat = 1;
}
uint32_t pTxMailbox;

uint16_t mallbox[3] = {0};
uint16_t txmallbox = 0;
uint16_t txmallbox_request = 0;
void CAN_transmit_Interrupt(void)
{
	if (tc) // more messages to send
	{
		CAN_TxHeaderTypeDef CanTxHeader;
		uint8_t CanTxData[8] = {0};
		CanTxHeader.StdId = ((tx[to][0] & 0xF0) << 3) + tx[to][1];
		CanTxHeader.DLC = tx[to][0] & 0x0F; // read data lenght code
		CanTxHeader.IDE = CAN_ID_STD;
		CanTxHeader.RTR = CAN_RTR_DATA;
		memcpy(CanTxData, (void *)(tx[to] + 2), 8);
		if (HAL_CAN_AddTxMessage(&hcan, &CanTxHeader, CanTxData, &pTxMailbox) == HAL_OK)
		{
			txmallbox++;
		}
		else
		{
			Error_Handler();
		}
		tc--;					 // decrement TX message counter
		if (to == (TX_SIZE - 1)) // increment TX message read pointer
			to = 0;
		else
			to++;
	}
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	mallbox[0]++;
	CAN_transmit_Interrupt();
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	mallbox[1]++;
	CAN_transmit_Interrupt();
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	mallbox[2]++;
	CAN_transmit_Interrupt();
}
// ransmit special inputs and calls
void transmit_in(uint8_t *input)
{
	uint8_t i;
	while (tc == TX_SIZE)
		;							  /* wait for TX buffer free				*/
	tx[ti][0] = MPDO + MAX_IO_TYPE; /* write function code + data length	*/
	tx[ti][1] = node_id;	
	/* write node id of UEA					*/
	for (i = 0; i < MAX_IO_TYPE; i++)
		tx[ti][2 + i] = *input++; /* write input function					*/
	can_transmit();				  /* transmit message						*/
}

uint8_t CanTxData_t[8] = {0};
void can_transmit(void)
{
	txmallbox_request++;
	if ((!hse_heartbeat) || (merker == BS_MERKER)) // û��MCU����������busoff�������κ���Ϣ
		return;

	if (ti == (TX_SIZE - 1)) /* increment TX message write pointer	*/
		ti = 0;
	else
		ti++;
	if (HAL_CAN_DeactivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
	}
	CAN_TxHeaderTypeDef CanTxHeader_t;

	uint32_t Txmallbox;
	CanTxHeader_t.StdId = ((tx[to][0] & 0xF0) << 3) + tx[to][1];
	CanTxHeader_t.DLC = tx[to][0] & 0x0F; // read data lenght code
	CanTxHeader_t.IDE = CAN_ID_STD;
	CanTxHeader_t.RTR = CAN_RTR_DATA;

	memcpy(CanTxData_t, (void *)(tx[to] + 2), 8);
	if (HAL_CAN_AddTxMessage(&hcan, &CanTxHeader_t, CanTxData_t, &Txmallbox) == HAL_OK)
	{
		txmallbox++;
		if (to == (TX_SIZE - 1)) /* increment TX message read pointer	*/
			to = 0;
		else
			to++;
	}
	else
	{
		tc++;
	}
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
	}
}

#ifndef _CAN_H_
#define	_CAN_H_



#define MBIT_0 0x00000001
#define MBIT_1 0x00000002
#define MBIT_2 0x00000004
#define MBIT_3 0x00000008
#define MBIT_4 0x00000010
#define MBIT_5 0x00000020
#define MBIT_6 0x00000040
#define MBIT_7 0x00000080
#define MBIT_8 0x00000100
#define MBIT_9 0x00000200
#define MBIT_10 0x00000400
#define MBIT_11 0x00000800
#define MBIT_12 0x00001000
#define MBIT_13 0x00002000
#define MBIT_14 0x00004000
#define MBIT_15 0x00008000
#define MBIT_16 0x00010000
#define MBIT_17 0x00020000
#define MBIT_18 0x00040000
#define MBIT_19 0x00080000
#define MBIT_20 0x00100000
#define MBIT_21 0x00200000
#define MBIT_22 0x00400000
#define MBIT_23 0x00800000
#define MBIT_24 0x01000000
#define MBIT_25 0x02000000
#define MBIT_26 0x04000000
#define MBIT_27 0x08000000
#define MBIT_28 0x10000000
#define MBIT_29 0x20000000
#define MBIT_30 0x40000000
#define MBIT_31 0x80000000

#define ERROR_FIRECASE    MBIT_0
#define ERROR_EMERGENCY   MBIT_1
#define ERROR_OVERLOAD    MBIT_2
#define ERROR_OUTOFORDER  MBIT_3
#define ERROR_GENEFAULT   MBIT_4
#define ERROR_INSPECTION  MBIT_5
#define ERROR_FULLLOAD    MBIT_6
#define ERROR_VIPRUN      MBIT_7
#define ERROR_PRIORITY    MBIT_8
#define ERROR_REMOTEOFF   MBIT_10
#define ERROR_SPECIALMODE MBIT_11
#define ERROR_EARTHQUAKE  MBIT_12
#define ERROR_ATTENDANCE  MBIT_13
/************************************************************************************************/
/* Definitions for configuration register CANCON												*/
/************************************************************************************************/
#define CAN_MODE_BITS		0xE0		   				/* Mode bits in configuration register	*/
#define CAN_MODE_CONFIG		0x80		   				/* Configuration mode					*/
#define CAN_MODE_LISTEN		0x60		   				/* Listen Only mode						*/
#define CAN_MODE_NORMAL1		0x00		   				/* Normal mode							*/

/************************************************************************************************/
/* Definitions for interrupt code in CANSTAT register											*/
/************************************************************************************************/
#define INTCODE				0x0E		   				/* Mask for interrupt code bits			*/
#define WAKEINT				0x0E		   				/* Wake up interrupt					*/
#define RXB0INT				0x0C		   				/* RX buffer 0 interrupt				*/
#define RXB1INT				0x0A		   				/* RX buffer 1 interrupt				*/
#define TXB0INT				0x08		   				/* TX buffer 0 interrupt				*/
#define TXB1INT				0x06		   				/* TX buffer 1 interrupt				*/
#define TXB2INT				0x04		   				/* TX buffer 2 interrupt				*/
#define ERRORINT			0x02		   				/* error interrupt						*/
#define NOINT				0x00		   				/* kein interrupt						*/

/************************************************************************************************/
/* Definitions for Win bits in CANCON register													*/
/************************************************************************************************/
#define TXB0REG				0x08		   				/* TX buffer 0 register access			*/
#define TXB1REG				0x06		   				/* TX buffer 1 register access			*/

#define	BPS_125K			0
#define	BPS_50K				1
#define	BPS_25K				2

#define	C_BAUDRATE_LISTEN_10S	780 * 4
#define	C_MINIMAM_TIMES			4

#define RX_SIZE						30
#define TX_SIZE						30

#ifdef _CAN_C_

uint8_t volatile rc;			// RX message counter
uint8_t volatile tc;			// TX message counter
uint8_t volatile ro;			// pointer for FIFO read
uint8_t volatile ri;			// pointer for FIFO write
uint8_t volatile to;			// pointer for TX FIFO read
uint8_t volatile ti;			// pointer for TX FIFO write
uint8_t volatile tr;			// Senden fertig

uint8_t volatile rx[RX_SIZE][10];	// RX message buffer
uint8_t volatile tx[TX_SIZE][10];	// TX message buffer
uint8_t aBCAN_ReceiveBuf_Clock[8];
uint8_t aBCAN_ReceiveBuf_Clock_old[8];
uint8_t floorDisplay[4];
void Init_Can (void);
void transmit_in(uint8_t *input);
void can_transmit (void);

#else
extern uint8_t floorDisplay[4];
extern uint8_t volatile rc;
extern uint8_t volatile tc;
extern uint8_t volatile ro;
extern uint8_t volatile ri;
extern uint8_t volatile to;
extern uint8_t volatile ti;
extern uint8_t volatile tr;
extern uint8_t volatile rx[RX_SIZE][10];
extern uint8_t volatile tx[TX_SIZE][10];

extern uint8_t aBCAN_ReceiveBuf_Clock[8];
extern uint8_t aBCAN_ReceiveBuf_Clock_old[8];

extern void Init_Can (void);;
extern void transmit_error(void);
extern void transmit_in(uint8_t *input);
extern void can_transmit (void);


#endif
#endif


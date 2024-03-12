
#ifndef _LIOP_H_
#define _LIOP_H_

#include "AllHeader.h"

#define true 1
#define false 0

#define BIT_0 0X01
#define BIT_1 0X02
#define BIT_2 0X04
#define BIT_3 0X08
#define BIT_4 0X10
#define BIT_5 0X20
#define BIT_6 0X40
#define BIT_7 0X80

#define T0START_1TO32 0x84 // Start Timer 0, prescaler 1 : 32/
#define T0H_500ms 0x0C	   // Timer 0 preset value 500 ms/
#define T0L_500ms 0x00
#define T1START_1TO1 0x03 //	0x81						// Start Timer 1, no prescaler
#define T1H_10ms 0x63	  // Timer 1 preset value 2 ms
#define T1L_10ms 0xC0
#define T1H_2ms 0xE0	   // 0xF0						// Timer 1 preset value 2 ms
#define T1L_2ms 0xC0	   // 0x60
#define T2START_1TO48 0x16 // 0x0E//0x16						// Start Timer 2, prescaler 1 : 16, postscale 1 : 3
#define T2_3ms 0xFF

#define MAX_ESE 110

#define SETID_NULL 0x00
#define SETID_BY_KEY 0x01
#define SETID_BY_CAN 0x02

/************************************************************************************************/
/* Definitions for input and output variables													*/
/************************************************************************************************/
#define IO_BASIC_FUNC 0 /* basic function of input / output		*/
#define IO_SUB_FUNC 1	/* sub function of input / output		*/
#define IO_LIFT 2		/* lift number of input / output		*/
#define IO_FLOOR 3		/* floor number of input / output		*/
#define IO_DOOR 4		/* door number of input / output		*/
#define IO_STATE 5		/* state of input / output				*/
#define IO_ENABLE 6
#define IO_ACK 7 /* acknowledgement marker for calls		*/


#define WD_MERKER 0xAA // restart by watchdog timer
#define BS_MERKER 0x55 // restart by CAN bus off
#define RC_MERKER 0x5A // restart by error in rc pointer
#define ID_MERKER 0xA5

#define SDO_TIMER 0x0A
#define MAX_IO 8
#define MAX_IO_TYPE 8

#define HSETIME 50 // 10
#define HEARTBEATTIME 4
#define ID_SET_TIME 50
#define MAX_LIFT 8
#define INPUT_DETECT_INTERVAL 7
#define ERROR_TIME 20
#define FLASH_TIME 6

//�����ʱ�䳣��
#define NO_SIGNAL_TIMEOUT 120
#define DARK_COUNT 8 // 5
#define CTE_FLAG_MODE 0x55

#define BACKLIGHT_NORMAL 0
#define BACKLIGHT_DARK 1
#define BACKLIGHT_OFF 2

#define KEYCODE_UP BIT_0
#define KEYCODE_DOWN BIT_1
#define KEYCODE_SYSSET BIT_2
#define NO_KEY 0xAA

enum
{
	NotUsed,
	KEYVALUE_UP,
	KEYVALUE_DOWN,
	KEYVALUE_SYSSET
};

//��ס����ɿ�ʼ�ظ���ʱ(��100MSΪ��λ)
#define ENABLEREPINTERVAL 5
//�ظ�����ʱ����(��100MSΪ��λ)
#define REPEATINTERVAL 2
#define LONGKEYTIME_SET 30

//Binz
#define OPEN_BL()																		 // LATAbits.LATA3 = 1
#define CLOSE_BL()																		 // LATAbits.LATA3 = 0
#define OPEN_FULL() HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)			 // LATEbits.LATE0 = 1
#define CLOSE_FULL() HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)		 // LATEbits.LATE0 = 0

#define OPEN_FIRE()	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)																	 // LATEbits.LATE1 = 1
#define CLOSE_FIRE() HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)

#define OPEN_INS() HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)
#define CLOSE_INS() HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)

#define OPEN_PACK() HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET)
#define CLOSE_PACK() HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET)



																	 // LATEbits.LATE1 = 0

#define OPEN_OUT_OF_ORDER() 	 // LATEbits.LATE1 = 1
#define CLOSE_OUT_OF_ORDER()  // LATEbits.LATE1 = 0

#define OPEN_GONG_UP() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // XGONG UP ON
#define CLOSE_GONG_UP() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); // XGONG UP OFF

#define OPEN_GONG_DOWN() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); // XGONG OFF ON
#define CLOSE_GONG_DOWN() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET); // XGONG OFF OFF

#define OPEN_INUSE()																	 // LATEbits.LATE2 = 1
#define CLOSE_INUSE()																	 // LATEbits.LATE2 = 0

#ifdef _MAIN_C_

uint8_t merker; // variable for restart

uint8_t node_id= 0x10; // node id of CAN
uint8_t disp_id_old = 0;

uint8_t id_buff[4];
uint16_t volatile nmtwait; // waiting time for 1. heartbeat

uint16_t volatile sdo_index = 0;
uint8_t volatile sdo_subindex = 0;
uint8_t sdo_timer = 0;		// timer for SDO segment transfer
uint8_t errorregister = 0;	// error register
uint16_t errorcode = 0;		// error code
uint32_t device_type;		// Device type and profile number
uint16_t virtual_device[3]; // Virtual device types + profile number

uint8_t volatile input[3]; // input variables for debouncing
uint8_t volatile in;	   // variable for input after debouncing
uint8_t inold;			   // variable to save old input state
uint8_t volatile out;	   // output state
uint8_t volatile inpush;   // push button inputs
uint8_t volatile outpush;  // push button indication
uint8_t in_polarity;	   // input is inverted / not inverted
uint8_t out_polarity;	   // output is inverted / not inverted

uint8_t row = 0;		 // row to switch on
uint8_t scroll = 0;		 // 1 : UP 	2: DN 	other: NO_Arrow
BYTE scrollpos = 0;	 // 1 more value to transmit
uint8_t scrolltimer = 0; // timer for scrolling arrows

uint8_t heartbeat; // timer for transmitting heartbeat
uint8_t input_detect = 0;

uint8_t landingcalltimer = 0; // timer for landing call misuse
uint8_t landingcallfloor = 0; // flor marker for landing call misuse
uint8_t direction_ind_mode = 0;

uint8_t virt_in[MAX_IO_TYPE];			 // virtual input (last changed)
uint8_t inpar[MAX_IO][MAX_IO_TYPE];		 // input parameter
uint8_t outpar[MAX_IO][MAX_IO_TYPE + 1]; // output parameter

uint8_t arrowtype = 0; // arrow type
uint8_t arrowflash = 0;
uint8_t arrowflash_old = 0;
uint8_t flashcontent = 0;
uint8_t mSwitch_code = 0;
uint8_t baudrate = 0;

uint8_t volatile hsetime;
uint8_t volatile hsecheck = 0;
uint8_t volatile hse_heartbeat = 0;
uint8_t volatile can_inittime = 0;
uint8_t can_passive_time = 0;
uint8_t display_scantimer = 0;
uint8_t hardware_version = 0;
uint8_t timer1_countH = 0;
uint8_t timer1_countL = 0;
uint8_t com_can_work = 0;
uint8_t keytimer = 0;
uint8_t flashtimer = 0;

uint8_t mKeyValue = 0;
uint8_t mKeyValue_Back = 0;
uint8_t mLongKeyTime = 0;
uint8_t timer_1S5 = 0;
uint8_t display_timer = 0;
uint8_t testno = 0;

uint8_t u8Start_Dark = 0;
uint8_t u8PWM_Count = 0;
uint8_t backlight_mode = 0;
uint8_t backlight_func = 0;
uint16_t backlight_off_time = 0;
uint16_t backlight_time_count = 0;
uint8_t light_para_ok = 0;
uint8_t display_STN_mode = 0;
// 0-->�������µ�¥�㲼��ͼ��ʾ,   ����"1"���ұ�
// 1 -->�Ľ�"17"����ʾ¥�㣬����"1"����ߣ�"17"��1���ұ�
// 2��3-->��������

uint8_t recei_monitor[8];

uint8_t const tKeyCode[] = {
	KEYCODE_UP,
	KEYCODE_DOWN,
	KEYCODE_SYSSET,
	NO_KEY,
};

struct
{
	uint8_t KeyScanEn : 1;	 //����ɨ��������־
	uint8_t KeyTrue : 1;	 //������Ч��־
	uint8_t LKeyScanEnd : 1; //������ɨ�������־
	uint8_t LongKeyEn : 1;	 //��������Ч��־
	uint8_t KeyAnswer : 1;	 //������Ӧ��־
	uint8_t KeyRepeat : 1;	 //�ظ�����ʱ
	uint8_t KeyRepeatEn : 1; //��ס��1��������ظ�
} bKey_Fg;

struct
{
	uint8_t Time_10ms : 1;	// 10ms ��־
	uint8_t Time_100ms : 1; // 100ms ��־
	uint8_t Time_500ms : 1; // 500ms ��־
	uint8_t Time_1s : 1;	// 500ms ��־
	uint8_t Time_2s : 1;
	uint8_t Time_5s : 1;
	uint8_t Time_alarm_fg : 1; //˾��������������־
	uint8_t dwin_500ms : 1;	   //˾��������������־
	uint8_t dwin_100ms : 1;	   //˾��������������־
} bTime;

#else

extern uint8_t input_detect;

extern uint8_t merker;
extern uint8_t scroll;

extern uint8_t node_id;
extern uint8_t disp_id;
extern uint8_t disp_id_old;
extern uint8_t disp_lift;
extern uint8_t nmtstate;
extern uint8_t setid_mode;
extern uint8_t setid_mode_old; // set liop can ID mode
extern uint8_t preset_node_id;

extern uint8_t id_buff[];
extern uint16_t volatile nmtwait;

extern uint16_t volatile sdo_index;
extern uint8_t volatile sdo_subindex;
extern uint8_t sdo_timer;
extern uint8_t errorregister;
extern uint16_t errorcode;
extern uint32_t device_type;
extern uint16_t virtual_device[];

extern uint8_t volatile input[];
extern uint8_t volatile in;
extern uint8_t inold;
extern uint8_t volatile input[];
extern uint8_t volatile in;
extern uint8_t inold;
extern uint8_t virt_in[];
extern uint8_t volatile out;
extern uint8_t volatile inpush;
extern uint8_t volatile outpush;
extern uint8_t in_polarity;
extern uint8_t out_polarity;
extern uint8_t landingcalltimer;
extern uint8_t landingcallfloor;
extern uint8_t direction_ind_mode;

extern uint8_t inpar[MAX_IO][MAX_IO_TYPE];
extern uint8_t outpar[MAX_IO][MAX_IO_TYPE + 1];

extern uint8_t volatile display[];
extern uint8_t arrowtype;
extern uint8_t arrowflash;
extern uint8_t arrowflash_old;
extern uint8_t flashcontent;
extern uint8_t mSwitch_code;

extern uint8_t row;			// row to switch on
extern uint8_t scroll;		// 1 : UP 	2: DN 	other: NO_Arrow
extern BYTE scrollpos;		// 1 more value to transmit
extern uint8_t scrolltimer; // timer for scrolling arrows

extern uint8_t heartbeat;

extern uint8_t volatile hsetime;
extern uint8_t volatile hsecheck;
extern uint8_t volatile hse_heartbeat;
extern uint8_t volatile can_inittime;
extern uint8_t can_passive_time;
extern uint8_t display_scantimer;

extern uint8_t mKeyValue;
extern uint8_t mKeyValue_Back;
extern uint8_t mLongKeyTime;

extern uint8_t hardware_version;
extern uint8_t timer1_countH;
extern uint8_t timer1_countL;
extern uint8_t com_can_work;
extern uint8_t keytimer;
extern uint8_t flashtimer;
extern uint8_t timer_1S5;
extern uint8_t display_timer;
extern uint8_t testno;
extern uint8_t u8Start_Dark;
extern uint8_t u8PWM_Count;
extern uint8_t backlight_mode;
extern uint8_t backlight_func;
extern uint16_t backlight_time_count;
extern uint16_t backlight_off_time;
extern uint8_t light_para_ok;

extern uint8_t recei_monitor[8];
extern uint8_t display_STN_mode;

extern uint8_t const tKeyCode[];
extern struct
{
	uint8_t KeyScanEn : 1;	 //����ɨ��������־
	uint8_t KeyTrue : 1;	 //������Ч��־
	uint8_t LKeyScanEnd : 1; //������ɨ�������־
	uint8_t LongKeyEn : 1;	 //��������Ч��־
	uint8_t KeyAnswer : 1;	 //������Ӧ��־
	uint8_t KeyRepeat : 1;	 //�ظ�����ʱ
	uint8_t KeyRepeatEn : 1; //��ס��1��������ظ�
} bKey_Fg;

extern struct
{
	uint8_t Time_10ms : 1;	// 10ms ��־
	uint8_t Time_100ms : 1; // 100ms ��־
	uint8_t Time_500ms : 1; // 500ms ��־
	uint8_t Time_1s : 1;	// 500ms ��־
	uint8_t Time_2s : 1;
	uint8_t Time_5s : 1;
	uint8_t Time_alarm_fg : 1; //˾��������������־
	uint8_t dwin_500ms : 1;	   //˾��������������־
	uint8_t dwin_100ms : 1;	   //˾��������������־

} bTime;

#endif
#endif

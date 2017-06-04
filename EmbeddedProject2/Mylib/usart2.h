#ifndef __USART2_H__
#define __USART2_H__

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)

/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)

/* ----------------------- Data Struct ------------------------------------- */
typedef struct
{
		struct
		{
			unsigned int  ch0;
			unsigned int ch1;
			unsigned int ch2;
			unsigned int ch3;
			unsigned int s1;
			unsigned int s2;
		}rc;
		struct
		{
			unsigned int x;
			unsigned int y;
			unsigned int z;
			unsigned int press_l;
			unsigned int press_r;
		}mouse;
		struct
		{
			unsigned int v;
		}key;
}RC_Ctl_t;

void USART2_Configuration(void);

#endif

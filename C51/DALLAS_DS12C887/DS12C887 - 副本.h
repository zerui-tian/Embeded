#include<main.h>

#define SEC_ADDR						0x00
#define SEC_ALARM_ADDR			0x01
#define MIN_ADDR						0x02
#define MIN_ALARM_ADDR			0x03
#define HOUR_ADDR						0x04
#define HOUR_ALARM_ADDR			0x05
#define DAY_OF_WEEK_ADDR		0x06
#define DAY_OF_MONTH_ADDR		0x07
#define MONTH_ADDR					0x08
#define YEAR_ADDR						0x09
#define	CENTURY_ADDR				0x32

#define REG_A								0x0A
#define REG_B								0x0B
#define REG_C								0x0C
#define	REG_D								0x0D

#define GETTIME_SIZE				8

#define REG_A_INIT					0x20
#define REG_B_INIT					0x10

extern void DS12C887_Init();
extern uchar* DS12C887_GetTime();
extern void DS12C887_SetTime(uchar* aim);
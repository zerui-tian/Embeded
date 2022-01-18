#include<main.h>

extern void delay_ms(uint z);
extern unsigned char LCD1602_ReadBusy();
extern void LCD1602_WriteCom(uchar com);
extern void LCD1602_WriteData(uchar dat);
extern void LCD1602_Init();
extern void LCD1602_Refresh(uchar stringOnLine1[], uchar stringOnLine2[]);
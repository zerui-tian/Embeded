/*********************************************************************
 *
 *                           top51.h
 *
 *********************************************************************
 * 文件名: qx51.h
 * 描  述: QX-X3开发板头文件
 * 公  司: 清翔电子科技
 * 作者              		日期         			注释
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 				    					原始文件
 * 刘清翔		14-2-13					v1.0
 *-------------------------------------------------------------------
 *功能：
 * 	QX-X3开发板初始化，和常用引脚定义！
 ********************************************************************/

#ifndef QX51_H
#define QX51_H

/*变量类型标识的宏定义,通常做法*/
#define uchar unsigned char
#define uint unsigned int
#define ON 1
#define OFF 0

/*定义晶振频率（单位 Hz）*/
#define FOSC  11059200 
//#define FOSC  12000000 
//#define FOSC  22118400 

/* QX-X3 引脚定义 */
sbit D0  = P0^0;
sbit D1  = P0^1;
sbit D2  = P0^2;
sbit D3  = P0^3;
sbit D4  = P0^4;
sbit D5  = P0^5;
sbit D6  = P0^6;
sbit D7  = P0^7;

sbit rs  = P3^5;   //RS为寄存器选择，1=数据寄存器、0=指令寄存器。
sbit rw  = P3^6;   //RW为读写信号线，1=读操作，0=写操作。
                   //当RS=RW=0可以写入指令或者显示地址，
                   //当RS=0，RW=1可以读忙信号，
                   //当RS=1，RW=0可以写入数据。

sbit LCDEN = P3^4;            //液晶使能控制
sbit LCD_PSB = P3^1;            //串/并方式控制
sbit wela = P2^6;
sbit dula = P2^7;

sbit BUZZ = P3^7; //蜂鸣器

sbit DC_MOTOR = P1^4; //直流电机

//位寻址寄存器definition
sbit ACC_7=ACC^7;
sbit ACC_0=ACC^0;

//DS1302IO definition
sbit SCLK=P2^5;
sbit DIO=P2^4; //1-write总线IO定义
sbit CE=P2^3;

//为了方便使用 IO口多重定义
sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;
sbit LED4 = P1^3;
sbit LED5 = P1^4;
sbit LED6 = P1^5;
sbit LED7 = P1^6;
sbit LED8 = P1^7;

//NRF引脚定义
/**************************************************/
sbit NRF_CE =  P1^2;
sbit CSN=  P1^3;
sbit SCK=  P1^7;
sbit MOSI= P1^5;
sbit MISO= P1^6;
sbit IRQ = P1^4;


//系统初始化函数
void qx51_init(void);/*qx51主板初始化*/


#endif
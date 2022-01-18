/*********************************************************************
 *
 *                           top51.h
 *
 *********************************************************************
 * �ļ���: qx51.h
 * ��  ��: QX-X3������ͷ�ļ�
 * ��  ˾: ������ӿƼ�
 * ����              		����         			ע��
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 				    					ԭʼ�ļ�
 * ������		14-2-13					v1.0
 *-------------------------------------------------------------------
 *���ܣ�
 * 	QX-X3�������ʼ�����ͳ������Ŷ��壡
 ********************************************************************/

#ifndef QX51_H
#define QX51_H

/*�������ͱ�ʶ�ĺ궨��,ͨ������*/
#define uchar unsigned char
#define uint unsigned int
#define ON 1
#define OFF 0

/*���徧��Ƶ�ʣ���λ Hz��*/
#define FOSC  11059200 
//#define FOSC  12000000 
//#define FOSC  22118400 

/* QX-X3 ���Ŷ��� */
sbit D0  = P0^0;
sbit D1  = P0^1;
sbit D2  = P0^2;
sbit D3  = P0^3;
sbit D4  = P0^4;
sbit D5  = P0^5;
sbit D6  = P0^6;
sbit D7  = P0^7;

sbit rs  = P3^5;   //RSΪ�Ĵ���ѡ��1=���ݼĴ�����0=ָ��Ĵ�����
sbit rw  = P3^6;   //RWΪ��д�ź��ߣ�1=��������0=д������
                   //��RS=RW=0����д��ָ�������ʾ��ַ��
                   //��RS=0��RW=1���Զ�æ�źţ�
                   //��RS=1��RW=0����д�����ݡ�

sbit LCDEN = P3^4;            //Һ��ʹ�ܿ���
sbit LCD_PSB = P3^1;            //��/����ʽ����
sbit wela = P2^6;
sbit dula = P2^7;

sbit BUZZ = P3^7; //������

sbit DC_MOTOR = P1^4; //ֱ�����

//λѰַ�Ĵ���definition
sbit ACC_7=ACC^7;
sbit ACC_0=ACC^0;

//DS1302IO definition
sbit SCLK=P2^5;
sbit DIO=P2^4; //1-write����IO����
sbit CE=P2^3;

//Ϊ�˷���ʹ�� IO�ڶ��ض���
sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;
sbit LED4 = P1^3;
sbit LED5 = P1^4;
sbit LED6 = P1^5;
sbit LED7 = P1^6;
sbit LED8 = P1^7;

//NRF���Ŷ���
/**************************************************/
sbit NRF_CE =  P1^2;
sbit CSN=  P1^3;
sbit SCK=  P1^7;
sbit MOSI= P1^5;
sbit MISO= P1^6;
sbit IRQ = P1^4;


//ϵͳ��ʼ������
void qx51_init(void);/*qx51�����ʼ��*/


#endif
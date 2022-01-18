#ifndef __VKL144_H__
#define __VKL144_H__

//�˰����ļ����ݿͻ���Ƭ������Ӧ���޸�
#include "reg51.h"

//��������
#define VKL144_ADDR   			0x7c    // IIC��ַ
#define VKL144_ICSET       		0xe8    // IC����  bit0ʱ��Դ  bit1��λ	bit2ӳ���ַbit5
										// Ĭ��Ϊ0 �ڲ�RC����  ��ִ�������λ  
#define BIT0_CLKEXT       		0x01    // �ⲿʱ��
#define BIT1_ENSRST       		0x02    // ִ�������λ
#define BIT2_ADDR5	       		0x04    // ӳ���ַbit5
#define VKL144_ADSET       		0x00    // ӳ����ʼ��ַbit4~bit0
#define VKL144_MODESET_1_3_ON  	0xc8    // ����ʾ 1/3bias
#define VKL144_MODESET_1_3_OFF 	0xc0    // �ر���ʾ 1/3bias
#define VKL144_MODESET_1_2_ON  	0xcc    // ����ʾ 1/2bias
#define VKL144_MODESET_1_2_OFF 	0xc4    // �ر���ʾ 1/2bias

//��ʾ��������
#define VKL144_DISCTL			0xA0  	//bit7~bit5=101

//bit4-bit3ʡ��ģʽFR
#define VKL144_FRNOR       		0x00  	//bit4-bit3=00   FR NORMAL 				�ϵ�Ĭ�� 
#define VKL144_FRPM1       		0x08  	//bit4-bit3=01   FR POWER SAVE MODE1
#define VKL144_FRPM2       		0x10  	//bit4-bit3=10   FR POWER SAVE MODE2
#define VKL144_FRPM3       		0x18  	//bit4-bit3=11   FR POWER SAVE MODE3 	��ʡ��

//bit2 LINE,FRAME��������LINE ��תʱ�Ĺ�������>FRAME��ת�ĵ���
#define VKL144_LINER       		0x00  	//bit2=0   LINE��ת	 					�ϵ�Ĭ��
#define VKL144_FRAMER      		0x04  	//bit2=1   FRAME��ת 					��ʡ��

//bit1-bit0ʡ��ģʽSR
#define VKL144_SRHP        		0x03  	//bit1-bit0=11   SR NORMAL 				���
#define VKL144_SRNOR       		0x02  	//bit1-bit0=10   SR POWER SAVE MODE1 	�ϵ�Ĭ�� 
#define VKL144_SRPM2       		0x01  	//bit1-bit0=01   SR POWER SAVE MODE2
#define VKL144_SRPM1       		0x00  	//bit1-bit0=00   SR POWER SAVE MODE1 	��ʡ��

//������ʾ�������
//VKL144_DISCTL|VKL144_FRNOR|VKL144_LINER |VKL144_SRHP      //�������
//VKL144_DISCTL|VKL144_FRPM1|VKL144_FRAMER|VKL144_FRAMER    //��������
//VKL144_DISCTL|VKL144_FRPM3|VKL144_FRAMER|VKL144_SRPM1    	//������ʡ

//������������
#define	VKL144_DISCTL_DATA		VKL144_DISCTL | VKL144_FRPM3 | VKL144_FRAMER | VKL144_SRPM1

//��˸����
#define VKL144_BLKCTL_OFF      	0xF0  						// �ر���˸
#define VKL144_BLKCTL_05HZ     	0xF1  						// ��˸Ƶ��Ϊ0.5HZ
#define VKL144_BLKCTL_1HZ      	0xF2  						// ��˸Ƶ��Ϊ1HZ
#define VKL144_BLKCTL_2HZ      	0xF3  						// ��˸Ƶ��Ϊ2HZ

//ȫ��ǿ�п����,����ʾ�ڴ������޹�,����������ȫ����Ϊ����ִ��
#define VKL144_APCTL_APON_ON   	0xFE 						// ȫ��ǿ��ȫ��ʾ_��
#define VKL144_APCTL_APON_OFF  	0xFC 						// ȫ��ǿ��ȫ��ʾ_��
#define VKL144_APCTL_APOFF_ON  	0xFD 						// ȫ��ǿ�й���ʾ_��
#define VKL144_APCTL_APOFF_OFF 	0xFC 						// ȫ��ǿ�й���ʾ_��

//����seg��
#define 	VKL144_SEGNUM		16
#define 	VKL144_RAMBUFNUM	VKL144_SEGNUM/2				//ż����2��������2+1

//1/3bias		1/2bias		1=��Ч		0=��Ч
#define		BIAS_1_2			0
#define		BIAS_1_3			1


//���¹ܽ����ø��ݿͻ���Ƭ������Ӧ���޸�
sbit VKL144_I2C_SDA_PIN      =	P0^1;
sbit VKL144_I2C_SCL_PIN      =	P0^2;

#define VKL144_SDA_H()			VKL144_I2C_SDA_PIN = 1
#define VKL144_SDA_L()			VKL144_I2C_SDA_PIN = 0

#define VKL144_SCL_H()			VKL144_I2C_SCL_PIN = 1
#define VKL144_SCL_L()			VKL144_I2C_SCL_PIN = 0
#define VKL144_GET_SDA() 		VKL144_I2C_SDA_PIN

#define VKL144_SET_SDA_IN() 	{}	//��������
#define VKL144_SET_SDA_OUT() 	{}	//�������

/* Exported functions ------------------------------------------------------- */
void VKL144_Delay_us(unsigned int ucDat);
void VKL144_Delay_ms(unsigned int ucDat);

void VKL144_I2CStart( void );
void VKL144_I2CStop( void );
unsigned char VKL144_I2CSlaveAck( void );
void VKL144_I2CWriteByte( unsigned char byte );
void VKL144_FunSet(unsigned char command);

void Write1DataVKL144(unsigned char Addr,unsigned char Dat);
void WritenDataVKL144(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt);
void VKL144_DisAll(unsigned char state);
void VKL144_DisSegOn(unsigned char Seg,unsigned char Com);
void VKL144_DisSegOff(unsigned char Seg,unsigned char Com);

void VKL144_Halt(void);
void VKL144_Init(void);
void VKL144_num_init(void);

void DispJUHUI(void);
void VKL144_Test(void);
void VKL144_Display(void);

#endif  /*__VKL144_H*/
/************************END OF FILE****/
















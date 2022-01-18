#ifndef __HT1621_H__
#define __HT1621_H__

//�˰����ļ����ݿͻ���Ƭ������Ӧ���޸�
#include "reg51.h"

//����LCDͼֽ����дSEG�ĸ���
#define HT1621_SEGNUM			8

//����LCDͼֽ��ѡ���Ӧ�Ĺ�����ʽ	1=��Ч	0=��Ч
#define	COM_4_BIAS_1_3			1
#define	COM_3_BIAS_1_3			0
#define	COM_2_BIAS_1_3			0

#define	COM_4_BIAS_1_2			0
#define	COM_3_BIAS_1_2			0
#define	COM_2_BIAS_1_2			0

//��������
#define OSC_OFF    				0x00        // �ر�����
#define OSC_ON    				0x01        // ��������
#define DISP_OFF 				0x02		// ��LCD Bias
#define DISP_ON   				0x03        // ��LCD Bias
#define COM_1_3__4  			0x29  		// 1/3bias 4com
#define COM_1_3__3  			0x25  		// 1/3bias 3com
#define COM_1_3__2  			0x21  		// 1/3bias 2com
#define COM_1_2__4  			0x28  		// 1/2bias 4com
#define COM_1_2__3  			0x24  		// 1/2bias 3com
#define COM_1_2__2  			0x20  		// 1/2bias 2com

//��չ����ù���δ�����رտɽ��͹���
#define TIMER_DIS   			0x04        // ��ʹ time base ݔ��
#define WDT_DIS   				0x05        // ��ʹ WDT ��ͣ���ݔ��
#define BUZZ_OFF  				0x08        // �رշ����� 
#define RC32K  					0X18       	// 
#define IRQ_DIS  				0X80        // ��ʹ IRQ ݔ��

//���¹ܽ����ø��ݿͻ���Ƭ������Ӧ���޸� 
sbit  	HT1621_CS_PIN		=	P0^0;
sbit 	HT1621_WR_PIN		=	P0^1;
sbit 	HT1621_DAT_PIN		=	P0^2;

//���¹ܽ����������ݿͻ���Ƭ������Ӧ���޸�
#define HT1621_CS_H()			HT1621_CS_PIN = 1
#define HT1621_CS_L() 			HT1621_CS_PIN = 0

#define HT1621_WR_H() 			HT1621_WR_PIN = 1
#define HT1621_WR_L() 			HT1621_WR_PIN = 0

#define HT1621_DATA_H() 		HT1621_DAT_PIN = 1
#define HT1621_DATA_L() 		HT1621_DAT_PIN = 0

/* Exported functions ------------------------------------------------------- */
void HT1621_Delay(unsigned int ucDat);
void HT1621_Delay_ms(unsigned int ucDat);
void WriteClockHT1621(void);

void WriteCommandHT1621(unsigned char FunctonCode);
void Write1DataHT1621(unsigned char Addr,unsigned char Dat);
void WritenDataHT1621(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt);

void HT1621_DisAll(unsigned char state);
void HT1621_DisSegOn(unsigned char Seg,unsigned char Com);
void HT1621_DisSegOff(unsigned char Seg,unsigned char Com);

void HT1621_Halt(void);
void HT1621_Init(void);
void HT1621_num_init(void);
void HT1621_Test(void);

void HT1621_Display(void);

#endif  /*__HT1621_H*/

/************************END OF FILE****/











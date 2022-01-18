#include "HT1621.h"
#include "intrins.h"

#define HT1621_BASE100MS	100		//��HT1621_Delayʵ��100mS��ʱ�����ݵ�Ƭ���޸Ĵ˳���
#define HT1621_CLK			10		//WR�ź���Ƶ��,�� HT1621_Delay_us ʵ�֣�����100K	1/0.1M=10us

const unsigned char HT1621_segtab[HT1621_SEGNUM]=
{
	0,1,2,3,4,5,6,7
};
unsigned char HT1621_maxcom=4;					//������com��������4com��3com��2com
unsigned char HT1621_addrbit=6;					//����ic��ram��ַλ�� A5-A0
unsigned char HT1621_dispram[HT1621_SEGNUM];	//��ʾRAM bufferֻʹ�õ�4λ��bit0->com0,bit1->com1,bit2->com2,bit3->com3
unsigned char HT1621_segi,HT1621_comi;

/*************************************************************
*�������ƣ�HT1621_Delay_us
*�������ܣ�Inserts a delay time
*���������ucDat-> ��ʱʱ��
*���������None
*��    ע���˳�����Ҫ����У׼	ʵ�� 10us ��ʱ
**************************************************************/
void HT1621_Delay_us(unsigned int ucDat)
{
	while(ucDat--)
	{
		_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();
	}
}
/*************************************************************
*�������ƣ�HT1621_Delay_ms
*�������ܣ�Inserts a delay time
*���������ucDat-> ��ʱʱ��
*���������None
*��    ע���˳�����Ҫ����У׼ ʵ�� 1ms ��ʱ
**************************************************************/
void HT1621_Delay_ms(unsigned int ucDat)
{
	unsigned int a,b;
	
	for(a=ucDat; a>0; a--)
	{
		for(b=122; b>0; b--);
	}
}
/**
  * @brief  HT1621 clk
  * @param  None
  * @retval None
  */
void WriteClockHT1621(void)
{
	HT1621_WR_L();
	HT1621_Delay_us(HT1621_CLK);
	HT1621_WR_H();
	HT1621_Delay_us(HT1621_CLK);
}
/**
  * @brief  Write HT1621 Command
  * @param  FunctonCode->����/������ 
  * @retval None
  */
void WriteCommandHT1621(unsigned char FunctonCode)
{
	unsigned char Shift = 0x80;
	unsigned char i;
	
	HT1621_CS_L();					//CS Ƭѡ��
	HT1621_Delay_us(HT1621_CLK);
	
	HT1621_DATA_H();				//100
	WriteClockHT1621();
	HT1621_DATA_L();
	WriteClockHT1621();
	HT1621_DATA_L();
	WriteClockHT1621();
	
	for(i = 0; i < 8; i++) 
	{
		if(Shift & FunctonCode)
			HT1621_DATA_H();
		else
			HT1621_DATA_L();
		
		 WriteClockHT1621();
		 Shift = Shift >> 1;
	}
	HT1621_DATA_L(); 
	WriteClockHT1621();				//����һ�� 0 �����������һλ X
	
	HT1621_CS_H(); 					//CS Ƭѡ��
	HT1621_Delay_us(HT1621_CLK);
	HT1621_DATA_H();
}
/**
  * @brief  Write 1 data to HT1621
  * @param  Addr->д��ram�ĵ�ַ 
  * @param  Databuf->д��ram������
  * @retval None
  */
void Write1DataHT1621(unsigned char Addr,unsigned char Dat)
{
	unsigned char i; 
	unsigned char Shift;
	
	HT1621_CS_L();  				//CS Ƭѡ��
	HT1621_Delay_us(HT1621_CLK);
	
	HT1621_DATA_H();  				//101
	WriteClockHT1621();
	HT1621_DATA_L();
	WriteClockHT1621();
	HT1621_DATA_H();
	WriteClockHT1621();
	
	Shift = 0x20; 					//���θ�3λ ֻ��5λ
	for( i = 0; i < HT1621_addrbit; i++) 
	{
		if(Addr & Shift)
			HT1621_DATA_H();
		else
			HT1621_DATA_L();
		WriteClockHT1621();
		Shift = Shift >> 1;
	}
	
	Shift = 0x01;
	for (i = 0; i < 4; i++)
	{
		if( Dat & Shift) 
			HT1621_DATA_H();
		else
			HT1621_DATA_L();
		WriteClockHT1621();
		Shift = Shift << 1;
	}   
	HT1621_CS_H(); 					//CS Ƭѡ��
	HT1621_Delay_us(HT1621_CLK);
	HT1621_DATA_H();
}
/**
  * @brief  Write n data to HT1621 
  * @param  Addr->д��ram����ʼ��ַ 
  * @param  Databuf->д��ram������buffer
  * @param  Cnt->д��ram�����ݸ���
  * @retval None
  */
void WritenDataHT1621(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char i,j;
	unsigned char Shift;
	unsigned char dataval;
	
	HT1621_CS_L();   				//CS Ƭѡ��
	HT1621_Delay_us(HT1621_CLK);
	
	HT1621_DATA_H();  		  		//101
	WriteClockHT1621();
	HT1621_DATA_L();  
	WriteClockHT1621();
	HT1621_DATA_H();  
	WriteClockHT1621();
	
	Shift = 0x20; 					//���θ�3λ ֻ��5λ
	for( i = 0; i < HT1621_addrbit; i++) 
	{
		if (Addr & Shift)
			HT1621_DATA_H();
		else
			HT1621_DATA_L();
		WriteClockHT1621();
		Shift = Shift >> 1;
	}
	
	for (j = 0; j < Cnt; j++) 
	{
		Shift = 0x01;
		dataval=*Databuf++;
		for (i = 0; i < 4; i++) 
		{
			if( dataval & Shift) 
				HT1621_DATA_H();
			else
				HT1621_DATA_L();
			WriteClockHT1621();
			Shift = Shift << 1;
		}
	}
	HT1621_CS_H();   				//CS Ƭѡ��
	HT1621_Delay_us(HT1621_CLK);
	HT1621_DATA_H();
}
/**
  * @brief  lcdȫ�Ի�ȫ��
  * @param  state=1->lcdȫ�� 
  * @param  state=0->lcdȫ��
  * @retval None
  */
void HT1621_DisAll(unsigned char state)
{
	unsigned char segi;
	unsigned char dispram[32];
	
	for(segi=0;segi<32;segi++)
	{
		dispram[segi]=0;
	}
	for(segi=0;segi<HT1621_SEGNUM;segi++)
	{
		if(state==1)
		{
			if(HT1621_maxcom==2)
				HT1621_dispram[segi]=0x03;
			else if(HT1621_maxcom==3)
				HT1621_dispram[segi]=0x07;
			else if(HT1621_maxcom==4)
				HT1621_dispram[segi]=0x0f;
		}
		else
		{
			HT1621_dispram[segi]=0x00;
		}
		dispram[HT1621_segtab[segi]] = HT1621_dispram[segi];
	}
	WritenDataHT1621(0,dispram,32);
}
/**
  * @brief  lcd��ʾ�����㣨1comx1seg��
  * @param  Seg->���Ӧ��seg 
  * @param  Com->���Ӧ��com
  * @retval None
  */
void HT1621_DisSegOn(unsigned char Seg,unsigned char Com)
{
	HT1621_dispram[Seg-HT1621_segtab[0]] |= (1<<Com);
	Write1DataHT1621(Seg,HT1621_dispram[Seg-HT1621_segtab[0]]);
}
/**
  * @brief  lcd�رյ����㣨1comx1seg��
  * @param  None 
  * @retval None
  */
void HT1621_DisSegOff(unsigned char Seg,unsigned char Com)
{
	HT1621_dispram[Seg-HT1621_segtab[0]] &= ~(1<<Com);
	Write1DataHT1621(Seg,HT1621_dispram[Seg-HT1621_segtab[0]]);
}
/**
  * @brief  HT1621�������͹���ģʽ
  * @param  None 
  * @retval None
  */
void HT1621_Halt(void)
{	
	//����ʹ������LCDOFF��SYSDIS,ϵͳ�����ڵ���͹���״̬��
	//ֻ��ʹ��Ƭ��RCʱ��Դʱ,����ʹϵͳ�������͹���״̬��
	WriteCommandHT1621(OSC_OFF);
	WriteCommandHT1621(DISP_OFF);
}
/**
  * @brief  HT1621 Init
  * @param  None
  * @retval None
  */
void HT1621_Init(void)	//�˳�����͹���״̬���³�ʼ��HT1621
{		
	HT1621_CS_H();
	HT1621_WR_H();
	HT1621_DATA_H();
	
	WriteCommandHT1621(OSC_ON);
	WriteCommandHT1621(DISP_ON);
	
	#if	COM_4_BIAS_1_3
	WriteCommandHT1621(COM_1_3__4);HT1621_maxcom=4;	//1/3bias 4com
	#endif
	
	#if	COM_3_BIAS_1_3
	WriteCommandHT1621(COM_1_3__3);HT1621_maxcom=3;	//1/3bias 3com
	#endif
	
	#if	COM_2_BIAS_1_3
	WriteCommandHT1621(COM_1_3__2);HT1621_maxcom=2;	//1/3bias 2com
	#endif
	
	#if	COM_4_BIAS_1_2
	WriteCommandHT1621(COM_1_2__4);HT1621_maxcom=4;	//1/2bias 4com
	#endif
	
	#if	COM_3_BIAS_1_2
	WriteCommandHT1621(COM_1_2__3);HT1621_maxcom=3;	//1/2bias 3com
	#endif
	
	#if	COM_2_BIAS_1_2
	WriteCommandHT1621(COM_1_2__2);HT1621_maxcom=2;	//1/2bias 2com
	#endif
	
	//�ϵ�Ĭ������(����δ�ù��ܹرս��͹���)
	
	WriteCommandHT1621(TIMER_DIS);
	WriteCommandHT1621(WDT_DIS);
	WriteCommandHT1621(BUZZ_OFF);
	WriteCommandHT1621(IRQ_DIS);
	
	HT1621_num_init();		//�Զ�����Һ������Ҫ������
}
/**
  * @brief  ���ݽ������Զ�����Һ����Ҫ�����飬�˳�����Ҫ����Һ��ͼֽ�޸�
  * @param  None
  * @retval None
  */
const unsigned char LED_Tab[]=
{
	0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,	//0-15 = 0--f
	0x58,	//16=c
	0x37,	//17=N
	0x40,	//18=-
	0x0E,	//19=J
	0x3E,	//20=U
	0x76,	//21=H
	0x30,	//22=I
};
unsigned char LCD_RAM_Tab[sizeof(LED_Tab)];

#define		LCD_A			0x01
#define		LCD_B			0x02
#define		LCD_C			0x04
#define		LCD_D			0x08
#define		LCD_E			0x10
#define		LCD_F			0x20
#define		LCD_G			0x40
#define		LCD_P			0x80

void HT1621_num_init(void)
{
	unsigned char num,LCD_data;
	
	for(num=0; num<sizeof(LED_Tab); num++)
	{
		LCD_data = 0;
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_P)) << 7);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_E)) << 6);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_F)) << 5);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_A)) << 4);
		
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_D)) << 3);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_C)) << 2);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_G)) << 1);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_B)) << 0);
		
		LCD_RAM_Tab[num] = LCD_data;
	}
}

void HT1621_Test(void)
{
	HT1621_DisAll(1);
	HT1621_Delay_ms(1000);
	HT1621_DisAll(0);
	HT1621_Delay_ms(1000);
	
	for(HT1621_segi=0;HT1621_segi<HT1621_SEGNUM;HT1621_segi++)		//seg
	{
		for(HT1621_comi=0;HT1621_comi<HT1621_maxcom;HT1621_comi++)	//com
		{
			HT1621_DisSegOn(HT1621_segtab[HT1621_segi],HT1621_comi);//LCD����seg����
			HT1621_Delay_ms(100);
			HT1621_DisAll(0);
		}
	}
	
	HT1621_DisAll(1);
	HT1621_Delay_ms(1000);
	for(HT1621_segi=0;HT1621_segi<HT1621_SEGNUM;HT1621_segi++)		//seg
	{
		for(HT1621_comi=0;HT1621_comi<HT1621_maxcom;HT1621_comi++)	//com
		{
			HT1621_DisSegOff(HT1621_segtab[HT1621_segi],HT1621_comi);//LCD����seg�ر�
			HT1621_Delay_ms(100);
			HT1621_DisAll(1);
		}
	}
	HT1621_Delay_ms(1000);
	HT1621_DisAll(0);
}
/**
  * @brief  �û���ʾ������Ҫ�Լ�����
  * @param  None
  * @param  None
  * @retval None
  */
void HT1621_Display(void)
{
	unsigned int  Temp=1234;
	unsigned char Temp_Data;
	
	Temp_Data = Temp / 1000;
	Write1DataHT1621(HT1621_segtab[0],	LCD_RAM_Tab[Temp_Data]>>4);
	Write1DataHT1621(HT1621_segtab[1],	LCD_RAM_Tab[Temp_Data]>>0);
	
	Temp_Data = Temp % 1000 / 100;
	Write1DataHT1621(HT1621_segtab[2],	LCD_RAM_Tab[Temp_Data]>>4);
	Write1DataHT1621(HT1621_segtab[3],	LCD_RAM_Tab[Temp_Data]>>0);
	
	Temp_Data = Temp % 1000 % 100 / 10;
	Write1DataHT1621(HT1621_segtab[4],	LCD_RAM_Tab[Temp_Data]>>4);
	Write1DataHT1621(HT1621_segtab[5],	LCD_RAM_Tab[Temp_Data]>>0);
	
	Temp_Data = Temp % 1000 % 100 % 10;
	Write1DataHT1621(HT1621_segtab[6],	LCD_RAM_Tab[Temp_Data]>>4);
	Write1DataHT1621(HT1621_segtab[7],	LCD_RAM_Tab[Temp_Data]>>0);
}

/************************END OF FILE****/














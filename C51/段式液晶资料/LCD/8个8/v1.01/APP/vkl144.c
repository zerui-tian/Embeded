#include "VKL144.h"
#include "intrins.h"

#define	RESET				0
#define	SET					1

#define VKL144_BASE100MS	100	 	//ʵ��100mS��ʱ�����ݵ�Ƭ���޸Ĵ˳���
#define VKL144_CLK			1		//WR�ź���Ƶ�� ,��VKL144_Delay_usʵ�֣� =1->400kHz =10->100kHz =100->11.11kHz

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

const unsigned char VKL144_segtab[VKL144_SEGNUM]=
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};
unsigned char VKL144_addrbit=6;			//����ic��ram��ַ����A5-A0
unsigned char VKL144_segi,VKL144_comi;
unsigned char VKL144_dispram[VKL144_RAMBUFNUM];
//��ʾRAM bufferʹ��8λ��bit0,4->com0,bit1,5->com1,bit2,6->com2,bit37->com3
//��4λż��seg����4λ����seg
//����Ǵ�����seg��ʼVKL144_dispram[0]�ĸ�4λ����



/*************************************************************
*�������ƣ�VKL144_Delay_us
*�������ܣ�Inserts a delay time
*���������ucDat-> ��ʱʱ��
*���������None
*��    ע���˳�����Ҫ����У׼	ʵ�� 2us ��ʱ
**************************************************************/
void VKL144_Delay_us(unsigned int ucDat)
{
	while(ucDat--) 
	{//nopָ����ݵ�Ƭ������Ӧ���޸�
		_nop_();_nop_();_nop_();_nop_();_nop_();
	}
}
/*************************************************************
*�������ƣ�VKL144_Delay_ms
*�������ܣ�Inserts a delay time
*���������ucDat-> ��ʱʱ��
*���������None
*��    ע���˳�����Ҫ����У׼ ʵ�� 1ms ��ʱ
**************************************************************/
void VKL144_Delay_ms(unsigned int ucDat)
{
	unsigned int a,b;
	
	for(a=ucDat; a>0; a--)
	{
		for(b=122; b>0; b--);
	}
}
/*************************************************************
*�������ƣ�I2CStart
*�������ܣ�I2C��ʼ�ź�
*���������None
*���������None
*��    ע��ʱ���߸�ʱ���������ɸߵ��͵����䣬��ʾI2C��ʼ�ź�
**************************************************************/
void VKL144_I2CStart( void )
{
	VKL144_SDA_H();
	VKL144_SCL_H();
	VKL144_Delay_us(VKL144_CLK/2);
	VKL144_SDA_L();
	VKL144_Delay_us(VKL144_CLK/2);
	VKL144_SCL_L();
}
/*************************************************************
*�������ƣ�I2CStop
*�������ܣ�I2Cֹͣ�ź�
*���������None
*���������None
*��    ע��ʱ���߸�ʱ���������ɵ͵��ߵ����䣬��ʾI2Cֹͣ�ź�
**************************************************************/
void VKL144_I2CStop( void )
{
	VKL144_SDA_L();
	VKL144_Delay_us(VKL144_CLK/2);
	VKL144_SCL_H();
	VKL144_Delay_us(VKL144_CLK/2);
	VKL144_SDA_H();
}
/*************************************************************
*�������ƣ�I2CSlaveAck
*�������ܣ�I2C�ӻ��豸Ӧ���ѯ
*���������None
*���������None
*��    ע��
**************************************************************/
unsigned char VKL144_I2CSlaveAck( void )
{
	unsigned int TimeOut;
	unsigned char RetValue;
	
	VKL144_SET_SDA_IN();
	VKL144_Delay_us(VKL144_CLK/2);
	VKL144_SCL_H();
	
	TimeOut = 10000;
	while( TimeOut-- > 0 )
	{
		if( VKL144_GET_SDA() )	//SDAΪ�ߣ�δ�յ�Ӧ��
		{
			RetValue = RESET;
		}
		else					//SDAΪ�ͣ��յ�Ӧ��
		{
			RetValue = SET;
			break;
		}
	}
	VKL144_SCL_L();
	VKL144_SET_SDA_OUT();
	
	return RetValue;
}
/*************************************************************
*�������ƣ�I2CWriteByte
*�������ܣ�I2Cдһ�ֽ�����
*���������None
*���������None
*��    ע��
**************************************************************/
void VKL144_I2CWriteByte( unsigned char byte )
{
	unsigned char i=8;
	while (i--)
	{
		if(byte&0x80)
			VKL144_SDA_H();
		else
			VKL144_SDA_L();
		byte<<=1; 
		VKL144_Delay_us(VKL144_CLK);
		VKL144_SCL_H();     
		VKL144_Delay_us(VKL144_CLK); 
		VKL144_SCL_L();
	}
}
/*************************************************************
*�������ƣ�FunSet
*�������ܣ���������
*���������command-����ָ�� 
*���������None
*��    ע��
**************************************************************/
void VKL144_FunSet(unsigned char command)
{ 
	unsigned char i;
	
	i=1;
	while(i--)
	{ 
		VKL144_Delay_ms(VKL144_BASE100MS/10);
		VKL144_I2CStop(); 
		VKL144_Delay_ms(VKL144_BASE100MS/20);
		VKL144_I2CStart(); 
		VKL144_I2CWriteByte(VKL144_ADDR); 
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		VKL144_I2CWriteByte(command); 
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		VKL144_I2CStop(); 
	}
	VKL144_I2CStop();
}
/**
  * @brief  Write 1 data to VKL144 
  * @param  Addr->д��ram�ĵ�ַ 
  * @param  Databuf->д��ram������
  * @retval None
  */
void Write1DataVKL144(unsigned char Addr,unsigned char Dat)
{
	unsigned char i;
	
	i=1;
	while(i--)
	{ 
		VKL144_Delay_ms(VKL144_BASE100MS/10);
		VKL144_I2CStop(); 
		VKL144_Delay_ms(VKL144_BASE100MS/20);
		VKL144_I2CStart(); 
		VKL144_I2CWriteByte(VKL144_ADDR);
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		if(Addr>0x1f)
			VKL144_I2CWriteByte(VKL144_ICSET|BIT2_ADDR5); 
		else
			VKL144_I2CWriteByte(VKL144_ICSET);
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		VKL144_I2CWriteByte(Addr&0x1f);
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		
		VKL144_I2CWriteByte(Dat);
		if( SET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
	}
	VKL144_I2CStop();
}
/**
  * @brief  Write n data to VKL144 
  * @param  Addr->д��ram����ʼ��ַ 
  * @param  Databuf->д��ram������buffer
  * @param  Cnt->д��ram�����ݸ���
  * @retval errorflag
  */
void WritenDataVKL144(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char i,n;
	
	i=1;
	while(i--)
	{ 
		VKL144_Delay_ms(VKL144_BASE100MS/10);
		VKL144_I2CStop(); 
		VKL144_Delay_ms(VKL144_BASE100MS/20);
		VKL144_I2CStart(); 
		VKL144_I2CWriteByte(VKL144_ADDR); 
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		if(Addr>0x1f)
			VKL144_I2CWriteByte(VKL144_ICSET|BIT2_ADDR5); 
		else
			VKL144_I2CWriteByte(VKL144_ICSET); 
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		VKL144_I2CWriteByte(Addr&0x1f); 
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		
		for(n=0;n<Cnt;n++)
		{ 
			VKL144_I2CWriteByte(*Databuf++);
			if( SET == VKL144_I2CSlaveAck() )
			{
				continue;
			}
			else
			{
				break;
			}
		}
	 }
	 VKL144_I2CStop();
}
/**
  * @brief  lcdȫ�Ի�ȫ��
  * @param  state=1->lcdȫ�� 
  * @param  state=0->lcdȫ��
  * @retval None
  */
void VKL144_DisAll(unsigned char state)
{
	unsigned char segi;
	unsigned char dispram[18];
	
	for(segi=0;segi<18/2;segi++)
	{
		dispram[segi]=0;
	}
	for(segi=0;segi<VKL144_RAMBUFNUM;segi++)
	{
		if(state==1)
		{
				VKL144_dispram[segi]=0xff;
		}
		else
		{
			VKL144_dispram[segi]=0x00;
		}
		dispram[VKL144_segtab[0]/2+segi]=VKL144_dispram[segi];
	}
	WritenDataVKL144(0,dispram,18);
}
/**
  * @brief  lcd��ʾ�����㣨1comx1seg��
  * @param  Seg->���Ӧ��seg 
  * @param  Com->���Ӧ��com
  * @retval None
  */
void VKL144_DisSegOn(unsigned char Seg,unsigned char Com)
{
	if(Seg%2==0)
	{
		VKL144_dispram[(Seg-VKL144_segtab[0])/2]|=(1<<(Com+4));
		Write1DataVKL144(Seg,VKL144_dispram[(Seg-VKL144_segtab[0])/2]);
	}
	else
	{
		VKL144_dispram[(Seg-VKL144_segtab[0])/2]|=(1<<Com);		
		Write1DataVKL144(Seg-1,VKL144_dispram[(Seg-VKL144_segtab[0])/2]);
	}
}
/**
  * @brief  lcd�رյ����㣨1comx1seg��
  * @param  None 
  * @retval None
  */
void VKL144_DisSegOff(unsigned char Seg,unsigned char Com)
{
	if(Seg%2==0)
	{
		VKL144_dispram[(Seg-VKL144_segtab[0])/2]&=~(1<<(Com+4));
		Write1DataVKL144(Seg,VKL144_dispram[(Seg-VKL144_segtab[0])/2]);
	}
	else
	{
		VKL144_dispram[(Seg-VKL144_segtab[0])/2]&=~(1<<Com);		
		Write1DataVKL144(Seg-1,VKL144_dispram[(Seg-VKL144_segtab[0])/2]);
	}
}

/**
  * @brief  VKL144�������͹���ģʽ
  * @param  None 
  * @retval None
  */
void VKL144_Halt(void)
{		
	unsigned char i;
	
	i=1;
	while(i--)
	{
		VKL144_I2CStart();
		VKL144_I2CWriteByte(VKL144_ADDR); 
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		
		#if	BIAS_1_2
		VKL144_I2CWriteByte(VKL144_MODESET_1_2_OFF);	//�ر���ʾ 1/2bias
		#endif
		
		#if	BIAS_1_3
		VKL144_I2CWriteByte(VKL144_MODESET_1_3_OFF);	//�ر���ʾ 1/3bias
		#endif
		
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
	}
	VKL144_I2CStop();
}
/**
  * @brief  VKL144 Init	�˳��͹��ĵ��ô˺�˰
  * @param  None 
  * @retval None
  */
void VKL144_Init(void)
{	
	unsigned char i,n;
	unsigned char errorflag = 1;//�����־λ��1Ϊ����0Ϊ��ȷ
	
	i=1;
	while(i--)
	{
		VKL144_Delay_ms(VKL144_BASE100MS/10);
		VKL144_I2CStop();
		VKL144_Delay_ms(VKL144_BASE100MS/20);
		VKL144_I2CStart();
		VKL144_I2CWriteByte(VKL144_ADDR); 
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		
		VKL144_I2CWriteByte(VKL144_ICSET|BIT1_ENSRST); //��λ
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		VKL144_I2CWriteByte(VKL144_ICSET); 	//ӳ����ʼ��ַ bit5=0
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		VKL144_I2CWriteByte(VKL144_ADSET); //ӳ����ʼ��ַ bit4~bit0=0
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		errorflag=0;
		for(n=0;n<VKL144_RAMBUFNUM;n++)
		{ 
			VKL144_I2CWriteByte(0x00);      // ��ʼ������ʾ�ڴ���ʾӳ��,���޸�����. 
			if( SET == VKL144_I2CSlaveAck() )
			{
				continue;
			}
			else
			{
				errorflag=1;
				break;
			}
		}
		if(errorflag==1)
		{
			continue;
		}
		
		VKL144_I2CStop(); 
		VKL144_Delay_ms(VKL144_BASE100MS/20);  
		VKL144_I2CStart();
		
		VKL144_I2CWriteByte(VKL144_ADDR);
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
		
		#if	BIAS_1_2
		VKL144_I2CWriteByte(VKL144_MODESET_1_2_ON);	//����ʾ 1/2bias
		#endif
		
		#if	BIAS_1_3
		VKL144_I2CWriteByte(VKL144_MODESET_1_3_ON);	//����ʾ 1/3bias
		#endif
		
		if( RESET == VKL144_I2CSlaveAck() )
		{
			continue;
		}
	}
	VKL144_I2CStop();
	VKL144_FunSet(VKL144_DISCTL_DATA);	//������������
	
	VKL144_num_init();					//�Զ�����Һ������Ҫ������
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
unsigned char xdata LCD_RAM_Tab[sizeof(LED_Tab)];

#define		LCD_A			0x01
#define		LCD_B			0x02
#define		LCD_C			0x04
#define		LCD_D			0x08
#define		LCD_E			0x10
#define		LCD_F			0x20
#define		LCD_G			0x40
#define		LCD_P			0x00

void VKL144_num_init(void)
{
	unsigned char num,LCD_data;
	
	for(num=0; num<sizeof(LED_Tab); num++)
	{
		LCD_data = 0;
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_D)) << 7);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_E)) << 6);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_F)) << 5);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_A)) << 4);
		
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_P)) << 3);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_C)) << 2);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_G)) << 1);
		LCD_data = LCD_data | ((unsigned char)((bit)(LED_Tab[num] & LCD_B)) << 0);
		
		LCD_RAM_Tab[num] = LCD_data;
	}
}

void DispJUHUI(void)
{
	Write1DataVKL144(VKL144_segtab[2], LCD_RAM_Tab[19]);
	Write1DataVKL144(VKL144_segtab[4], LCD_RAM_Tab[20]);
	
	Write1DataVKL144(VKL144_segtab[8], LCD_RAM_Tab[21]);
	Write1DataVKL144(VKL144_segtab[10],LCD_RAM_Tab[20]);
	Write1DataVKL144(VKL144_segtab[12],LCD_RAM_Tab[22]);
}

void VKL144_Test(void)
{	
	VKL144_DisAll(1);
	VKL144_Delay_ms(1000);
	
	VKL144_DisAll(0);
	VKL144_Delay_ms(1000);
	
	for(VKL144_segi=0;VKL144_segi<VKL144_SEGNUM;VKL144_segi++)		//seg
	{
		for(VKL144_comi=0;VKL144_comi<4;VKL144_comi++)				//com
		{
			VKL144_DisSegOn(VKL144_segtab[VKL144_segi],VKL144_comi);//LCD����seg����
			VKL144_Delay_ms(100);
			VKL144_DisAll(0);
		}
	}
	
	VKL144_DisAll(1);
	VKL144_Delay_ms(1000);
	
	for(VKL144_segi=0;VKL144_segi<VKL144_SEGNUM;VKL144_segi++)		//seg
	{
		for(VKL144_comi=0;VKL144_comi<4;VKL144_comi++)				//com
		{
			VKL144_DisSegOff(VKL144_segtab[VKL144_segi],VKL144_comi);//LCD����seg�ر�
			VKL144_Delay_ms(100);
			VKL144_DisAll(1);
		}
	}
}
/**
  * @brief  �û���ʾ������Ҫ�Լ�����
  * @param  None
  * @param  None
  * @retval None
  */
void VKL144_Display(void)
{
	unsigned char Temp_Data,LCD_Data;
	
	//---------------------------------------------------------------
	
	Temp_Data = 12;
	
	LCD_Data = Temp_Data / 10;
	Write1DataVKL144(VKL144_segtab[0],	LCD_RAM_Tab[LCD_Data]);
	LCD_Data = Temp_Data % 10;
	Write1DataVKL144(VKL144_segtab[2],	LCD_RAM_Tab[LCD_Data]);
	
	
	//---------------------------------------------------------------
	
	Temp_Data = 34;
	
	LCD_Data = Temp_Data / 10;
	Write1DataVKL144(VKL144_segtab[4],	LCD_RAM_Tab[LCD_Data]);
	LCD_Data = Temp_Data % 10;
	Write1DataVKL144(VKL144_segtab[6],	LCD_RAM_Tab[LCD_Data]);
	
	//---------------------------------------------------------------
	
	Temp_Data = 56;
	
	LCD_Data = Temp_Data / 10;
	Write1DataVKL144(VKL144_segtab[8],	LCD_RAM_Tab[LCD_Data]);
	LCD_Data = Temp_Data % 10;
	Write1DataVKL144(VKL144_segtab[10],	LCD_RAM_Tab[LCD_Data]);
	
	//---------------------------------------------------------------
	
	Temp_Data = 78;
	
	LCD_Data = Temp_Data / 10;
	Write1DataVKL144(VKL144_segtab[12],	LCD_RAM_Tab[LCD_Data]);
	LCD_Data = Temp_Data % 10;
	Write1DataVKL144(VKL144_segtab[14],	LCD_RAM_Tab[LCD_Data]);
}
/************************END OF FILE****/








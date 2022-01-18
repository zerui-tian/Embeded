#include "ht1621.h"

#define HT1621_BASE100MS	100		//由HT1621_Delay实现100mS延时，根据单片机修改此常数
#define HT1621_CLK			10		//WR信号线频率,由 HT1621_Delay_us 实现，建议100K	1/0.1M=10us

const unsigned char HT1621_segtab[HT1621_SEGNUM]={
	0,1,2,3,4,5,6,7
};

unsigned char HT1621_maxcom=4;					//驱动的com数可以是4com，3com，2com
unsigned char HT1621_addrbit=6;					//驱动ic的ram地址位数 A5-A0
unsigned char HT1621_dispram[HT1621_SEGNUM];	//显示RAM buffer只使用低4位，bit0->com0,bit1->com1,bit2->com2,bit3->com3
unsigned char HT1621_segi,HT1621_comi;

void HT1621_CS_L(int piece){
    switch(piece){
        case 0:
            HT1621_CS2_H();
            HT1621_CS3_H();
            HT1621_CS1_L();
            break;
        case 1:
            HT1621_CS1_H();
            HT1621_CS3_H();
            HT1621_CS2_L();
            break;
        case 2:
            HT1621_CS1_H();
            HT1621_CS2_H();
            HT1621_CS3_L();
            break;
        default:
            break;
    }
    return;
}

void HT1621_CS_H(){
    HT1621_CS1_H();
    HT1621_CS2_H();
    HT1621_CS3_H();
    return;
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
  * @param  FunctonCode->功能/命令码 
  * @retval None
  */
void WriteCommandHT1621(unsigned char FunctonCode, int piece)
{
	unsigned char Shift = 0x80;
	unsigned char i;
	
	HT1621_CS_L(piece);					//CS 片选开
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
	WriteClockHT1621();				//发送一个 0 命令码中最后一位 X
	
	HT1621_CS_H(); 					//CS 片选关
	HT1621_Delay_us(HT1621_CLK);
	HT1621_DATA_H();
}
/**
  * @brief  Write 1 data to HT1621
  * @param  Addr->写入ram的地址 
  * @param  Databuf->写入ram的数据
  * @retval None
  */
void Write1DataHT1621(unsigned char Addr,unsigned char Dat, int piece){
	unsigned char i; 
	unsigned char Shift;
	
	HT1621_CS_L(piece);  				//CS 片选开
	HT1621_Delay_us(HT1621_CLK);
	
	HT1621_DATA_H();  				//101
	WriteClockHT1621();
	HT1621_DATA_L();
	WriteClockHT1621();
	HT1621_DATA_H();
	WriteClockHT1621();
	
	Shift = 0x20; 					//屏蔽高3位 只用5位
	for(i = 0; i < HT1621_addrbit; i++){
		if(Addr & Shift)
			HT1621_DATA_H();
		else
			HT1621_DATA_L();
		WriteClockHT1621();
		Shift = Shift >> 1;
	}
	
	Shift = 0x01;
	for (i = 0; i < 4; i++){
		if( Dat & Shift) 
			HT1621_DATA_H();
		else
			HT1621_DATA_L();
		WriteClockHT1621();
		Shift = Shift << 1;
	}   
	HT1621_CS_H(); 					//CS 片选关
	HT1621_Delay_us(HT1621_CLK);
	HT1621_DATA_H();
}
/**
  * @brief  Write n data to HT1621 
  * @param  Addr->写入ram的起始地址 
  * @param  Databuf->写入ram的数据buffer
  * @param  Cnt->写入ram的数据个数
  * @retval None
  */
void WritenDataHT1621(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt,int piece){
	unsigned char i,j;
	unsigned char Shift;
	unsigned char dataval;
	
	HT1621_CS_L(piece);   				//CS 片选开
	HT1621_Delay_us(HT1621_CLK);
	
	HT1621_DATA_H();  		  		//101
	WriteClockHT1621();
	HT1621_DATA_L();  
	WriteClockHT1621();
	HT1621_DATA_H();  
	WriteClockHT1621();
	
	Shift = 0x20; 					//屏蔽高3位 只用5位
	for( i = 0; i < HT1621_addrbit; i++){
		if (Addr & Shift)
			HT1621_DATA_H();
		else
			HT1621_DATA_L();
		WriteClockHT1621();
		Shift = Shift >> 1;
	}
	
	for (j = 0; j < Cnt; j++){
		Shift = 0x01;
		dataval=*Databuf++;
		for (i = 0; i < 4; i++){
			if( dataval & Shift) 
				HT1621_DATA_H();
			else
				HT1621_DATA_L();
			WriteClockHT1621();
			Shift = Shift << 1;
		}
	}
	HT1621_CS_H();   				//CS 片选关
	HT1621_Delay_us(HT1621_CLK);
	HT1621_DATA_H();
}
/**
  * @brief  lcd全显或全灭
  * @param  state=1->lcd全亮
  * @param  state=0->lcd全灭
  * @retval None
  */
void HT1621_DisAll(unsigned char state){
	unsigned char segi;
	unsigned char dispram[32];

	for(segi=0;segi<32;segi++){
		dispram[segi]=0;
	}
	for(segi=0;segi<HT1621_SEGNUM;segi++){
		if(state==1){
			if(HT1621_maxcom==2)
				HT1621_dispram[segi]=0x03;
			else if(HT1621_maxcom==3)
				HT1621_dispram[segi]=0x07;
			else if(HT1621_maxcom==4)
				HT1621_dispram[segi]=0x0f;
		}
		else{
			HT1621_dispram[segi]=0x00;
		}
		dispram[HT1621_segtab[segi]] = HT1621_dispram[segi];
	}
    WritenDataHT1621(0,dispram,32,0);
    WritenDataHT1621(0,dispram,32,1);
	WritenDataHT1621(0,dispram,32,2);
}
/**
  * @brief  lcd显示单个点（1comx1seg）
  * @param  Seg->点对应的seg 
  * @param  Com->点对应的com
  * @retval None
  */
void HT1621_DisSegOn(unsigned char Seg,unsigned char Com,int piece){
	HT1621_dispram[Seg-HT1621_segtab[0]] |= (1<<Com);
	Write1DataHT1621(Seg,HT1621_dispram[Seg-HT1621_segtab[0]],piece);
}
/**
  * @brief  lcd关闭单个点（1comx1seg）
  * @param  None 
  * @retval None
  */
void HT1621_DisSegOff(unsigned char Seg,unsigned char Com,int piece){
	HT1621_dispram[Seg-HT1621_segtab[0]] &= ~(1<<Com);
	Write1DataHT1621(Seg,HT1621_dispram[Seg-HT1621_segtab[0]],piece);
}
/**
  * @brief  HT1621进入掉电低功耗模式
  * @param  None 
  * @retval None
  */
void HT1621_Halt(int piece){
	//连续使用命令LCDOFF和SYSDIS,系统将处于掉电低功耗状态。
	//只有使用片内RC时钟源时,才能使系统进入掉电低功耗状态。
	WriteCommandHT1621(OSC_OFF,piece);
	WriteCommandHT1621(DISP_OFF,piece);
}
/**
  * @brief  HT1621 Init
  * @param  None
  * @retval None
  */
void HT1621_Init()	//退出掉电低功耗状态重新初始化HT1621
{		
	HT1621_CS_H();
	HT1621_WR_H();
	HT1621_DATA_H();
	
	WriteCommandHT1621(OSC_ON,0);
	WriteCommandHT1621(OSC_ON,1);
	WriteCommandHT1621(OSC_ON,2);
	WriteCommandHT1621(DISP_ON,0);
    WriteCommandHT1621(DISP_ON,1);
    WriteCommandHT1621(DISP_ON,2);
	
	#if	COM_4_BIAS_1_3
	WriteCommandHT1621(COM_1_3__4,0);HT1621_maxcom=4;	//1/3bias 4com
    WriteCommandHT1621(COM_1_3__4,1);HT1621_maxcom=4;   //1/3bias 4com
    WriteCommandHT1621(COM_1_3__4,2);HT1621_maxcom=4;   //1/3bias 4com
	#endif
	
	#if	COM_3_BIAS_1_3
	WriteCommandHT1621(COM_1_3__3,0);HT1621_maxcom=3;	//1/3bias 3com
    WriteCommandHT1621(COM_1_3__3,1);HT1621_maxcom=3;   //1/3bias 3com
    WriteCommandHT1621(COM_1_3__3,2);HT1621_maxcom=3;   //1/3bias 3com
	#endif
	
	#if	COM_2_BIAS_1_3
	WriteCommandHT1621(COM_1_3__2,0);HT1621_maxcom=2;	//1/3bias 2com
    WriteCommandHT1621(COM_1_3__2,1);HT1621_maxcom=2;   //1/3bias 2com
    WriteCommandHT1621(COM_1_3__2,2);HT1621_maxcom=2;   //1/3bias 2com
	#endif
	
	#if	COM_4_BIAS_1_2
	WriteCommandHT1621(COM_1_2__4,0);HT1621_maxcom=4;	//1/2bias 4com
    WriteCommandHT1621(COM_1_2__4,1);HT1621_maxcom=4;   //1/2bias 4com
    WriteCommandHT1621(COM_1_2__4,2);HT1621_maxcom=4;   //1/2bias 4com
	#endif
	
	#if	COM_3_BIAS_1_2
	WriteCommandHT1621(COM_1_2__3,0);HT1621_maxcom=3;	//1/2bias 3com
    WriteCommandHT1621(COM_1_2__3,1);HT1621_maxcom=3;   //1/2bias 3com
    WriteCommandHT1621(COM_1_2__3,2);HT1621_maxcom=3;   //1/2bias 3com
	#endif
	
	#if	COM_2_BIAS_1_2
	WriteCommandHT1621(COM_1_2__2,0);HT1621_maxcom=2;	//1/2bias 2com
    WriteCommandHT1621(COM_1_2__2,1);HT1621_maxcom=2;   //1/2bias 2com
    WriteCommandHT1621(COM_1_2__2,2);HT1621_maxcom=2;   //1/2bias 2com
	#endif
	
	//上电默认配置(以下未用功能关闭降低功耗)
	
	WriteCommandHT1621(TIMER_DIS,0);
    WriteCommandHT1621(TIMER_DIS,1);
    WriteCommandHT1621(TIMER_DIS,2);
	WriteCommandHT1621(WDT_DIS,0);
    WriteCommandHT1621(WDT_DIS,1);
    WriteCommandHT1621(WDT_DIS,2);
	WriteCommandHT1621(BUZZ_OFF,0);
    WriteCommandHT1621(BUZZ_OFF,1);
    WriteCommandHT1621(BUZZ_OFF,2);
	WriteCommandHT1621(IRQ_DIS,0);
    WriteCommandHT1621(IRQ_DIS,1);
    WriteCommandHT1621(IRQ_DIS,2);
	
	HT1621_num_init();		//自动生成液晶屏需要的数组
}
/**
  * @brief  数据交换，自动生成液晶需要的数组，此程序，需要根据液晶图纸修改
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
	0x38    //23=L
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

void HT1621_num_init(void){
	unsigned char num,LCD_data;
	
	for(num=0; num<sizeof(LED_Tab); num++){
		LCD_data = 0;
		LCD_data = LCD_data | ((unsigned char)((LED_Tab[num] & LCD_P)?0x01:0x00) << 7);
		LCD_data = LCD_data | ((unsigned char)((LED_Tab[num] & LCD_E)?0x01:0x00) << 6);
		LCD_data = LCD_data | ((unsigned char)((LED_Tab[num] & LCD_F)?0x01:0x00) << 5);
		LCD_data = LCD_data | ((unsigned char)((LED_Tab[num] & LCD_A)?0x01:0x00) << 4);
		
		LCD_data = LCD_data | ((unsigned char)((LED_Tab[num] & LCD_D)?0x01:0x00) << 3);
		LCD_data = LCD_data | ((unsigned char)((LED_Tab[num] & LCD_C)?0x01:0x00) << 2);
		LCD_data = LCD_data | ((unsigned char)((LED_Tab[num] & LCD_G)?0x01:0x00) << 1);
		LCD_data = LCD_data | ((unsigned char)((LED_Tab[num] & LCD_B)?0x01:0x00) << 0);
		
		LCD_RAM_Tab[num] = LCD_data;
	}
}

//void HT1621_Test(void)
//{
//	HT1621_DisAll(1);
//	HT1621_Delay_ms(1000);
//	HT1621_DisAll(0);
//	HT1621_Delay_ms(1000);
//
//	for(HT1621_segi=0;HT1621_segi<HT1621_SEGNUM;HT1621_segi++)		//seg
//	{
//		for(HT1621_comi=0;HT1621_comi<HT1621_maxcom;HT1621_comi++)	//com
//		{
//			HT1621_DisSegOn(HT1621_segtab[HT1621_segi],HT1621_comi);//LCD单个seg点亮
//			HT1621_Delay_ms(100);
//			HT1621_DisAll(0);
//		}
//	}
//
//	HT1621_DisAll(1);
//	HT1621_Delay_ms(1000);
//	for(HT1621_segi=0;HT1621_segi<HT1621_SEGNUM;HT1621_segi++)		//seg
//	{
//		for(HT1621_comi=0;HT1621_comi<HT1621_maxcom;HT1621_comi++)	//com
//		{
//			HT1621_DisSegOff(HT1621_segtab[HT1621_segi],HT1621_comi);//LCD单个seg关闭
//			HT1621_Delay_ms(100);
//			HT1621_DisAll(1);
//		}
//	}
//	HT1621_Delay_ms(1000);
//	HT1621_DisAll(0);
//}
/**
  * @brief  用户显示程序，需要自己完善
  * @param  None
  * @param  None
  * @retval None
  */
void HT1621_Display(unsigned char num1,
                    unsigned char num2,
                    unsigned char num3,
                    unsigned char num4,
                    unsigned char colon,
                    unsigned char dot,
                    int piece){
    if(num1 < 10){
        Write1DataHT1621(HT1621_segtab[0],	((LCD_RAM_Tab[num1]>>4)&0x0F)|(colon?0x08:0x00),piece);
	    Write1DataHT1621(HT1621_segtab[1],	(LCD_RAM_Tab[num1]>>0)&0x0F,piece);
    }
    else{
        Write1DataHT1621(HT1621_segtab[0],  (colon?0x08:0x00),piece);
        Write1DataHT1621(HT1621_segtab[1],  0,piece);
    }
    if(num2 < 10){
        Write1DataHT1621(HT1621_segtab[2],	(LCD_RAM_Tab[num2]>>4)&0x0F,piece);
        Write1DataHT1621(HT1621_segtab[3],	(LCD_RAM_Tab[num2]>>0)&0x0F,piece);
    }
    else{
        Write1DataHT1621(HT1621_segtab[2],  0,piece);
        Write1DataHT1621(HT1621_segtab[3],  0,piece);
    }
    if(num3 < 10){
        Write1DataHT1621(HT1621_segtab[4],	((LCD_RAM_Tab[num3]>>4)&0x0F)|(dot?0x08:0x00),piece);
        Write1DataHT1621(HT1621_segtab[5],	(LCD_RAM_Tab[num3]>>0)&0x0F,piece);
    }
    else{
        Write1DataHT1621(HT1621_segtab[4],  (dot?0x08:0x00),piece);
        Write1DataHT1621(HT1621_segtab[5],  0,piece);
    }
    if(num4 < 10){
        Write1DataHT1621(HT1621_segtab[6],	(LCD_RAM_Tab[num4]>>4)&0x0F,piece);
        Write1DataHT1621(HT1621_segtab[7],	(LCD_RAM_Tab[num4]>>0)&0x0F,piece);
    }
    else{

        Write1DataHT1621(HT1621_segtab[6],  0,piece);
        Write1DataHT1621(HT1621_segtab[7],  0,piece);
    }
}

void HT1621_UpdateStatusDisplay(
    unsigned char frameReceivedCounter,
    char phaseSynchronized,
    char timeSynchronized,
    unsigned int listenCounterDown,
    int piece){
    if(listenCounterDown>0){
        if(phaseSynchronized){
            Write1DataHT1621(HT1621_segtab[0],  (LCD_RAM_Tab[18]>>4)&0x0F,piece);
            Write1DataHT1621(HT1621_segtab[1],  (LCD_RAM_Tab[18]>>0)&0x0F,piece);
            if(timeSynchronized){
                Write1DataHT1621(HT1621_segtab[2],  (LCD_RAM_Tab[18]>>4)&0x0F,piece);
                Write1DataHT1621(HT1621_segtab[3],  (LCD_RAM_Tab[18]>>0)&0x0F,piece);
                Write1DataHT1621(HT1621_segtab[4],  (LCD_RAM_Tab[18]>>4)&0x0F,piece);
                Write1DataHT1621(HT1621_segtab[5],  (LCD_RAM_Tab[18]>>0)&0x0F,piece);
                Write1DataHT1621(HT1621_segtab[6],  (LCD_RAM_Tab[18]>>4)&0x0F,piece);
                Write1DataHT1621(HT1621_segtab[7],  (LCD_RAM_Tab[18]>>0)&0x0F,piece);
            }
            else{
                if(frameReceivedCounter==2){
                    Write1DataHT1621(HT1621_segtab[2],  (LCD_RAM_Tab[18]>>4)&0x0F,piece);
                    Write1DataHT1621(HT1621_segtab[3],  (LCD_RAM_Tab[18]>>0)&0x0F,piece);
                    Write1DataHT1621(HT1621_segtab[4],  0,piece);
                    Write1DataHT1621(HT1621_segtab[5],  0,piece);
                    Write1DataHT1621(HT1621_segtab[6],  0,piece);
                    Write1DataHT1621(HT1621_segtab[7],  0,piece);
                }
                else if(frameReceivedCounter==3){
                    Write1DataHT1621(HT1621_segtab[2],  (LCD_RAM_Tab[18]>>4)&0x0F,piece);
                    Write1DataHT1621(HT1621_segtab[3],  (LCD_RAM_Tab[18]>>0)&0x0F,piece);
                    Write1DataHT1621(HT1621_segtab[4],  (LCD_RAM_Tab[18]>>4)&0x0F,piece);
                    Write1DataHT1621(HT1621_segtab[5],  (LCD_RAM_Tab[18]>>0)&0x0F,piece);
                    Write1DataHT1621(HT1621_segtab[6],  0,piece);
                    Write1DataHT1621(HT1621_segtab[7],  0,piece);
                }
                else{
                    Write1DataHT1621(HT1621_segtab[2],  0,piece);
                    Write1DataHT1621(HT1621_segtab[3],  0,piece);
                    Write1DataHT1621(HT1621_segtab[4],  0,piece);
                    Write1DataHT1621(HT1621_segtab[5],  0,piece);
                    Write1DataHT1621(HT1621_segtab[6],  0,piece);
                    Write1DataHT1621(HT1621_segtab[7],  0,piece);
                }
            }
        }
        else{
            Write1DataHT1621(HT1621_segtab[0],  0,piece);
            Write1DataHT1621(HT1621_segtab[1],  0,piece);
            Write1DataHT1621(HT1621_segtab[2],  0,piece);
            Write1DataHT1621(HT1621_segtab[3],  0,piece);
            Write1DataHT1621(HT1621_segtab[4],  0,piece);
            Write1DataHT1621(HT1621_segtab[5],  0,piece);
            Write1DataHT1621(HT1621_segtab[6],  0,piece);
            Write1DataHT1621(HT1621_segtab[7],  0,piece);
        }
    }
    else{
        Write1DataHT1621(HT1621_segtab[0],  (LCD_RAM_Tab[15]>>4),piece);
        Write1DataHT1621(HT1621_segtab[1],  (LCD_RAM_Tab[15]>>0),piece);
        Write1DataHT1621(HT1621_segtab[2],  (LCD_RAM_Tab[10]>>4),piece);
        Write1DataHT1621(HT1621_segtab[3],  (LCD_RAM_Tab[10]>>0),piece);
        Write1DataHT1621(HT1621_segtab[4],  (LCD_RAM_Tab[22]>>4),piece);
        Write1DataHT1621(HT1621_segtab[5],  (LCD_RAM_Tab[22]>>0),piece);
        Write1DataHT1621(HT1621_segtab[6],  (LCD_RAM_Tab[23]>>4),piece);
        Write1DataHT1621(HT1621_segtab[7],  (LCD_RAM_Tab[23]>>0),piece);
    }
}
/************************END OF FILE****/

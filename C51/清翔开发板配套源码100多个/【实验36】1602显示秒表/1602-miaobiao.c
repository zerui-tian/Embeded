/*********************************************************************************
* ����дʱ�䡿�� 2014��3��5��
* ����    �ߡ��� �������:03
* ����    ������ 1.0
* ����    վ���� http://www.qxmcu.com/ 
* ���Ա����̡��� http://qxmcu.taobao.com/ 
* ��ʵ��ƽ̨���� QX-MCS51 ��Ƭ��������
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52
* �����뻷������ Keil ��Visio3	
* �������ܡ��� 			   			            			    
* ��ʹ��˵������ Һ��1�Ž�1602��ɫ��ĸ��1��  ������ޱ����Һ��ֻ��14���� ��ĸ�ұ߿�2��λ   
                 ���Һ���Աȶȵ�λ��˳ʱ�����10���ӷ�����������������λ�û��𻵵�λ����
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/

/*******************************************************************


*                                                                 	*
* ������                                                          	*
*                                                                 	*
*   �ϵ��Һ��������ʾ��Ϣ�����Ű���S2����ʱ��ʼ���ٴΰ���      	*
*                                                                 	*
*   S2��ͣ����3�ΰ�����ʾ�ۻ���ʱ����4�ΰ�����ͣ��ʱ���κ�ʱ����S3*
*                                                                 	*
*   �������㡣                                             		    *
*									                                *
*                                                                 	*
************************************************************************/

#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

uchar KeyCount=0;

sbit  S2 = P3^0;
sbit  S3 = P3^1;

sbit BEEP = P2^3;          //������

uchar code  cdis1[ ] = {"  STOPWATCH  0  "};
uchar code  cdis2[ ] = {"    QX-MCS51    "};
uchar code  cdis3[ ] = {"TIME            "};

uchar code  cdis4[ ] = {" BEGIN COUNT  1 "};
uchar code  cdis5[ ] = {" PAUSE COUNT  2 "};
uchar code  cdis6[ ] = {" BEGIN COUNT  3 "};
uchar code  cdis7[ ] = {" PAUSE COUNT  4 "};
uchar code  cdis8[ ] = {"                "};

sbit LCD_RS = P3^5;             
sbit LCD_RW = P3^6;
sbit LCD_EN = P3^4;
sbit dula=P2^6;
sbit wela=P2^7;

#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

uchar display[] =  {0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uchar display2[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*********************************************************
 ��ʱ����
*********************************************************/

void Delay(uint num)//��ʱ����
{
  while( --num );
}

/*********************************************************
 ��������һ��                                           
**********************************************************/
void beep()
{
    unsigned char y;
    for (y=0;y<180;y++)
    {
      BEEP=!BEEP;                //BEEPȡ��
	  Delay(70);
    } 
    BEEP=1;                      //�رշ�����
}

/*********************************************************
 ��ʱ����1
*********************************************************/
void delay1(int ms)
{
   unsigned char n;
   while(ms--)
   {
     for(n = 0; n<250; n++)
    {
      _nop_();
      _nop_();
      _nop_();
      _nop_();
    }
   }
}

/**********************************************************
*                                                         *
*дָ�����ݵ�LCD                                          *
*RS=L��RW=L��E=�����壬D0-D7=ָ���롣                     *
*                                                         *
**********************************************************/
void lcd_wcmd(uchar cmd)
{                          
//   while(lcd_busy());
    LCD_RS = 0;
//    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    P0 = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
    Delay(10);
}

/**********************************************************
*                                                         *
*д��ʾ���ݵ�LCD                                          *
*RS=H��RW=L��E=�����壬D0-D7=���ݡ�                       *
*                                                         *
**********************************************************/
void lcd_wdat(uchar dat)
{                          
//   while(lcd_busy());
    LCD_RS = 1;
//    LCD_RW = 0;
    LCD_EN = 0;
    P0 = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
    Delay(10);
}

/**********************************************************
*                                                         *
*  LCD��ʼ���趨                                          *
*                                                         *
**********************************************************/
void lcd_init()
{    
   
    LCD_RW = 0;
	dula=0;
     wela=0;
    delay1(15);   
    lcd_wcmd(0x01);      //���LCD����ʾ����            
    lcd_wcmd(0x38);      //16*2��ʾ��5*7����8λ����
    delay1(5);
    lcd_wcmd(0x38);         
    delay1(5);
    lcd_wcmd(0x38);         
    delay1(5);

    lcd_wcmd(0x0c);      //����ʾ������ʾ���  
    delay1(5);

    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay1(5);
}

/**********************************************************
*                                                         *
*  �趨��ʾλ��                                           *
*                                                         *
**********************************************************/

void lcd_pos(uchar pos)
{                          
  lcd_wcmd(pos | 0x80);  //����ָ��=80+��ַ����
}

/********************************************************
 ��ʾ����
*********************************************************/
void play()
{
   uchar  i;
   for(i=0;i<6;i++)
   {
    display2[i]=display[i]+0x30;  
   }

   display2[7]=display[6]/10+0x30;   //ʱ��λ���ݴ���
   display2[6]=display[6]%10+0x30;
   
   lcd_pos(0x45);
   lcd_wdat(display2[7]);   //��ʾʱ
   lcd_wdat(display2[6]);
   lcd_wdat(0x3a);          //��ʾ':'
  
   lcd_wdat(display2[5]);   //��ʾ��
   lcd_wdat(display2[4]);
   lcd_wdat(0x3a);          //��ʾ':'

   lcd_wdat(display2[3]);   //��ʾ��
   lcd_wdat(display2[2]);
   lcd_wdat(0x3a);          //��ʾ':'

   lcd_wdat(display2[1]);   //��ʾ����
   lcd_wdat(display2[0]);  
}

/********************************************************
 ������
*********************************************************/
main()
{
   uchar m;
  
   TMOD=0x01; 
   TH0=0xdc; TL0=0x00;      //50ms��ʱ   
   EA=1; ET0=1;

   lcd_init();
   
   lcd_pos(0x00);             //������ʾλ��Ϊ��һ��
   for(m=0;m<16;m++) 
   lcd_wdat(cdis1[m]);        //��ʾ�ַ� 

   lcd_pos(0x40);             //������ʾλ��Ϊ�ڶ���
   for(m=0;m<16;m++)
   {
    lcd_wdat(cdis2[m]);        //��ʾ�ַ�
    delay1(60);
   }

   delay1(100);

   lcd_pos(0x40);             //������ʾλ��Ϊ�ڶ���
   for(m=0;m<16;m++)
   lcd_wdat(cdis3[m]); 

   while(1)
   {   
     if(S2==0)
     {
	   KeyCount++;          //����
       beep();
	   delay1(100);    
     
       switch (KeyCount)
       {
        case 1: 
        TR0=1;              //�����ж�
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis4[m]);     
        break;
  
        case 2: 
        TR0=0;              //ֹͣ�ж�
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis5[m]);     
        break;
      
		case 3: 
        TR0=1;              //�����ж�
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis6[m]);     
        break;

        case 4: 
        TR0=0;              //ֹͣ�ж�
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis7[m]);     
        break;
     
        default:  
        TR0=0;                   //ֹͣ�ж� 
        break;
       }
     }
     if(S3==0)
     {
	   TR0=0;                     //ֹͣ�ж�
       KeyCount=0;
	   for(m=0;m<8;m++)
       display[m]=0x00;           //��ʱ��Ԫ����
       lcd_pos(0x00);             //������ʾλ��Ϊ��һ��
       for(m=0;m<16;m++)
       lcd_wdat(cdis1[m]); 
       beep();
       delay1(100);              
     } 

     play(); 
   }
}

/*********************************************************
*                                                        *
* Time0�жϺ���                                          *
*                                                        *
**********************************************************/
void Time0(void) interrupt 1 using 0
{
   TH0=0xdc;               //10ms��ʱ
   TL0=0x00;

   display[0]++;         //0.01S  

   if(display[0]==10)
   {
    display[0]=0;
	display[1]++;         //0.1S
   }
   if(display[1]==10)
   {
    display[1]=0;
    display[2]++;         //���λ
   }
   if(display[2]==10)
   {
    display[2]=0;
    display[3]++;         //��ʮλ
   }
   if(display[3]==6)
   {
    display[3]=0;
    display[4]++;         //�ָ�λ
   }
   if(display[4]==10)
   {
    display[4]=0;
    display[5]++;         //��ʮλ
   }
   if(display[5]==6)
   {
    display[5]=0;
    display[6]++;        //ʱ
   }
   if(display[6]==24)
   {
     display[6]=0;
   } 
}

/*********************************************************/

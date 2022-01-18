/*********************************************************************************
* 【编写时间】： 2014年3月5日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】： http://www.qxmcu.com/ 
* 【淘宝店铺】： http://qxmcu.taobao.com/ 
* 【实验平台】： QX-MCS51 单片机开发板
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52
* 【编译环境】： Keil μVisio3	
* 【程序功能】： 			   			            			    
* 【使用说明】： 液晶1脚接1602黑色排母的1脚  如果是无背光的液晶只有14个脚 排母右边空2个位   
                 请把液晶对比度电位器顺时针调制10点钟方向，请勿多调超出极限位置会损坏电位器！
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

/*******************************************************************


*                                                                 	*
* 描述：                                                          	*
*                                                                 	*
*   上电后液晶屏先显示信息，接着按下S2，定时开始，再次按下      	*
*                                                                 	*
*   S2暂停，第3次按下显示累积计时，第4次按下暂停计时，任何时候按下S3*
*                                                                 	*
*   计数清零。                                             		    *
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

sbit BEEP = P2^3;          //蜂鸣器

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
 延时函数
*********************************************************/

void Delay(uint num)//延时函数
{
  while( --num );
}

/*********************************************************
 蜂鸣器响一声                                           
**********************************************************/
void beep()
{
    unsigned char y;
    for (y=0;y<180;y++)
    {
      BEEP=!BEEP;                //BEEP取反
	  Delay(70);
    } 
    BEEP=1;                      //关闭蜂鸣器
}

/*********************************************************
 延时函数1
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
*写指令数据到LCD                                          *
*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                     *
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
*写显示数据到LCD                                          *
*RS=H，RW=L，E=高脉冲，D0-D7=数据。                       *
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
*  LCD初始化设定                                          *
*                                                         *
**********************************************************/
void lcd_init()
{    
   
    LCD_RW = 0;
	dula=0;
     wela=0;
    delay1(15);   
    lcd_wcmd(0x01);      //清除LCD的显示内容            
    lcd_wcmd(0x38);      //16*2显示，5*7点阵，8位数据
    delay1(5);
    lcd_wcmd(0x38);         
    delay1(5);
    lcd_wcmd(0x38);         
    delay1(5);

    lcd_wcmd(0x0c);      //开显示，不显示光标  
    delay1(5);

    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay1(5);
}

/**********************************************************
*                                                         *
*  设定显示位置                                           *
*                                                         *
**********************************************************/

void lcd_pos(uchar pos)
{                          
  lcd_wcmd(pos | 0x80);  //数据指针=80+地址变量
}

/********************************************************
 显示函数
*********************************************************/
void play()
{
   uchar  i;
   for(i=0;i<6;i++)
   {
    display2[i]=display[i]+0x30;  
   }

   display2[7]=display[6]/10+0x30;   //时单位数据处理
   display2[6]=display[6]%10+0x30;
   
   lcd_pos(0x45);
   lcd_wdat(display2[7]);   //显示时
   lcd_wdat(display2[6]);
   lcd_wdat(0x3a);          //显示':'
  
   lcd_wdat(display2[5]);   //显示分
   lcd_wdat(display2[4]);
   lcd_wdat(0x3a);          //显示':'

   lcd_wdat(display2[3]);   //显示秒
   lcd_wdat(display2[2]);
   lcd_wdat(0x3a);          //显示':'

   lcd_wdat(display2[1]);   //显示毫秒
   lcd_wdat(display2[0]);  
}

/********************************************************
 主函数
*********************************************************/
main()
{
   uchar m;
  
   TMOD=0x01; 
   TH0=0xdc; TL0=0x00;      //50ms定时   
   EA=1; ET0=1;

   lcd_init();
   
   lcd_pos(0x00);             //设置显示位置为第一行
   for(m=0;m<16;m++) 
   lcd_wdat(cdis1[m]);        //显示字符 

   lcd_pos(0x40);             //设置显示位置为第二行
   for(m=0;m<16;m++)
   {
    lcd_wdat(cdis2[m]);        //显示字符
    delay1(60);
   }

   delay1(100);

   lcd_pos(0x40);             //设置显示位置为第二行
   for(m=0;m<16;m++)
   lcd_wdat(cdis3[m]); 

   while(1)
   {   
     if(S2==0)
     {
	   KeyCount++;          //计数
       beep();
	   delay1(100);    
     
       switch (KeyCount)
       {
        case 1: 
        TR0=1;              //启动中断
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis4[m]);     
        break;
  
        case 2: 
        TR0=0;              //停止中断
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis5[m]);     
        break;
      
		case 3: 
        TR0=1;              //启动中断
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis6[m]);     
        break;

        case 4: 
        TR0=0;              //停止中断
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis7[m]);     
        break;
     
        default:  
        TR0=0;                   //停止中断 
        break;
       }
     }
     if(S3==0)
     {
	   TR0=0;                     //停止中断
       KeyCount=0;
	   for(m=0;m<8;m++)
       display[m]=0x00;           //计时单元清零
       lcd_pos(0x00);             //设置显示位置为第一行
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
* Time0中断函数                                          *
*                                                        *
**********************************************************/
void Time0(void) interrupt 1 using 0
{
   TH0=0xdc;               //10ms定时
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
    display[2]++;         //秒个位
   }
   if(display[2]==10)
   {
    display[2]=0;
    display[3]++;         //秒十位
   }
   if(display[3]==6)
   {
    display[3]=0;
    display[4]++;         //分个位
   }
   if(display[4]==10)
   {
    display[4]=0;
    display[5]++;         //分十位
   }
   if(display[5]==6)
   {
    display[5]=0;
    display[6]++;        //时
   }
   if(display[6]==24)
   {
     display[6]=0;
   } 
}

/*********************************************************/

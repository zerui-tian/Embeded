//1.	  程序分别下载到2个开发板，注意两个板都要接上USB电源供电。
//2.	然后按S2后发送信息，发送完毕后蜂鸣器响一下，对方受到信息后同样 响一下。
//3.	而且两个开发板都可以做为发送和接收，一个做为发送，对应另一即为接收,实现及时双向通信。
//4.    QX-MCS51开发板上最下角第一个按键
// 

#include <reg52.h>
#include <ABSACC.h>
#include <intrins.h>
#include <stdio.h>
//----------------------------------------------------------------------------------------------------------------
#define uint unsigned int
#define uchar unsigned char
//----------------------------------------------------------------------------------------------------------------
#define BYTE_BIT0	0x01
#define BYTE_BIT7	0x80
//----------------------------------------------------------------------------------------------------------------
bdata unsigned  char DATA_BUF;
#define DATA7	((DATA_BUF&BYTE_BIT7) != 0)
#define DATA0   ((DATA_BUF&BYTE_BIT0) != 0)
sbit	flag	=DATA_BUF^7;
sbit	flag1	=DATA_BUF^0;
//---------------------------------------------------发送数据缓冲区-------------------------------------------------
#define TxRxBuf_Len 4
unsigned char TxRxBuf[TxRxBuf_Len]={0x29,0x30,0x31,0x32,};
code TxAddress[4]={0xcc,0xcc,0xcc,0xcc};
char tf;
//----------------------------------------NRF905工作模式控制端口------------------------------------------------------
sbit	TXEN=P1^0;
sbit	TRX_CE=P3^2;
sbit	PWR=P1^1;
//-------------------------------------------------------------------------------------------
sbit    LED=P2^3; //蜂鸣器
//----------------------------------------NRF905 数据交换端口---------------------------------------------------
sbit	MISO=P1^6;
sbit	MOSI=P1^5;
sbit	SCK=P1^7;
sbit	CSN=P1^3;
//----------------------------------------nrf905状态端口---------------------------------------------------------
sbit	AM=P1^4;
sbit	DR=P3^3;
sbit	CD=P1^2;
//---------------------------------------------------------------------------------------------------------------
//----------------------------------------按键端口-------------------------------------------------------
sbit	KEY=P3^0;  //QX-MCS51开发板上左下角第一个按键
//----------------------------------------nrf905控制指令-------------------------------------------
#define WC		0x00
#define RC		0x10
#define WTP		0x20
#define RTP		0x21
#define WTA		0x22
#define RTA		0x23
#define RRP		0x24
//------------------------------------------------NRF905寄存器配置------------------------------------------------
unsigned char idata RFConf[11]=
{
  0x00,                             //配置命令//
  0x4c,                             //CH_NO,配置频段在430MHZ
  0x0c,                             //输出功率为10db,不重发，节电为正常模式
  0x44,                             //地址宽度设置，为4字节
  0x04,0x04,                        //接收发送有效数据长度为4字节
  0xCC,0xCC,0xCC,0xCC,              //接收地址
  0x58,                              //CRC充许，8位CRC校验，外部时钟信号不使能，16M晶振
};
//================================================延时===========================================================
void nrf905_Delay(int n)
{
	uint i;
	while(n--)
	for(i=0;i<80;i++);
}
//=================================================SPI读函数=======================================================
unsigned char SpiRead(void)
{
	unsigned char j;
	for (j=0;j<8;j++)
	{
        DATA_BUF=DATA_BUF<<1;
		SCK=1;
		if (MISO)	//读取最高位，保存至最末尾，通过左移位完成整个字节
		{
			DATA_BUF|=BYTE_BIT0;
		}
		else
		{
			DATA_BUF&=~BYTE_BIT0;
		}
		SCK=0;
	 }
	 return DATA_BUF;
}
//===========================================SPI写函数===============================================================
void SpiWrite(unsigned char send)
{
	unsigned char i;
	DATA_BUF=send;
	for (i=0;i<8;i++)
	{
		if (DATA7)	//总是发送最高位
		{
			MOSI=1;
		}
		else
		{
			MOSI=0;
		}
		SCK=1;
		DATA_BUF=DATA_BUF<<1;
		SCK=0;
	}
}
//------------------------------------------------------初始化nRF905---------------------------------------------
void nRF905Init(void)
{
    CSN=1;						// Spi 	disable
	SCK=0;						// Spi clock line init low
	DR=1;						// Init DR for input
	AM=1;						// Init AM for input
	CD=1;						// Init CD for input
	PWR=1;					// nRF905 power on
	TRX_CE=0;					// Set nRF905 in standby mode
	TXEN=0;					// set radio in Rx mode
}
//-----------------------------------------------------初始化寄存器-----------------------------------------------
void Config905(void)
{
	uchar i;
	CSN=0;						// Spi enable for write a spi command
	//SpiWrite(WC);				// Write config command写放配置命令
	for (i=0;i<11;i++)	// Write configration words  写放配置字
	{
	   SpiWrite(RFConf[i]);
	}
	CSN=1;					// Disable Spi
}
//-----------------------------------------------------发送数据打包---------------------------------------------------
void TxPacket(uchar *TxRxBuf)
{
	uchar i;
	//Config905();
	CSN=0;
	SpiWrite(WTP);				// Write payload command
	for (i=0;i<4;i++)
	{
		SpiWrite(TxRxBuf[i]);		// Write 32 bytes Tx data
	}
	CSN=1;
	nrf905_Delay(1);						// Spi disable
	CSN=0;						// Spi enable for write a spi command
	SpiWrite(WTA);				// Write address command
	for (i=0;i<4;i++)			// 写入与对方地址一样的地址
	{
		SpiWrite(TxAddress[i]);
	}
	CSN=1;						// Spi disable
	TRX_CE=1;					// Set TRX_CE high,start Tx data transmission
	nrf905_Delay(1);					// while (DR!=1);
	TRX_CE=0;					// Set TRX_CE low
}
//----------------------------------------------设置发送初始状态---------------------------------------------
void SetTxMode(void)
{
	TRX_CE=0;
	TXEN=1;
	nrf905_Delay(1); 					// nrf905_Delay for mode change(>=650us)
}
//-----------------------------------------------设置接收初始化---------------------------------------------------
void SetRxMode(void)
{
	TXEN=0;
	TRX_CE=1;
	nrf905_Delay(1); 					// nrf905_Delay for mode change(>=650us)
}
//-------------------------------------------------判断数据接收状态-----------------------------------------------------
unsigned char CheckDR(void)		//检查是否有新数据传入 Data Ready
{
	DR=1;      
//通过对端口写1，可以使端口为输入状态，这51的 特性。不熟悉者可以参阅51相关书籍作证(将DR端口设置为输入状态。)
	if (DR==1)
	{
		DR=0;
		return 1;
	}
	else
	{
		return 0;
	}

}
//----------------------------------------------------读NRF905接收数据------------------------------------------------------------
void RxPacket(void)						
{
	uchar i;
    nrf905_Delay(1);
//	TRX_CE=0;					// Set nRF905 in standby mode
    nrf905_Delay(100);
    TRX_CE=0;
	CSN=0;						// Spi enable for write a spi command
    nrf905_Delay(1);
	SpiWrite(RRP);
	for (i = 0 ;i < 4 ;i++)
	{ 
		TxRxBuf[i]=SpiRead();		// Read data and save to buffer       
	}
	CSN=1;
    nrf905_Delay(10);
	TRX_CE=1;							
}
//--------------------------------------------------------数据接收------------------------------------------------
void  RX(void)
{
          SetRxMode();	
        //  while (CheckDR()==0);  为了实现双向通信，就不能一直处于接收等待状态，所以注释掉
		  nrf905_Delay(10);
	  	  RxPacket();
		  if(TxRxBuf[0]==0x29)
		  {
		  		LED=0;
			    nrf905_Delay(300);
				LED=1;
			    nrf905_Delay(300);//接收到数据 后闪烁
		  }

}
//-----------------------------------------------------------------------------------------------------------------
void main(void)
{
        nRF905Init();
     	Config905();
		LED=1;
        while(1)
		{
			RX();
		   if(KEY ==0 )
			 {	
			 	while(KEY==0);
				tf = 1 ;
				TxRxBuf[0]=0x29;
				 }
		  
	       if (tf==1)
			{
                SetTxMode();
			    TxPacket(TxRxBuf);	// 发送命令数据
  				LED=0;
			    nrf905_Delay(300);
				LED=1;
			    nrf905_Delay(300);			//发送后蜂鸣器响一下
				tf = 0;	
			}	
}
}

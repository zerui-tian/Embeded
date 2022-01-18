//1.	  ����ֱ����ص�2�������壬ע�������嶼Ҫ����USB��Դ���硣
//2.	Ȼ��S2������Ϣ��������Ϻ��������һ�£��Է��ܵ���Ϣ��ͬ�� ��һ�¡�
//3.	�������������嶼������Ϊ���ͺͽ��գ�һ����Ϊ���ͣ���Ӧ��һ��Ϊ����,ʵ�ּ�ʱ˫��ͨ�š�
//4.    QX-MCS51�����������½ǵ�һ������
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
//---------------------------------------------------�������ݻ�����-------------------------------------------------
#define TxRxBuf_Len 4
unsigned char TxRxBuf[TxRxBuf_Len]={0x29,0x30,0x31,0x32,};
code TxAddress[4]={0xcc,0xcc,0xcc,0xcc};
char tf;
//----------------------------------------NRF905����ģʽ���ƶ˿�------------------------------------------------------
sbit	TXEN=P1^0;
sbit	TRX_CE=P3^2;
sbit	PWR=P1^1;
//-------------------------------------------------------------------------------------------
sbit    LED=P2^3; //������
//----------------------------------------NRF905 ���ݽ����˿�---------------------------------------------------
sbit	MISO=P1^6;
sbit	MOSI=P1^5;
sbit	SCK=P1^7;
sbit	CSN=P1^3;
//----------------------------------------nrf905״̬�˿�---------------------------------------------------------
sbit	AM=P1^4;
sbit	DR=P3^3;
sbit	CD=P1^2;
//---------------------------------------------------------------------------------------------------------------
//----------------------------------------�����˿�-------------------------------------------------------
sbit	KEY=P3^0;  //QX-MCS51�����������½ǵ�һ������
//----------------------------------------nrf905����ָ��-------------------------------------------
#define WC		0x00
#define RC		0x10
#define WTP		0x20
#define RTP		0x21
#define WTA		0x22
#define RTA		0x23
#define RRP		0x24
//------------------------------------------------NRF905�Ĵ�������------------------------------------------------
unsigned char idata RFConf[11]=
{
  0x00,                             //��������//
  0x4c,                             //CH_NO,����Ƶ����430MHZ
  0x0c,                             //�������Ϊ10db,���ط����ڵ�Ϊ����ģʽ
  0x44,                             //��ַ������ã�Ϊ4�ֽ�
  0x04,0x04,                        //���շ�����Ч���ݳ���Ϊ4�ֽ�
  0xCC,0xCC,0xCC,0xCC,              //���յ�ַ
  0x58,                              //CRC����8λCRCУ�飬�ⲿʱ���źŲ�ʹ�ܣ�16M����
};
//================================================��ʱ===========================================================
void nrf905_Delay(int n)
{
	uint i;
	while(n--)
	for(i=0;i<80;i++);
}
//=================================================SPI������=======================================================
unsigned char SpiRead(void)
{
	unsigned char j;
	for (j=0;j<8;j++)
	{
        DATA_BUF=DATA_BUF<<1;
		SCK=1;
		if (MISO)	//��ȡ���λ����������ĩβ��ͨ������λ��������ֽ�
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
//===========================================SPIд����===============================================================
void SpiWrite(unsigned char send)
{
	unsigned char i;
	DATA_BUF=send;
	for (i=0;i<8;i++)
	{
		if (DATA7)	//���Ƿ������λ
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
//------------------------------------------------------��ʼ��nRF905---------------------------------------------
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
//-----------------------------------------------------��ʼ���Ĵ���-----------------------------------------------
void Config905(void)
{
	uchar i;
	CSN=0;						// Spi enable for write a spi command
	//SpiWrite(WC);				// Write config commandд����������
	for (i=0;i<11;i++)	// Write configration words  д��������
	{
	   SpiWrite(RFConf[i]);
	}
	CSN=1;					// Disable Spi
}
//-----------------------------------------------------�������ݴ��---------------------------------------------------
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
	for (i=0;i<4;i++)			// д����Է���ַһ���ĵ�ַ
	{
		SpiWrite(TxAddress[i]);
	}
	CSN=1;						// Spi disable
	TRX_CE=1;					// Set TRX_CE high,start Tx data transmission
	nrf905_Delay(1);					// while (DR!=1);
	TRX_CE=0;					// Set TRX_CE low
}
//----------------------------------------------���÷��ͳ�ʼ״̬---------------------------------------------
void SetTxMode(void)
{
	TRX_CE=0;
	TXEN=1;
	nrf905_Delay(1); 					// nrf905_Delay for mode change(>=650us)
}
//-----------------------------------------------���ý��ճ�ʼ��---------------------------------------------------
void SetRxMode(void)
{
	TXEN=0;
	TRX_CE=1;
	nrf905_Delay(1); 					// nrf905_Delay for mode change(>=650us)
}
//-------------------------------------------------�ж����ݽ���״̬-----------------------------------------------------
unsigned char CheckDR(void)		//����Ƿ��������ݴ��� Data Ready
{
	DR=1;      
//ͨ���Զ˿�д1������ʹ�˿�Ϊ����״̬����51�� ���ԡ�����Ϥ�߿��Բ���51����鼮��֤(��DR�˿�����Ϊ����״̬��)
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
//----------------------------------------------------��NRF905��������------------------------------------------------------------
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
//--------------------------------------------------------���ݽ���------------------------------------------------
void  RX(void)
{
          SetRxMode();	
        //  while (CheckDR()==0);  Ϊ��ʵ��˫��ͨ�ţ��Ͳ���һֱ���ڽ��յȴ�״̬������ע�͵�
		  nrf905_Delay(10);
	  	  RxPacket();
		  if(TxRxBuf[0]==0x29)
		  {
		  		LED=0;
			    nrf905_Delay(300);
				LED=1;
			    nrf905_Delay(300);//���յ����� ����˸
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
			    TxPacket(TxRxBuf);	// ������������
  				LED=0;
			    nrf905_Delay(300);
				LED=1;
			    nrf905_Delay(300);			//���ͺ��������һ��
				tf = 0;	
			}	
}
}

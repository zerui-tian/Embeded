
#include "reg52.h"	/*包含STC单片机头文件*/
#include "QX51/qx51.h" /*包含qx51开发板头文件*/
#include "DELAY/delay.h"/*包含delay延时函数文件*/
#include "NRFreceive.h"//包涵NRF驱动

uchar const TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address
uchar rx_buf[TX_PLOAD_WIDTH];
uchar tx_buf[TX_PLOAD_WIDTH];

/**************************************************/
uchar 	bdata sta;
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;	
/*************************************************/
 //NRF24L01+

uchar SPI_RW(uchar dat)//写一字节并读出此地址的状态
 {
 	uchar i;
	for(i=0;i<8;i++)
	{
		SCK=0;
		MOSI=(dat & 0x80);
		dat<<=1;
		SCK=1;
		dat|=MISO;		
	}
	SCK=0;	   //拉低时钟保持通信状态
	return dat;
 }
uchar SPI_RW_Reg(uchar reg,value)//写一字节并读出此地址的状态
 {
 	uchar status;
	CSN=0;
	status=SPI_RW(reg);
	SPI_RW(value);
	CSN=1;
	return status;
 }
uchar SPI_Read(uchar reg)  //读一字节
{
	uchar value;
	CSN=0;
	SPI_RW(reg);
	value=SPI_RW(0);
	CSN=1;
	return value;
}
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar uchars)//读多个字节并读出此地址的状态
{
	uchar status,i;
	CSN=0;
	status=SPI_RW(reg);
	for(i=0;i<uchars;i++)
	pBuf[i]=SPI_RW(0);
	CSN=1;
	return status;	
}
uchar SPI_write_Buf(uchar reg,uchar *pBuf,uchar uchars)//写多个字节并读出此地址的状态
{
	uchar status,i;
	CSN=0;
	status=SPI_RW(reg);
	for(i=0;i<uchars;i++)
	SPI_RW(*pBuf++);
	CSN=1;
	return status;
}/*
void nrf24l01_TXBUF(uchar *tx_buf) //发送tx_buf中的数据
{
	CE=0;//停止射频工作
	SPI_write_Buf(0x20+10,0x01,5);//写本机地址
	SPI_write_Buf(0x20+10,0x01,5);//装载接收端O通道地址
	SPI_write_Buf(0xa0,tx_buf,2);//装载要发送的数据
	SPI_RW_Reg(0x20+0x00,0x0e);//IRQ收发完成中断响应16位CRC，发送模式
	CE=1; //置高CE，激发数据发射
	delay(1);
} */
void nrf24l01_init()
{
	NRF_CE=0;
	CSN=1;
	SCK=0;
	IRQ=1;
	delay8us();
	delay8us();
}
void setRX_Mode()
{
	NRF_CE=0;
  	SPI_write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 写接收地址到0通道
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x00);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  	SPI_RW_Reg(WRITE_REG + RF_CH,40);        // Select 工作频段 channel 2.4G
  	SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);     //IRQ中断响应16位CRC校验，接收模式

  	NRF_CE = 1; // Set CE pin high to enable RX device
	delay_ms(1);
}
uchar nRF24L01_RxPacket(uchar *rx_buf)
{
	uchar flag=0;
	sta=SPI_Read(STATUS);	// read register STATUS's value
	if(RX_DR)				// if receive data ready (RX_DR) interrupt
	{
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		flag=1;
		SPI_RW_Reg(WRITE_REG+STATUS,0xff);//清空状态寄存器
	}		
	return flag;
}
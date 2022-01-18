
#include "reg52.h"	/*����STC��Ƭ��ͷ�ļ�*/
#include "QX51/qx51.h" /*����qx51������ͷ�ļ�*/
#include "DELAY/delay.h"/*����delay��ʱ�����ļ�*/
#include "NRFreceive.h"//����NRF����

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

uchar SPI_RW(uchar dat)//дһ�ֽڲ������˵�ַ��״̬
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
	SCK=0;	   //����ʱ�ӱ���ͨ��״̬
	return dat;
 }
uchar SPI_RW_Reg(uchar reg,value)//дһ�ֽڲ������˵�ַ��״̬
 {
 	uchar status;
	CSN=0;
	status=SPI_RW(reg);
	SPI_RW(value);
	CSN=1;
	return status;
 }
uchar SPI_Read(uchar reg)  //��һ�ֽ�
{
	uchar value;
	CSN=0;
	SPI_RW(reg);
	value=SPI_RW(0);
	CSN=1;
	return value;
}
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar uchars)//������ֽڲ������˵�ַ��״̬
{
	uchar status,i;
	CSN=0;
	status=SPI_RW(reg);
	for(i=0;i<uchars;i++)
	pBuf[i]=SPI_RW(0);
	CSN=1;
	return status;	
}
uchar SPI_write_Buf(uchar reg,uchar *pBuf,uchar uchars)//д����ֽڲ������˵�ַ��״̬
{
	uchar status,i;
	CSN=0;
	status=SPI_RW(reg);
	for(i=0;i<uchars;i++)
	SPI_RW(*pBuf++);
	CSN=1;
	return status;
}/*
void nrf24l01_TXBUF(uchar *tx_buf) //����tx_buf�е�����
{
	CE=0;//ֹͣ��Ƶ����
	SPI_write_Buf(0x20+10,0x01,5);//д������ַ
	SPI_write_Buf(0x20+10,0x01,5);//װ�ؽ��ն�Oͨ����ַ
	SPI_write_Buf(0xa0,tx_buf,2);//װ��Ҫ���͵�����
	SPI_RW_Reg(0x20+0x00,0x0e);//IRQ�շ�����ж���Ӧ16λCRC������ģʽ
	CE=1; //�ø�CE���������ݷ���
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
  	SPI_write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // д���յ�ַ��0ͨ��
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x00);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  	SPI_RW_Reg(WRITE_REG + RF_CH,40);        // Select ����Ƶ�� channel 2.4G
  	SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);     //IRQ�ж���Ӧ16λCRCУ�飬����ģʽ

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
		SPI_RW_Reg(WRITE_REG+STATUS,0xff);//���״̬�Ĵ���
	}		
	return flag;
}
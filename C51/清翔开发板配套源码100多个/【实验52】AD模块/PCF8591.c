#include "qxmcs51_config.h"
#include "delay.h"
#include "IIC.h"
#include "PCF8591.H"

#define DAC_EN 0x40
#define ADC_AutoINC 0x04
/*DACÊä³ö*//*
bit I2C_DAC_WriteData(uchar Data)
{
	I2C_Start();
	I2C_send_byte(PCF8591_ADDR + 0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(DAC_EN); 
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(Data); 
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_Stop();
	return(1);	
}	 */

/*AD¶ÁÊý¾Ý*/
BOOL I2C_ADC_ReadData(uchar ADDR, uchar *ADC_Value)
{
	I2C_Start();
	I2C_send_byte(PCF8591_ADDR + 0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(ADDR);
	Master_ACK(0);
	I2C_Start();
	I2C_send_byte(PCF8591_ADDR + 1);
	if (!Test_ACK())
	{
		return(0);
	}
	*ADC_Value = I2C_read_byte();
	Master_ACK(0);
	I2C_Stop();
	return(1);	
}
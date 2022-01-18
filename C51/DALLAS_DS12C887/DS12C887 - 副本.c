#include<reg51.h>
#include<intrins.h>
#include<DS12C887.h>

#define ADDR_DATA P1

sbit DS12C887_DS = P3^4;
sbit DS12C887_RW = P3^5;
sbit DS12C887_AS = P3^6;
sbit DS12C887_CS = P3^7;

uchar Bin2BCD(uchar bin) {
	uchar bcd;
	bcd = bin % 10;
	bcd |= (bin / 10) << 4;
	return bcd;
}

uchar DS12C887_ReadData(uchar addr){
	
	uchar dat;
	
	DS12C887_AS=1;
	DS12C887_DS=1;
	DS12C887_RW=1;
	DS12C887_CS=0;
	
	ADDR_DATA=addr;
	
	DS12C887_AS=0;
	DS12C887_DS=0;
	
	ADDR_DATA=0xff;
	
	dat=ADDR_DATA;
	
	DS12C887_DS=1;
	DS12C887_AS=1;
	DS12C887_CS=1;
	
	return dat;
}

void DS12C887_WriteData(uchar addr,uchar dat){
	
	ADDR_DATA=0xff;
	DS12C887_CS=0;
	DS12C887_AS=1;
	DS12C887_DS=1;
	DS12C887_RW=1;
	
	ADDR_DATA=addr;
	DS12C887_RW=0;
	DS12C887_AS=0;
	
	ADDR_DATA=dat;
	DS12C887_RW=1;
	DS12C887_AS=1;
	DS12C887_CS=1;
}

void DS12C887_Init(){
	DS12C887_WriteData(REG_A, REG_A_INIT); //Turn on oscillator, disable SQW
	DS12C887_WriteData(REG_B, REG_B_INIT); //Enable alarm, 12-hour mode, BCD
}

uchar* DS12C887_GetTime(){
	uchar time[GETTIME_SIZE];
	time[0] = DS12C887_ReadData(SEC_ADDR);
	time[1] = DS12C887_ReadData(MIN_ADDR);
	time[2] = DS12C887_ReadData(HOUR_ADDR);
	time[3] = DS12C887_ReadData(DAY_OF_WEEK_ADDR);
	time[4] = DS12C887_ReadData(DAY_OF_MONTH_ADDR);
	time[5] = DS12C887_ReadData(MONTH_ADDR);
	time[6] = DS12C887_ReadData(YEAR_ADDR);
	time[7] = DS12C887_ReadData(CENTURY_ADDR);
	return time;
}

void DS12C887_SetTime(uchar* aim){
	uchar dat;
	bit isPM;
	
	DS12C887_WriteData(REG_B,REG_B_INIT|0x80);
	
	dat = Bin2BCD( aim[0] );
	DS12C887_WriteData(SEC_ADDR,dat);
	
	dat = Bin2BCD( aim[1] );
	DS12C887_WriteData(MIN_ADDR,dat);
	
	isPM = aim[2] >= 12;
	if(aim[2] > 12){
		aim[2] -= 12;
	}
	else{
		/* do nothing */
	}
	dat = Bin2BCD( aim[2] );
	if(isPM){
		dat |= 0x80;
	}
	else{
		/* do nothing */
	}
	DS12C887_WriteData(HOUR_ADDR,dat);
	
	dat = ((aim[6] + (aim[6]>>2) + (aim[7]>>2) - 2*aim[7] + (26*(aim[5]+1)/10) + aim[4] - 1) % 7) + 1;
	DS12C887_WriteData(DAY_OF_WEEK_ADDR,dat);
	
	dat = Bin2BCD( aim[4] );
	DS12C887_WriteData(DAY_OF_MONTH_ADDR,dat);
	
	dat = Bin2BCD( aim[5] );
	DS12C887_WriteData(MONTH_ADDR,dat);
	
	dat = Bin2BCD( aim[6] );
	DS12C887_WriteData(YEAR_ADDR,dat);
	
	dat = Bin2BCD( aim[7] );
	DS12C887_WriteData(CENTURY_ADDR,dat);
	
	DS12C887_WriteData(REG_B,REG_B_INIT);
}

#include <reg52.h>
#include <intrins.h>

#define DEBUG 0

#define TRUE 	1
#define FALSE 0

#define HIGH	1
#define LOW		0

#define FRAME_LENGTH_BPC 12

#define SCAN_PERIOD_H 0x00
#define SCAN_PERIOD_L 0x00

//time information
// 0			1				2				3				4				5				6				7
// second minute 	hour 		weekday date 		month 	year 		century
// 0-59		0-59		0-23		0-6			1-31		1-12		00-99		00-99
#define SECOND		0
#define MINUTE		1
#define HOUR			2
#define WEEKDAY		3
#define DATE			4
#define MONTH			5
#define YEAR			6
#define CENTURY		7
xdata char time_BPC[8];

#define COUNTDOWN_RESETBITSRECIEVER_BPC 100
char countDown_ResetBitsReciever_BPC = COUNTDOWN_RESETBITSRECIEVER_BPC;

//signals
sbit DATA_BPC 	= P3^3;
sbit RESET_BPC 	= P3^5;
sbit PON_BPC		= P3^4;

//counters
char interruptCounter_BitsReciever_BPC = 0;

//recieve buffers
char buffer_BPC[FRAME_LENGTH_BPC];//{0x02,0x01,0x04,0x0C,0x03,0x0E,0x0A,0x05,0x05,0x00,0x00,0x00};

//flags
bit resetBPC = 				TRUE;
bit recentlySucceed = FALSE;
bit recentlyRecieve = FALSE;

/************************************************
 *                 Initializer                  *
 ************************************************/
void initializer(){
	//timers
	TMOD=0x21;
	TH0=SCAN_PERIOD_H;
	TL0=SCAN_PERIOD_L;
	TL1=0xFD;
	TH1=0xFD;//baudrate 9600
	
	//serial
	SCON=0x50;
	PCON=0x00;
	
	//external: negative edge
	IT0=TRUE;
	IT1=TRUE;
	
	//eable timers
	TR0=TRUE;
	TR1=TRUE;
	
	//priority
	IP = 0x01;
	
	//reset BPC reciever
	RESET_BPC=HIGH;

	//eable IRQs
	ET0=TRUE;
	EX0=TRUE;
	ES=TRUE;
	
	//eable interruption
	EA=TRUE;
}

void main(){
	initializer();
	while(TRUE){
	}
}

void EX0_IRQ(void) interrupt 0{
	char i;
	char checkSum;
	
	recentlyRecieve=TRUE;
	
	if(interruptCounter_BitsReciever_BPC%4==0){
		buffer_BPC[interruptCounter_BitsReciever_BPC/4] = DATA_BPC==HIGH?0x01:0x00;
	}
	else{
		buffer_BPC[interruptCounter_BitsReciever_BPC/4] <<= 1;
		buffer_BPC[interruptCounter_BitsReciever_BPC/4] |= DATA_BPC==HIGH?0x01:0x00;
	}
	interruptCounter_BitsReciever_BPC++;
	
	if(interruptCounter_BitsReciever_BPC>=FRAME_LENGTH_BPC*4){
		
		//reset counter
		interruptCounter_BitsReciever_BPC=0;
		
		//calculate checksum
		checkSum = 0x00;
		for(i=0; i<FRAME_LENGTH_BPC-1; i++){checkSum += buffer_BPC[i];}
		checkSum = checkSum&0x0F;
		
		//calculate validity and fix
		if((buffer_BPC[3]&0x0C)==0x0C){
				if(checkSum==buffer_BPC[11]){
					recentlySucceed = TRUE;
				}
				else{
					recentlySucceed = FALSE;
				}
		}
		else{
			recentlySucceed = FALSE;
		}
		
		//if the frame is valid, update time information
		if(recentlySucceed){
			time_BPC[SECOND] = 	(buffer_BPC[10]&0x0F)|(buffer_BPC[9]<<4);
			time_BPC[MINUTE] = 	(buffer_BPC[8]&0x0F)|(buffer_BPC[7]<<4);
			
			time_BPC[HOUR] = buffer_BPC[6]&0x0F;
			if((buffer_BPC[5]&0x08) && (buffer_BPC[6]!=0x0C)){time_BPC[HOUR] = time_BPC[HOUR] + 12;}
			buffer_BPC[6] = time_BPC[HOUR];
			time_BPC[HOUR] = 		(buffer_BPC[6]%10)|((buffer_BPC[6]/10)<<4);

			time_BPC[WEEKDAY] =  buffer_BPC[5]&0x07;
			time_BPC[DATE] = 		(buffer_BPC[4]&0x0F)|((buffer_BPC[3]&0x03)<<4);
			time_BPC[MONTH] = 	(buffer_BPC[2]%10)|((buffer_BPC[2]/10)<<4);
			time_BPC[YEAR] = 		(buffer_BPC[1]&0x0F)|(buffer_BPC[0]<<4);
			time_BPC[CENTURY] =  0x20;
		}
		
		for(i=0; i<FRAME_LENGTH_BPC; i++){
			SBUF=buffer_BPC[i];
			while(!TI);
			TI=FALSE;
		}
		SBUF=checkSum;
		while(!TI);
		TI=FALSE;
		
		resetBPC = !recentlySucceed;
	}
}

void Timer0_IRQ(void) interrupt 1{
	if(recentlyRecieve){
		if(countDown_ResetBitsReciever_BPC<=0){
			recentlyRecieve=FALSE;
		}
		else{
			countDown_ResetBitsReciever_BPC--;
		}
	}
	else{
		countDown_ResetBitsReciever_BPC = COUNTDOWN_RESETBITSRECIEVER_BPC;
		interruptCounter_BitsReciever_BPC = 0;
	}
	if(resetBPC){
		if(RESET_BPC==HIGH){
			RESET_BPC=LOW;
			resetBPC=FALSE;
		}
		else{
			RESET_BPC=HIGH;
		}
	}
	else{
		RESET_BPC=LOW;
	}
	
	TH0=SCAN_PERIOD_H;
	TL0=SCAN_PERIOD_L;
}
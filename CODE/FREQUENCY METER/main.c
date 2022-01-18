#include<LCD1602.h>
#include<reg51.h>
#include<main.h>
#include<LCD1602.h>
#include<math.h>

//negative edge counter
unsigned int negedgeCounter;

//buffers
unsigned int counterBuffer;
unsigned int counterBuffer_last;
unsigned char gearBuffer;

//state variables and flags
unsigned char gear;
bit updateFlag;
bit isBusy_Updater;
bit fipfop;
bit overflow;
bit hasSignal;

//strings and tables
char line1[16] = "FREQUENCY  METER";
char unitTable[3] = {' ','k','M'};
unsigned char gearTable[4] = {0x7E, 0x9F, 0xF3, 0xED};


void Frequency_Updater(unsigned int counter, unsigned char gear){
	char line2[16] = "              Hz";
	unsigned int counter_temp = counter;
	if(counter_temp > 0){
		unsigned char orderOfCounter=0;
		unsigned char orderOfFreq;
		long int power10 = 1;
		int ptr=0;
		isBusy_Updater = 1;
		
		//[orderOfCounter] is the number of digits in [counter]
		//	e.g. 1    -> [orderOfCounter]=1
		//	e.g. 1000 -> [orderOfCounter]=4
		while(counter_temp/power10!=0){
			power10 *= 10;
			orderOfCounter++;
		}
		
		//[orderOfFreq] is the number of digits in real frequency value
		orderOfFreq = orderOfCounter + 3 - gear;
		
		//add unit prefix
		// e.g. orderOfFreq=4 unitTable[1]='k'
		// e.g. orderOfFreq=8 unitTable[2]='M'
		line2[13] = unitTable[(orderOfFreq-1)/3];
		
		while(1){
			//do not add fraction point at the beginning or the end of the number
			if(orderOfFreq%3==0&&ptr!=0&&orderOfFreq!=0){
				line2[ptr++] = '.';
			}
			counter_temp %= power10;
			power10 /= 10;
			line2[ptr++]=(char)(counter_temp/power10)+'0';
			if(power10<=1){
				break;
			}
			orderOfFreq--;
		}
		
		//patch the digits
		if(gear==2&&orderOfCounter<3){
			line2[ptr++] = '*';
		}
		else if(gear==1&&counter<10){
			line2[ptr++] = '*';
			line2[ptr++] = '*';
		}
		else{
		}
	}
	else{
		line2[0] = '0';
	}
	LCD1602_Refresh(line1, line2);
	updateFlag=0;
	isBusy_Updater = 0;
}

void initializer(){
	LCD1602_Init();
	
	//initial variables
	isBusy_Updater = 0;
	negedgeCounter = 0;
	updateFlag=0;
	fipfop = 0;
	gear = 0;
	gearBuffer = 1;
	hasSignal = 1;
	overflow = 0;
	P2 = gearTable[gear];
	counterBuffer_last = 0xFFFF;
	
	IP = 0x01;//EX0 has the highest power
	
	IT0=1;//negative edge trigger external interrupt0
	IT1=1;//negative edge trigger external interrupt1
	EX0=1;//enable trigger external interrupt0
	EX1=1;//enable trigger external interrupt0
	EA=1;
}

void main(){
	initializer();
	while(1){
		if(updateFlag){
			Frequency_Updater(counterBuffer, gearBuffer);
		}
	}
}

void periodical_interrupt_500ms() interrupt 2{
	if(isBusy_Updater){
		return;
	}
	hasSignal = !((counterBuffer==0&&gear==3)||!hasSignal);
	if(fipfop){
		counterBuffer_last = counterBuffer;//cache last [counterBuffer]
		counterBuffer=negedgeCounter;//record [negedgeCounter]
		gearBuffer = gear;//record [gear]
		negedgeCounter=0;//reset the negative edge counter
		updateFlag=1;//inform update function
	}
	else{
		if(hasSignal){
			if(gear<3&&!overflow&&
				((counterBuffer <= 3300&&abs(counterBuffer_last-counterBuffer)<counterBuffer/100)||
					(counterBuffer <= 100)
				)
			){
				gear++;
			}
			else if(overflow&&gear>0){
				gear = 0;
				overflow = 0;
			}
			else{
			}
		}
		else{
			hasSignal = counterBuffer>0;
		}
	}
	P2 = gearTable[gear];
	fipfop = ~fipfop;
}
void negetive_edge_counter() interrupt 0{
	overflow = negedgeCounter > 0x80E7;
	negedgeCounter++;
}
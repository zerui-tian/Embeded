#include<LCD1602.h>
#include<reg52.h>

/***********
 *	Modes  *
 ***********/
#define NORMAL				0
#define ADJUSTYEAR		1
#define ADJUSTMONTH		2
#define ADJUSTDATE		3
#define ADJUSTWEEKDAY	4
#define ADJUSTHOUR		5
#define ADJUSTMINUTE	6
#define RESETSECOND		7
#define CHANGELIGHT		8
#define SWITCHSOUND		9
#define UPDATESTATE		10
char mode = NORMAL;

/**********
 *	Time  *
 **********/
#define SECOND 0
#define MINUTE 1
#define HOUR 2
#define WEEKDAY 3
#define DAY 4
#define MONTH 5
#define YEAR 6
int innerClock[7] = {0,0,0,4,1,1,1970};
int receivedTimeBuffer1[7];
int receivedTimeBuffer2[7];
idata int *currentReceivedTime = receivedTimeBuffer1;
idata int *lastReceivedTime = receivedTimeBuffer2;
idata int *addrExchangeTempVar;
bit refreshTimeFlag = 0;

/******************************
 *	Flags of light and sound  *
 ******************************/
#define OFF 	0
#define ON  	1
#define AUTO	2
char light = OFF;
char colorCode = 7;
bit hourlyChime = ON;

#define DELAYXMS			20

sbit test = P2^4;

/********************************************
 *	Radio Signal and Antenna Control Ports  *
 ********************************************/
sbit antennaIsOff = P3^4;
sbit signal = P3^2;

/***************************
 *	Light and Sound Ports  *
 ***************************/
sbit red = P2^7;
sbit green = P2^6;
sbit blue = P2^5;
sbit buzz = P2^0;

/*******************
 *	Buttons Ports  *
 *******************/
sbit middle = P1^7;
sbit up = P1^5;
sbit down = P1^4;
sbit left = P1^3;
sbit right = P1^6;

/*********************
 *	Display Strings  *
 *********************/
idata char displayString1[17];
idata char displayString2[17];

xdata char updateStatusString1[17] = 	"LAST UPDATE TIME";
xdata char updateStatusString2[17] = 	"****-**-** **:**";

xdata char lightModeString1[17] = 			"   LIGHT MODE   ";
xdata char lightModeString2[3][17] = {
	"       Off      ",
	"       On       ",
	"      Auto      "
};

xdata char soundSwitchString1[17] = 		"  HOURLY CHIME  ";
xdata char soundSwitchString2[2][17] = {
	"       Off      ",
	"       On       "
};

/*********************
 *	Weekday Strings  *
 *********************/
xdata char weekdayTable[7][3] = {
	{'S','U','N'},
	{'M','O','N'},
	{'T','U','E'},
	{'W','E','D'},
	{'T','H','U'},
	{'F','R','I'},
	{'S','A','T'}
};

/**************************************
 *	The number of days in each innerClock[MONTH]  *
 **************************************/
int monthdayAmount[12] = {
	31,28,31,30,31,30,31,31,30,31,30,31
};

/***************************
 *	Synchronization Flags  *
 ***************************/
bit fipfop = 0;
bit phaseSynchronized = 0;
bit timeSynchronized = 0;
/******************************************
 *	Synchronization Counters and Buffers  *
 ******************************************/
idata unsigned char mainCounter = 0;
idata unsigned int  syncCounter = 0;
idata unsigned char freqMatchCounter = 0;
idata unsigned char timeMatchCounter = 0;
idata unsigned char decodeCounter = 0;
idata unsigned char decodeBuffer[20];
idata unsigned char decodePointer = 0;

void delay(int xms){
	int x,y;
	for(x=xms;x>0;x--){
		for(y=110;y>0;y--){
		}
	}
}

void RefreshTime(){
	
	displayString1[0] = ' ';
	
	displayString1[1] = (mode!=ADJUSTYEAR)||fipfop?('0'+innerClock[YEAR]/1000):' ';
	displayString1[2] = (mode!=ADJUSTYEAR)||fipfop?('0'+(innerClock[YEAR]%1000)/100):' ';
	displayString1[3] = (mode!=ADJUSTYEAR)||fipfop?('0'+(innerClock[YEAR]%100)/10):' ';
	displayString1[4] = (mode!=ADJUSTYEAR)||fipfop?('0'+innerClock[YEAR]%10):' ';
	
	displayString1[5] = '-';
	
	displayString1[6] = (mode!=ADJUSTMONTH)||fipfop?('0' + innerClock[MONTH]/10):' ';
	displayString1[7] = (mode!=ADJUSTMONTH)||fipfop?('0' + innerClock[MONTH]%10):' ';

	displayString1[8] = '-';	
	
	displayString1[9] = (mode!=ADJUSTDATE)||fipfop?('0' + innerClock[DAY]/10):' ';
	displayString1[10] = (mode!=ADJUSTDATE)||fipfop?('0' + innerClock[DAY]%10):' ';
	
	displayString1[11] = ' ';
	
	displayString1[12] = (mode!=ADJUSTWEEKDAY)||fipfop?weekdayTable[innerClock[WEEKDAY]][0]:' ';
	displayString1[13] = (mode!=ADJUSTWEEKDAY)||fipfop?weekdayTable[innerClock[WEEKDAY]][1]:' ';
	displayString1[14] = (mode!=ADJUSTWEEKDAY)||fipfop?weekdayTable[innerClock[WEEKDAY]][2]:' ';

	displayString1[15] = ' ';
	
	displayString2[0] = ' ';
	displayString2[1] = ' ';
	displayString2[2] = ' ';
	displayString2[3] = ' ';
	
	displayString2[4] = (mode!=ADJUSTHOUR)||fipfop?('0' + innerClock[HOUR]/10):' ';
	displayString2[5] = (mode!=ADJUSTHOUR)||fipfop?('0' + innerClock[HOUR]%10):' ';

	displayString2[6] = (mode!=NORMAL)||fipfop?':':' ';

	displayString2[7] = (mode!=ADJUSTMINUTE)||fipfop?('0' + innerClock[MINUTE]/10):' ';
	displayString2[8] = (mode!=ADJUSTMINUTE)||fipfop?('0' + innerClock[MINUTE]%10):' ';

	displayString2[9] = (mode!=NORMAL)||fipfop?':':' ';

	displayString2[10] = (mode!=RESETSECOND)||fipfop?('0' + innerClock[SECOND]/10):' ';
	displayString2[11] = (mode!=RESETSECOND)||fipfop?('0' + innerClock[SECOND]%10):' ';
	
	displayString2[12] = ' ';
	displayString2[13] = ' ';
	displayString2[14] = ' ';
	displayString2[15] = ' ';
	
	LCD1602_Refresh(displayString1, displayString2);
}



bit UpdateTime(){
	int monthdayAmountTemp;
	if((currentReceivedTime[SECOND]==1)||(currentReceivedTime[SECOND]==21)||currentReceivedTime[SECOND]==41){
		if(currentReceivedTime[SECOND]==41){
			currentReceivedTime[SECOND] = 0;
			currentReceivedTime[MINUTE]++;
			if(currentReceivedTime[MINUTE] >= 60){
				currentReceivedTime[MINUTE] = 0;
				currentReceivedTime[HOUR]++;
			}
			if(currentReceivedTime[HOUR] >= 24){
				currentReceivedTime[HOUR] = 0;
				currentReceivedTime[DAY]++;
				currentReceivedTime[WEEKDAY] = (currentReceivedTime[WEEKDAY]+1)%7;
			}
			if(currentReceivedTime[MONTH]==2){
				monthdayAmountTemp = 
					((currentReceivedTime[YEAR]%4==0)&&(currentReceivedTime[YEAR]%100!=0))||(currentReceivedTime[YEAR]%400==0) ? 29 : 28;
			}
			else{
				monthdayAmountTemp = monthdayAmount[currentReceivedTime[MONTH]-1];
			}
			if(currentReceivedTime[DAY] > monthdayAmountTemp){
				currentReceivedTime[DAY] = 1;
				currentReceivedTime[MONTH]++;
			}
			if(currentReceivedTime[MONTH] > 12){
				currentReceivedTime[MONTH] = 1;
				currentReceivedTime[YEAR]++;
			}
		}
		else{
			currentReceivedTime[SECOND]+=19;
		}
		
		innerClock[SECOND] = currentReceivedTime[SECOND];
		innerClock[MINUTE] = currentReceivedTime[MINUTE];
		innerClock[HOUR] = currentReceivedTime[HOUR];
		innerClock[WEEKDAY] = currentReceivedTime[WEEKDAY];
		innerClock[DAY] = currentReceivedTime[DAY];
		innerClock[MONTH] = currentReceivedTime[MONTH];
		innerClock[YEAR] = currentReceivedTime[YEAR];
		
		return 1;
	}
	else{
		return 0;
	}
}

void ChangeColor(){
	if(light == ON){
		red = colorCode&0x01;
		green = colorCode&0x02;
		blue = colorCode&0x04;
	}
	else if(light == OFF){
		red = 0;
		green = 0;
		blue = 0;
	}
}

void MiddleKeyAction(){
	switch(mode){
		case NORMAL:
			mode = ADJUSTYEAR;
			break;
		
		case CHANGELIGHT:
			LCD1602_Refresh(soundSwitchString1,soundSwitchString2[hourlyChime]);
			mode = SWITCHSOUND;
			break;
		
		case SWITCHSOUND:
			LCD1602_Refresh(updateStatusString1,updateStatusString2);
			mode = UPDATESTATE;
			break;
		
		case UPDATESTATE:
			RefreshTime();
			mode = NORMAL;
			break;
		
		default:
			LCD1602_Refresh(lightModeString1,lightModeString2[light]);
			mode = CHANGELIGHT;
			break;
	}	
}



void UpKeyAction(){
	switch(mode){		
		case ADJUSTYEAR:
			EA = 0;
			innerClock[YEAR]++;
			monthdayAmount[1] = 
				((innerClock[YEAR]%4==0)&&(innerClock[YEAR]%100!=0))||(innerClock[YEAR]%400==0) ? 29 : 28;
			RefreshTime();
			EA = 1;
			break;
		
		case ADJUSTMONTH:
			EA = 0;
			innerClock[MONTH] = innerClock[MONTH]>=12?1:innerClock[MONTH]+1;
			RefreshTime();
			EA = 1;
			break;
		
		case ADJUSTDATE:
			innerClock[DAY] = innerClock[DAY]>=monthdayAmount[innerClock[MONTH]-1]?1:innerClock[DAY]+1;
			break;
		
		case ADJUSTWEEKDAY:
			innerClock[WEEKDAY] = innerClock[WEEKDAY]>=6?0:innerClock[WEEKDAY]+1;
			RefreshTime();
			break;
		
		case ADJUSTHOUR:
			innerClock[HOUR] = innerClock[HOUR]>=23?0:innerClock[HOUR]+1;
			RefreshTime();
			break;
		
		case ADJUSTMINUTE:
			innerClock[MINUTE] = innerClock[MINUTE]>=59?0:innerClock[MINUTE]+1;
			RefreshTime();
			break;
		
		case RESETSECOND:
			EA = 0;
			innerClock[SECOND] = 0;
			RefreshTime();
			EA = 1;
			break;
		
		case CHANGELIGHT:
			light = (light+1)%3;
			ChangeColor();
			LCD1602_Refresh(lightModeString1,lightModeString2[light]);
			break;
		
		case SWITCHSOUND:
			hourlyChime = ~hourlyChime;
			LCD1602_Refresh(soundSwitchString1,soundSwitchString2[hourlyChime]);
			break;
		
		case UPDATESTATE:
			break;
		
		default:
			/*do nothing*/
			break;
	}
}

void DownKeyAction(){
	switch(mode){		
		case ADJUSTYEAR:
			EA = 0;
			innerClock[YEAR]--;
			monthdayAmount[1] = 
				((innerClock[YEAR]%4==0)&&(innerClock[YEAR]%100!=0))||(innerClock[YEAR]%400==0) ? 29 : 28;
			RefreshTime();
			EA = 1;
			break;
		
		case ADJUSTMONTH:
			EA = 0;
			innerClock[MONTH] = innerClock[MONTH]<=1?12:innerClock[MONTH]-1;
			RefreshTime();
			EA = 1;
			break;
		
		case ADJUSTDATE:
			innerClock[DAY] = innerClock[DAY]<=1?monthdayAmount[innerClock[MONTH]-1]:innerClock[DAY]-1;
			break;
		
		case ADJUSTWEEKDAY:
			innerClock[WEEKDAY] = innerClock[WEEKDAY]<=0?6:innerClock[WEEKDAY]-1;
			RefreshTime();
			break;
		
		case ADJUSTHOUR:
			innerClock[HOUR] = innerClock[HOUR]<=0?23:innerClock[HOUR]-1;
			RefreshTime();
			break;
		
		case ADJUSTMINUTE:
			innerClock[MINUTE] = innerClock[MINUTE]<=0?59:innerClock[MINUTE]-1;
			RefreshTime();
			break;
		
		case RESETSECOND:
			EA = 0;
			innerClock[SECOND] = 0;
			RefreshTime();
			EA = 1;
			break;
		
		case CHANGELIGHT:
			light = light<=0?2:light-1;
			ChangeColor();
			LCD1602_Refresh(lightModeString1,lightModeString2[light]);
			break;
		
		case SWITCHSOUND:
			hourlyChime = ~hourlyChime;
			LCD1602_Refresh(soundSwitchString1,soundSwitchString2[hourlyChime]);
			break;
		
		case UPDATESTATE:
			break;
		
		default:
			/*do nothing*/
			break;
	}
}

void LeftKeyAction(){
	switch(mode){
		case ADJUSTYEAR:
			mode = RESETSECOND;
			break;
		
		case ADJUSTMONTH:
			mode = ADJUSTYEAR;
			break;
		
		case ADJUSTDATE:
			mode = ADJUSTMONTH;
			break;
		
		case ADJUSTWEEKDAY:
			mode = ADJUSTDATE;
			break;
		
		case ADJUSTHOUR:
			mode = ADJUSTWEEKDAY;
			break;
		
		case ADJUSTMINUTE:
			mode = ADJUSTHOUR;
			break;
		
		case RESETSECOND:
			mode = ADJUSTMINUTE;
			break;
		
		case CHANGELIGHT:
			if(light == ON){
				colorCode = colorCode<=1?7:(colorCode-1);
				ChangeColor();
			}
			break;
		
		default:
			/*do nothing*/
			break;
	}
}

void RightKeyAction(){
	switch(mode){
		case ADJUSTYEAR:
			mode = ADJUSTMONTH;
			break;
		
		case ADJUSTMONTH:
			mode = ADJUSTDATE;
			break;
		
		case ADJUSTDATE:
			mode = ADJUSTWEEKDAY;
			break;
		
		case ADJUSTWEEKDAY:
			mode = ADJUSTHOUR;
			break;
		
		case ADJUSTHOUR:
			mode = ADJUSTMINUTE;
			break;
		
		case ADJUSTMINUTE:
			mode = RESETSECOND;
			break;
		
		case RESETSECOND:
			mode = ADJUSTYEAR;
			break;
		
		case CHANGELIGHT:
			if(light == ON){
				colorCode = colorCode>=7?1:(colorCode+1);
				ChangeColor();
			}
			break;
		
		default:
			/*do nothing*/
			break;
	}
}

void RunTwicePerSecond(){
		if(fipfop){
			innerClock[SECOND]++;
			if(innerClock[SECOND] >= 60){
				innerClock[SECOND] = 0;
				innerClock[MINUTE]++;
			}
			if(innerClock[MINUTE] >= 60){
				innerClock[MINUTE] = 0;
				innerClock[HOUR]++;
			}
			if(innerClock[HOUR] >= 24){
				innerClock[HOUR] = 0;
				innerClock[DAY]++;
				innerClock[WEEKDAY] = (innerClock[WEEKDAY]+1)%7;
			}
			if(innerClock[DAY] > monthdayAmount[innerClock[MONTH]-1]){
				innerClock[DAY] = 1;
				innerClock[MONTH]++;
			}
			if(innerClock[MONTH] > 12){
				innerClock[MONTH] = 1;
				innerClock[YEAR]++;
			}
			monthdayAmount[1] = 
				((innerClock[YEAR]%4==0)&&(innerClock[YEAR]%100!=0))||(innerClock[YEAR]%400==0) ? 29 : 28;
		}
		buzz = hourlyChime&&(innerClock[MINUTE]==0)&&(innerClock[SECOND]==0);
		if(mode >= NORMAL && mode <= RESETSECOND){
			RefreshTime();
		}
}



unsigned char PulseReferee(unsigned char decodeCounter){
	if(decodeCounter <= 17)				return 0xFF;
	else if(decodeCounter <= 46)	return 0x00;
	else if(decodeCounter <= 72)	return 0x01;
	else if(decodeCounter <= 95)	return 0x02;
	else if(decodeCounter <= 120)	return 0x03;
	else													return 0xFE;
}

//void SendByte(char byte){
//	SBUF = byte;
//	while(TI==0);
//	TI=0;
//}

bit FrameDecoder(){
	int cnt;
	for(cnt = 1; cnt < 20; cnt++){
		if(decodeBuffer[(decodePointer+cnt)%20]==0xFF || decodeBuffer[(decodePointer+cnt)%20]==0xFE){
			return 0;
		}
	}
	
	//sets second
	switch(decodeBuffer[(decodePointer+1)%20]){
		case 0x00:
			currentReceivedTime[SECOND] = 1;
			break;
		case 0x01:
			currentReceivedTime[SECOND] = 21;
			break;
		case 0x02:
			currentReceivedTime[SECOND] = 41;
			break;
		default:
			return 0;
	}
	
	//sets minute
	currentReceivedTime[MINUTE] = decodeBuffer[(decodePointer+5)%20]*16+
																decodeBuffer[(decodePointer+6)%20]*4+
																decodeBuffer[(decodePointer+7)%20];
	
	//sets hour
	currentReceivedTime[HOUR] = 	decodeBuffer[(decodePointer+3)%20]*4+
																decodeBuffer[(decodePointer+4)%20]+
																(((decodeBuffer[(decodePointer+10)%20]==0x00)||(decodeBuffer[(decodePointer+10)%20]==0x01))?0:12);
	
	//sets weekday
	currentReceivedTime[WEEKDAY] = 	decodeBuffer[(decodePointer+8)%20]*4+
																	decodeBuffer[(decodePointer+9)%20];
	
	//set day
	currentReceivedTime[DAY] =	decodeBuffer[(decodePointer+11)%20]*16+
															decodeBuffer[(decodePointer+12)%20]*4+
															decodeBuffer[(decodePointer+13)%20];

	//sets month
	currentReceivedTime[MONTH] = 	decodeBuffer[(decodePointer+14)%20]*4+
																decodeBuffer[(decodePointer+15)%20];
																
	//set year
	currentReceivedTime[YEAR] =	decodeBuffer[(decodePointer+16)%20]*16+
															decodeBuffer[(decodePointer+17)%20]*4+
															decodeBuffer[(decodePointer+18)%20]+
															2000;
	
	
	return 1;
}

bit FrameComparator(){
	int i;
	int monthdayAmountTemp;
	if((lastReceivedTime[SECOND]==41)&&(currentReceivedTime[SECOND]==1)){
		lastReceivedTime[MINUTE]++;
		if(lastReceivedTime[MINUTE] >= 60){
			lastReceivedTime[MINUTE] = 0;
			lastReceivedTime[HOUR]++;
		}
		if(lastReceivedTime[HOUR] >= 24){
			lastReceivedTime[HOUR] = 0;
			lastReceivedTime[DAY]++;
			lastReceivedTime[WEEKDAY] = (lastReceivedTime[WEEKDAY]+1)%7;
		}
		if(lastReceivedTime[MONTH]==2){
			monthdayAmountTemp = 
				((lastReceivedTime[YEAR]%4==0)&&(lastReceivedTime[YEAR]%100!=0))||(lastReceivedTime[YEAR]%400==0) ? 29 : 28;
		}
		else{
			monthdayAmountTemp = monthdayAmount[lastReceivedTime[MONTH]-1];
		}
		if(lastReceivedTime[DAY] > monthdayAmountTemp){
			lastReceivedTime[DAY] = 1;
			lastReceivedTime[MONTH]++;
		}
		if(lastReceivedTime[MONTH] > 12){
			lastReceivedTime[MONTH] = 1;
			lastReceivedTime[YEAR]++;
		}
		for(i=1; i<7; i++){
			if(lastReceivedTime[i]!=currentReceivedTime[i]){
				return 0;
			}
		}
		return 1;
	}
	else if(
			((lastReceivedTime[SECOND]==21)&&(currentReceivedTime[SECOND]==41))
		||((lastReceivedTime[SECOND]==1)&&(currentReceivedTime[SECOND]==21))
	){
		for(i=1; i<7; i++){
			if(lastReceivedTime[i]!=currentReceivedTime[i]){
				return 0;
			}
		}
		return 1;
	}
	else{
		return 0;
	}
}
bit Decode(){
	
	SBUF = timeMatchCounter;
	while(TI==0);
	TI = 0;
	if(FrameDecoder()){
		//timeMatchCounter is zero means the values in lastReceivedTime is invalid
		if(timeMatchCounter==0){
			addrExchangeTempVar = currentReceivedTime;
			currentReceivedTime = lastReceivedTime;
			lastReceivedTime = addrExchangeTempVar;
			timeMatchCounter++;
			return 0;
		}
		else{
			if(FrameComparator()){
				if(timeMatchCounter>=4){
					return 1;
				}
				else{
					addrExchangeTempVar = currentReceivedTime;
					currentReceivedTime = lastReceivedTime;
					lastReceivedTime = addrExchangeTempVar;
					timeMatchCounter++;
					return 0;
				}
			}
			else{
				timeMatchCounter = 0;
				return 0;
			}
		}
//		SendByte((currentReceivedTime[YEAR]/1000)+'0');
//		SendByte((currentReceivedTime[YEAR]%1000)/100+'0');
//		SendByte((currentReceivedTime[YEAR]%100)/10+'0');
//		SendByte((currentReceivedTime[YEAR]%10)+'0');
//		
//		SendByte((currentReceivedTime[MONTH]/10)+'0');
//		SendByte((currentReceivedTime[MONTH]%10)+'0');
//		
//		SendByte((currentReceivedTime[DAY]/10)+'0');
//		SendByte((currentReceivedTime[DAY]%10)+'0');
//		
//		SendByte(currentReceivedTime[WEEKDAY]+'0');
//		
//		SendByte((currentReceivedTime[HOUR]/10)+'0');
//		SendByte((currentReceivedTime[HOUR]%10)+'0');
//		
//		SendByte((currentReceivedTime[MINUTE]/10)+'0');
//		SendByte((currentReceivedTime[MINUTE]%10)+'0');
//		
//		SendByte((currentReceivedTime[SECOND]/10)+'0');
//		SendByte((currentReceivedTime[SECOND]%10)+'0');
	}
	else{
		timeMatchCounter = 0;
		return 0;
	}
}

void Initialize(){
	ChangeColor();
	buzz = 0;
	antennaIsOff = 0;
	
	LCD1602_Init();
	
	SCON=0x50;
	PCON=0x00;
	TMOD=0x20;
	TCON|=0x05;
	TL1=0xFD;
	TH1=0xFD;
  TR1=1;
	
	IP = 0x01;
	
	IT1 = 1;
	EX1 = 1;
	
	IT0 = 1;
	EX0 = 1;
	
	EA = 1;
}

void main(){
	Initialize();
	while(1){
		if(middle == 0){
			
			MiddleKeyAction();
			
			delay(DELAYXMS);
			while(middle == 0)
			delay(DELAYXMS);
		}
		else if(up == 0){
			
			UpKeyAction();
			
			delay(DELAYXMS);
			while(up == 0)
			delay(DELAYXMS);
		}
		else if(down == 0){
			
			DownKeyAction();
			
			delay(DELAYXMS);
			while(down == 0)
			delay(DELAYXMS);
		}
		else if(left == 0){
			
			LeftKeyAction();
			
			delay(DELAYXMS);
			while(left == 0)
			delay(DELAYXMS);
		}
		else if(right == 0){
			
			RightKeyAction();
			
			delay(DELAYXMS);
			while(right == 0)
			delay(DELAYXMS);
		}
		else{
			/*do nothing*/
		}
	
		if(refreshTimeFlag){
			RunTwicePerSecond();
			refreshTimeFlag = 0;
		}
	};
}

void EX0_IRQ() interrupt 0{
	if(syncCounter >= 255 && syncCounter <= 257){
		freqMatchCounter++;
	}
	else{
		freqMatchCounter = 0;
	}
	if(freqMatchCounter >= 3){
		
		//synchronize the inner clock with the radio signal
		mainCounter = 0;
		fipfop = 1;
		
		//shut the synchronization mechanism down
		freqMatchCounter = 0;
		EX0 = 0;
		
		//frequency synchronized
		phaseSynchronized = 1;
	}
	
	//reset the counter
	syncCounter = 0;
}

void EX1_IRQ() interrupt 2{
	mainCounter++;
//	test = fipfop;
	if(phaseSynchronized){
		if(!timeSynchronized && signal==0 && mainCounter<=128){decodeCounter++;}
	}
	else{
		syncCounter++;
	}
	
	if(mainCounter >= 128){
		mainCounter = 0;
		fipfop = !fipfop;
		refreshTimeFlag = 1;
		
		//Time is not synchronized
		//Frequency is synchronized
		//A new second has just started
		if(!timeSynchronized && phaseSynchronized && fipfop){
			
			decodeBuffer[decodePointer] = PulseReferee(decodeCounter);
			
			//0xFF is considerred as the end of the last frame
			if((decodeBuffer[decodePointer]==0xFF)&&Decode()&&UpdateTime()){				
				antennaIsOff = 1;
				timeSynchronized = 1;
				decodePointer = 0;
				timeMatchCounter = 0;
			}
			else{
				decodePointer = (decodePointer+1)%20;
			}
//			SBUF = (decodeCounter/100)+'0';
//			while(TI==0);
//			TI=0;
//			
//			SBUF = ((decodeCounter%100)/10)+'0';
//			while(TI==0);
//			TI=0;
//			
//			SBUF = (decodeCounter%10)+'0';
//			while(TI==0);
//			TI=0;
//			
//			SBUF = ' ';
//			while(TI==0);
//			TI=0;
			
			decodeCounter = 0;
		}
	}
}
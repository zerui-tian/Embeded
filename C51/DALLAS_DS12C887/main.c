#include<reg52.h>
#include<LCD1602.h>

#define FRAME_LENGTH 23
#define BUZZERRING (time[1]==0&&time[0]==0)
#define DOTLIGHTUP (interruptCounter >= 256)

xdata char chSecondsChannel 	_at_ 0x7F00;
xdata char chMinutesChannel 	_at_ 0x7F02;
xdata char chHoursChannel 		_at_ 0x7F04;
xdata char chDofWChannel 			_at_ 0x7F06;
xdata char chDateChannel 			_at_ 0x7F07;
xdata char chMonthChannel		 	_at_ 0x7F08;
xdata char chYearChannel 			_at_ 0x7F09;
xdata char chRegA 						_at_ 0x7F0A;
xdata char chRegB							_at_ 0x7F0B;
xdata char chRegC 						_at_ 0x7F0C;
xdata char chRegD 						_at_ 0x7F0D;
xdata char chCenturyChannel 	_at_ 0x7F32;

char buffer[30];//1970-01-01 THU 00:00:00
char byteBuffer;
char pBuffer = 0;
int secondCounter = 0;

xdata char weekDaysIndex[7] = {0xF6,0xEA,0xEE,0xE0,0xF1,0xE1,0xE8};

char time[8];

xdata char Line1[] =    " ****-**-** *** ";
xdata char Line2[2][17] = 
{
	" LOCAL time **s ",
	" NTP time   **s "
};

int interruptCounter = 0;
char lastSecond = 0xFF;

bit buzzerRing = 0;
bit isWIFIOn = 0;
bit hasUpdated = 0;

void GetTime(){
	time[0] = chSecondsChannel;
	time[1] = chMinutesChannel;
	time[2] = chHoursChannel;
	time[3] = chDofWChannel-1;
	time[4] = chDateChannel;
	time[5] = chMonthChannel;
	time[6] = chYearChannel;
	time[7] = chCenturyChannel;
}

void SetTime(){
	char regB;
	
	regB = chRegB;
	chRegB = regB|0x80;
	
	chSecondsChannel = time[0];
	chMinutesChannel = time[1];
	chHoursChannel = time[2];
	chDofWChannel = time[3]+1;
	chDateChannel = time[4];
	chMonthChannel = time[5];
	chYearChannel = time[6];
	chCenturyChannel = time[7];
	
	chRegB = regB;
}

void Frame_Analyzer(){
	int i;
	char weekDayCharSum;
	if(pBuffer == FRAME_LENGTH){
		
		time[0] = ((buffer[21]-'0')<<4)|(buffer[22]-'0');
		time[1] = ((buffer[18]-'0')<<4)|(buffer[19]-'0');
		time[2] = ((buffer[15]-'0')<<4)|(buffer[16]-'0');
		
		weekDayCharSum = buffer[13]+buffer[12]+buffer[11];
		for(i = 0; i < 7; i++){
			if(weekDayCharSum == weekDaysIndex[i]){
				break;
			}
		}
		time[3] = i;
		
		time[4] = ((buffer[8]-'0')<<4)|(buffer[9]-'0');
		time[5] = ((buffer[5]-'0')<<4)|(buffer[6]-'0');
		time[6] = ((buffer[2]-'0')<<4)|(buffer[3]-'0');
		time[7] = ((buffer[0]-'0')<<4)|(buffer[1]-'0');
		
		EA = 0;
		SetTime();
		EA = 1;
	}
}

char Nixie_Action(bit dotFlag, char digit, char number){
	char ret_val = 0x00;
	
	ret_val = BUZZERRING ? ret_val|0x80 : ret_val;
	ret_val = dotFlag ? ret_val : ret_val|0x40;
	ret_val = ret_val|(digit<<4);
	ret_val = ret_val|number;
	
	return ret_val;
}

void Refresh(){
	Line1[1] = ((time[7] >> 4) & 0x0F)  + '0';
	Line1[2] = (time[7] & 0x0F ) + '0';
	Line1[3] = ((time[6] >> 4) & 0x0F) + '0';
	Line1[4] = (time[6] & 0x0F) + '0';
	
	Line1[6] = ((time[5] >> 4) & 0x0F) + '0';
	Line1[7] = (time[5] & 0x0F) + '0';
	
	Line1[9] = ((time[4] >> 4) & 0x0F) + '0';
	Line1[10] = (time[4] & 0x0F) + '0';
	
	switch(time[3]){
		case 0x00:
			Line1[12] = 'S';
			Line1[13] = 'U';
			Line1[14] = 'N';
			break;
		case 0x01:
			Line1[12] = 'M';
			Line1[13] = 'O';
			Line1[14] = 'N';
			break;
		case 0x02:
			Line1[12] = 'T';
			Line1[13] = 'U';
			Line1[14] = 'E';
			break;
		case 0x03:
			Line1[12] = 'W'; 
			Line1[13] = 'E';
			Line1[14] = 'D';
			break;
		case 0x04:
			Line1[12] = 'T';
			Line1[13] = 'H';
			Line1[14] = 'U';
			break;
		case 0x05:
			Line1[12] = 'F';
			Line1[13] = 'R';
			Line1[14] = 'I';
			break;
		case 0x06:
			Line1[12] = 'S';
			Line1[13] = 'A';
			Line1[14] = 'T';
			break;
		default:
			/* do nothing */
			break;
	}
	
	Line2[isWIFIOn][12] = time[0]>10 ? ((time[0]>>4)&0x0F)+'0' : ' ';
	Line2[isWIFIOn][13] = (time[0] & 0x0F) + '0';

	LCD1602_Refresh(Line1, Line2[isWIFIOn]);
}

void Initialize(){
	LCD1602_Init();
	
	chRegA = 0x27; //UIP DV2 DV1 DV0 RS3 RS2 RS1 RS0
	chRegB = 0x0A; // SET PIE AIE UIE SQWE DM 24/12 DSE
	
	SCON=0x50;
	PCON=0x00;

	TMOD=0x20;
	TL1=0xFA;
	TH1=0xFA;
  TR1=1;
	
	IT0=1;

	EX0 = 1;
	
	EA = 1;
}

void main(){
	Initialize();
	while(1){
		if(RI){
			RI = 0;
			byteBuffer = SBUF;
			if(byteBuffer == '.'){
				isWIFIOn = 0;
			}
			else if(byteBuffer == '^'){
				pBuffer = 0;
			}
			else if(byteBuffer == '$'){
				isWIFIOn = 1;
				if(!hasUpdated){
					hasUpdated = 1;
					Frame_Analyzer();
				}
			}
			else{
				buffer[pBuffer++] = byteBuffer;
			}
		}
		if(secondCounter>3600){
			secondCounter = 0;
			hasUpdated = 0;
		}
	}
}


void EX0_IRQ() interrupt 0{
	
	GetTime();
	
	switch(interruptCounter%4){
		case 0:
			P2 = Nixie_Action(0, 0, time[1]&0x0F);
			break;
		case 1:
			P2 = Nixie_Action(0, 1, (time[1]>>4)&0x0F);
			break;
		case 2:
			P2 = Nixie_Action(DOTLIGHTUP, 2, time[2]&0x0F);
			break;
		case 3:
			P2 = Nixie_Action(0, 3, (time[2]>>4)&0x0F);
			break;
		default:
			break;
	}
	
	if(lastSecond != time[0]){
		interruptCounter = 0;
		secondCounter++;
		lastSecond = time[0];
		Refresh();
	}
	else{
		interruptCounter++;
	}
}
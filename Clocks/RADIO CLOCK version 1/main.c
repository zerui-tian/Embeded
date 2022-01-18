#include <reg52.h>
#include <intrins.h>

#define TRUE 	1
#define FALSE 0

#define HIGH	1
#define LOW		0

#define SCAN_PERIOD_H 0xF9
#define SCAN_PERIOD_L 0x00
#define FRAME_LENGTH_PC 16
#define FRAME_LENGTH_BPC 12

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
xdata char time_DS12C887[8];
xdata char time_BPC[8];

#define DISPLAY_MINUTEANDHOUR 0
#define DISPLAY_SECOND				1
#define DISPLAY_DATEANDMONTH	2
#define DISPLAY_WEEKDAY				3
#define DISPLAY_YEAR					4
#define OUTPUT_SERIAL					5
char state = DISPLAY_MINUTEANDHOUR;

#define COUNTDOWN_MATRIXLIGHTUP 11
#define COUNTDOWN_RESETBITSRECIEVER_BPC 5
char countDown_ResetBitsReciever_BPC = COUNTDOWN_RESETBITSRECIEVER_BPC;
char countDown_MatrixLightUp = COUNTDOWN_MATRIXLIGHTUP;

//signals
sbit RCLK_595 	= P1^0;
sbit SER_595 		= P1^1;
sbit SRCLK_595 	= P1^2;
sbit OE_595 		= P1^3;
sbit BUTTON 		= P1^4;
sbit DATA_BPC 	= P3^5;
sbit RESET_BPC 	= P3^4;
sbit PON_BPC		= P1^7;

//addresses of variables in ds12c887
xdata char chSecondsChannel 	_at_ 0x8000;
xdata char chMinutesChannel 	_at_ 0x8002;
xdata char chHoursChannel 		_at_ 0x8004;
xdata char chDofWChannel 			_at_ 0x8006;
xdata char chDateChannel 			_at_ 0x8007;
xdata char chMonthChannel		 	_at_ 0x8008;
xdata char chYearChannel 			_at_ 0x8009;
xdata char chRegA 						_at_ 0x800A;
xdata char chRegB							_at_ 0x800B;
xdata char chRegC 						_at_ 0x800C;
xdata char chRegD 						_at_ 0x800D;
xdata char chCenturyChannel 	_at_ 0x8032;

//counters
char interruptCounter_MatrixScanner = 0;
char interruptCounter_BitsReciever_BPC = 0;
char interruptCounter_BytesReciever_PC = 0;

//video buffer
int vbuf[8][3];
char digits[4] = {0,0,0,0};

//recieve buffers
char buffer_BPC[FRAME_LENGTH_BPC];//{0x02,0x01,0x04,0x0C,0x03,0x0E,0x0A,0x05,0x05,0x00,0x00,0x00};
char buffer_serial[FRAME_LENGTH_PC];

//flags
bit colonLightUp = 		FALSE;
bit matrixLightUp = 	FALSE;
bit resetBPC = 				TRUE;
bit recentlySucceed = FALSE;
bit recentlyRecieve = FALSE;

int code rowSelectCode[8][3] = {
	{0x3434,0x34EB,0xEBEB},
	{0x3434,0x346F,0x6F6F},
	{0x3535,0x356B,0x6B6B},
	{0x3434,0x347B,0x7B7B},
	{0xB4B4,0xB46B,0x6B6B},
	{0x3636,0x366B,0x6B6B},
	{0x7474,0x746B,0x6B6B},
	{0x3C3C,0x3C6B,0x6B6B}
};
int code digit3Code[3][8][3] = {
	// 0x:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFBFF,0xFFFF,0xFFDD},//____***_________________
		{0xFBFF,0xFFFF,0xFFFD},//____*_*_________________
		{0xFBFF,0xFFFF,0xFFFD},//____*_*_________________
		{0xFBFF,0xFFFF,0xFFFD},//____*_*_________________
		{0xFBFF,0xFFFF,0xFFDD},//____***_________________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// 1x:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
		{0xFBFF,0xFFFF,0xFFDF},//____**__________________
		{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
		{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
		{0xFBFF,0xFFFF,0xFFDD},//____***_________________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// 2x:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFBFF,0xFFFF,0xFFDD},//____***_________________
		{0xFFFF,0xFFFF,0xFFFD},//______*_________________
		{0xFBFF,0xFFFF,0xFFDD},//____***_________________
		{0xFBFF,0xFFFF,0xFFFF},//____*___________________
		{0xFBFF,0xFFFF,0xFFDD},//____***_________________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	}
};

int code digit2Code[10][8][3] = {
	// x0:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xF7FF},//________*_*_____________
		{0xFFEF,0xFFFF,0xF7FF},//________*_*_____________
		{0xFFEF,0xFFFF,0xF7FF},//________*_*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// x1:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFDF,0xFFFF,0xFFFF},//_________*______________
		{0xFFDF,0xFFFF,0xF7FF},//________**______________
		{0xFFDF,0xFFFF,0xFFFF},//_________*______________
		{0xFFDF,0xFFFF,0xFFFF},//_________*______________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// x2:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xF7FF},//________*_______________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// x3:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// x4:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFEF,0xFFFF,0xF7FF},//________*_*_____________
		{0xFFEF,0xFFFF,0xF7FF},//________*_*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// x5:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xF7FF},//________*_______________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// x6:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xF7FF},//________*_______________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xF7FF},//________*_*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// x7:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// x8:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xF7FF},//________*_*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xF7FF},//________*_*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// x9:xx
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xF7FF},//________*_*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFEF,0xFFFF,0xFFFF},//__________*_____________
		{0xFFCF,0xFFFF,0xF7FF},//________***_____________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	}
};




int code digit1Code[6][8][3] = {
	// xx:0x
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFF7,0xFDFF},//______________*_*_______
		{0xFFFF,0xFFF7,0xFDFF},//______________*_*_______
		{0xFFFF,0xFFF7,0xFDFF},//______________*_*_______
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:1x
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFEFF},//_______________*________
		{0xFFFF,0xFFFF,0xFCFF},//______________**________
		{0xFFFF,0xFFFF,0xFEFF},//_______________*________
		{0xFFFF,0xFFFF,0xFEFF},//_______________*________
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:2x
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFF7,0xFFFF},//________________*_______
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFFF,0xFDFF},//______________*_________
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:3x
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFF7,0xFFFF},//________________*_______
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFF7,0xFFFF},//________________*_______
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:4x
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFF7,0xFDFF},//______________*_*_______
		{0xFFFF,0xFFF7,0xFDFF},//______________*_*_______
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFF7,0xFFFF},//________________*_______
		{0xFFFF,0xFFF7,0xFFFF},//________________*_______
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:5x
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFFF,0xFDFF},//______________*_________
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFF7,0xFFFF},//________________*_______
		{0xFFFF,0xFFF7,0xFCFF},//______________***_______
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	}
};



int code digit0Code[10][8][3] = {
	// xx:x0
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xEBFF,0xFFFF},//__________________*_*___
		{0xFFFF,0xEBFF,0xFFFF},//__________________*_*___
		{0xFFFF,0xEBFF,0xFFFF},//__________________*_*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:x1
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFBF,0xFFFF},//___________________*____
		{0xFFFF,0xEFBF,0xFFFF},//__________________**____
		{0xFFFF,0xFFBF,0xFFFF},//___________________*____
		{0xFFFF,0xFFBF,0xFFFF},//___________________*____
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:x2
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xEFFF,0xFFFF},//__________________*_____
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:x3
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:x4
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xEBFF,0xFFFF},//__________________*_*___
		{0xFFFF,0xEBFF,0xFFFF},//__________________*_*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:x5
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xEFFF,0xFFFF},//__________________*_____
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:x6
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xEFFF,0xFFFF},//__________________*_____
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xEBFF,0xFFFF},//__________________*_*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:x7
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:x8
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xEBFF,0xFFFF},//__________________*_*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xEBFF,0xFFFF},//__________________*_*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// xx:x9
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xEBFF,0xFFFF},//__________________*_*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFBFF,0xFFFF},//____________________*___
		{0xFFFF,0xEBBF,0xFFFF},//__________________***___
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	}
};

int code colonCode[2][8][3] = {
	// extinguish
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	},
	// light up
	{
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFB,0xFFFF,0xFFFF},//____________*___________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFB,0xFFFF,0xFFFF},//____________*___________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF} //________________________
	}
};


int code dotCode [8][3] = {
	{0xFFFF,0xFFFF,0xFFFF},//________________________
	{0xFFFF,0xFFFF,0xFFFF},//________________________
	{0xFFFF,0xFFFF,0xFFFF},//________________________
	{0xFFFF,0xFFFF,0xFFFF},//________________________
	{0xFFFF,0xFFFF,0xFFFF},//________________________
	{0xFFFF,0xFFFF,0xFFFF},//________________________
	{0xFFFB,0xFFFF,0xFFFF},//____________*___________
	{0xFFFF,0xFFFF,0xFFFF} //________________________
};
int code weekdayCode[7][3][8][3] = {
	{
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFBFF,0xFFFF,0xFFDD},//____***_________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFFFF,0xFFFF,0xFFBF},//___*____________________
			{0xFBFF,0xFFFF,0xFFDD},//____***_________________
			{0xFFFF,0xFFFF,0xFFFE},//_______*________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFBFF,0xFFFF,0xFFDD} //____***_________________
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDB,0xFFFF,0xDFFF},//_________*__**__________
			{0xFFEF,0xFFFF,0x9FFF} //__________**_*__________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xCFFF,0xFEFF},//_______________*_**_____
			{0xFFFF,0xFFB7,0xFEFF},//_______________**__*____
			{0xFFFF,0xFFBF,0xFEFF},//_______________*___*____
			{0xFFFF,0xFFBF,0xFEFF},//_______________*___*____
			{0xFFFF,0xFF9F,0xFEFF} //_______________*___*_*__
		}
	},
	{
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFBFF,0xFFFF,0xFFBC},//___**_**________________
			{0xFFFF,0xFFFF,0xFF9E},//___*_*_*________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFFFF,0xFFFF,0xFFBE} //___*___*________________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFEB,0xFFFF,0xBFFF},//__________***___________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFEB,0xFFFF,0xBFFF} //__________***___________
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xCFFF,0xFEFF},//_______________*_**_____
			{0xFFFF,0xFFB7,0xFEFF},//_______________**__*____
			{0xFFFF,0xFFBF,0xFEFF},//_______________*___*____
			{0xFFFF,0xFFBF,0xFEFF},//_______________*___*____
			{0xFFFF,0xFF9F,0xFEFF} //_______________*___*_*__
		}
	},
	{
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFBFF,0xFFFF,0xFF9C},//___*****________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF} //_____*__________________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDB,0xFFFF,0xDFFF},//_________*__**__________
			{0xFFEF,0xFFFF,0x9FFF} //__________**_*__________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//_______________________
			{0xFFFF,0xFFFF,0xFFFF},//_______________________
			{0xFFFF,0xFFFF,0xFFFF},//_______________________
			{0xFFFF,0xCFF7,0xFFFF},//________________***____
			{0xFFFF,0xFFBF,0xFEFF},//_______________*___*___
			{0xFFFF,0xCFB7,0xFEFF},//_______________*****___
			{0xFFFF,0xFFFF,0xFEFF},//_______________*_______
			{0xFFFF,0xCFD7,0xFFFF},//________________***__*_
	
		}
	},
	{
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFFFF,0xFFFF,0xFF9E},//___*_*_*________________
			{0xFFFF,0xFFFF,0xFF9E},//___*_*_*________________
			{0xFFFF,0xFFFF,0xFF9E},//___*_*_*________________
			{0xFFFF,0xFFFF,0xFF9E},//___*_*_*________________
			{0xFBFF,0xFFFF,0xFFFD} //____*_*_________________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFEB,0xFFFF,0xBFFF},//__________***___________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFCB,0xFFFF,0x9FFF},//_________*****__________
			{0xFFDF,0xFFFF,0xFFFF},//_________*______________
			{0xFFEB,0xFFFF,0xBFFF} //__________***___________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//_______________________
			{0xFFFF,0xFFBF,0xFFFF},//__________________*____
			{0xFFFF,0xFFBF,0xFFFF},//__________________*____
			{0xFFFF,0xDFB7,0xFFFF},//_______________**_*____
			{0xFFFF,0xEFBF,0xFEFF},//______________*__**____
			{0xFFFF,0xFFBF,0xFEFF},//______________*___*____
			{0xFFFF,0xEFBF,0xFEFF},//______________*__**____
			{0xFFFF,0xDF97,0xFFFF} //_______________**_*_*__
	
		}
	},
	{
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFBFF,0xFFFF,0xFF9C},//___*****________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF},//_____*__________________
			{0xFFFF,0xFFFF,0xFFDF} //_____*__________________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFDF,0xFFFF,0xFFFF},//_________*______________
			{0xFFDF,0xFFFF,0xFFFF},//_________*______________
			{0xFFDB,0xFFFF,0xBFFF},//_________*_**___________
			{0xFFCF,0xFFFF,0xDFFF},//_________**__*__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//_______________________
			{0xFFFF,0xFFFF,0xFFFF},//_______________________
			{0xFFFF,0xFFFF,0xFFFF},//_______________________
			{0xFFFF,0xFFBF,0xFEFF},//______________*___*____
			{0xFFFF,0xFFBF,0xFEFF},//______________*___*____
			{0xFFFF,0xFFBF,0xFEFF},//______________*___*____
			{0xFFFF,0xEFBF,0xFEFF},//______________*__**____
			{0xFFFF,0xDF97,0xFFFF} //_______________**_*_*__
	
		}
	},
	{
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFBFF,0xFFFF,0xFF9C},//___*****________________
			{0xFFFF,0xFFFF,0xFFBF},//___*____________________
			{0xFFFF,0xFFFF,0xFFBF},//___*____________________
			{0xFBFF,0xFFFF,0xFF9D},//___****_________________
			{0xFFFF,0xFFFF,0xFFBF},//___*____________________
			{0xFFFF,0xFFFF,0xFFBF},//___*____________________
			{0xFFFF,0xFFFF,0xFFBF} //___*____________________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFDB,0xFFFF,0xBFFF},//_________*_**___________
			{0xFFCF,0xFFFF,0xDFFF},//_________**__*__________
			{0xFFDF,0xFFFF,0xFFFF},//_________*______________
			{0xFFDF,0xFFFF,0xFFFF},//_________*______________
			{0xFFDF,0xFFFF,0xFFFF} //_________*______________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xDFFF,0xFFFF},//_________________*______
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xDFF7,0xFFFF},//________________**______
			{0xFFFF,0xDFFF,0xFFFF},//_________________*______
			{0xFFFF,0xDFFF,0xFFFF},//_________________*______
			{0xFFFF,0xDFFF,0xFFFF},//_________________*______
			{0xFFFF,0xCFD7,0xFFFF} //________________***__*_
	
		}
	},
	{
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFBFF,0xFFFF,0xFFDD},//____***_________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFFFF,0xFFFF,0xFFBF},//___*____________________
			{0xFBFF,0xFFFF,0xFFDD},//____***_________________
			{0xFFFF,0xFFFF,0xFFFE},//_______*________________
			{0xFFFF,0xFFFF,0xFFBE},//___*___*________________
			{0xFBFF,0xFFFF,0xFFDD} //____***_________________
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFEB,0xFFFF,0xBFFF},//__________***___________
			{0xFFFF,0xFFFF,0xDFFF},//_____________*__________
			{0xFFEB,0xFFFF,0x9FFF},//__________****__________
			{0xFFDF,0xFFFF,0xDFFF},//_________*___*__________
			{0xFFEB,0xFFFF,0x9FFF} //__________****__________
	
		},
		{
			{0xFFFF,0xFFFF,0xFFFF},//________________________
			{0xFFFF,0xFFF7,0xFFFF},//________________*_______
			{0xFFFF,0xFFF7,0xFFFF},//________________*_______
			{0xFFFF,0xDFF7,0xFEFF},//_______________***______
			{0xFFFF,0xFFF7,0xFFFF},//________________*_______
			{0xFFFF,0xFFF7,0xFFFF},//________________*_______
			{0xFFFF,0xFFB7,0xFFFF},//________________*__*____
			{0xFFFF,0xCFDF,0xFFFF},//_________________**__*_
	
		}
	}
};



int code uartCode[8][3] = {
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xFFFF,0xFFFF,0xFFFF},//________________________
		{0xEFDB,0xCB9F,0xD5DF},//__*__*__**__***__*****__
		{0xEFEB,0xFFBF,0xFEDE},//__*__*_*__*_*__*___*____
		{0xEFEB,0xFFBF,0xDDDE},//__*__*_*__*_***____*____
		{0xEFCB,0xFFBF,0xF5DE},//__*__*_****_*_*____*____
		{0xFBEB,0xFFBF,0xFEBE},//___**__*__*_*__*___*____
		{0xFFFF,0xFFFF,0xFFFF} //________________________
};

//int code columnSelectCode[24][3] = {
//	{0xFFFF,0xFFFF,0xFFF7},
//	{0xDFFF,0xFFFF,0xFFFF},
//	{0xEFFF,0xFFFF,0xFFFF},
//	{0xFFFF,0xFFFF,0xFFBF},
//	{0xFBFF,0xFFFF,0xFFFF},
//	{0xFFFF,0xFFFF,0xFFDF},
//	{0xFFFF,0xFFFF,0xFFFD},
//	{0xFFFF,0xFFFF,0xFFFE},
//	
//	{0xFFFF,0xFFFF,0xF7FF},
//	{0xFFDF,0xFFFF,0xFFFF},
//	{0xFFEF,0xFFFF,0xFFFF},
//	{0xFFFF,0xFFFF,0xBFFF},
//	{0xFFFB,0xFFFF,0xFFFF},
//	{0xFFFF,0xFFFF,0xDFFF},
//	{0xFFFF,0xFFFF,0xFDFF},
//	{0xFFFF,0xFFFF,0xFEFF},
//	
//	{0xFFFF,0xFFF7,0xFFFF},
//	{0xFFFF,0xDFFF,0xFFFF},
//	{0xFFFF,0xEFFF,0xFFFF},
//	{0xFFFF,0xFFBF,0xFFFF},
//	{0xFFFF,0xFBFF,0xFFFF},
//	{0xFFFF,0xFFDF,0xFFFF},
//	{0xFFFF,0xFFFD,0xFFFF},
//	{0xFFFF,0xFFFE,0xFFFF}
//};
/************************************************
 *             Functions of DS12C887            *
 ************************************************/
void GetTime_DS12C887(){
	time_DS12C887[SECOND] = chSecondsChannel;
	time_DS12C887[MINUTE] = chMinutesChannel;
	time_DS12C887[HOUR] = chHoursChannel;
	time_DS12C887[WEEKDAY] = chDofWChannel-1;
	time_DS12C887[DATE] = chDateChannel;
	time_DS12C887[MONTH] = chMonthChannel;
	time_DS12C887[YEAR] = chYearChannel;
	time_DS12C887[CENTURY] = chCenturyChannel;
}
void SetTime_DS12C887(char* time){
	char regB;
	regB = chRegB;
	chRegB = regB|0x80;
	
	chSecondsChannel = 	time[SECOND];
	chMinutesChannel = 	time[MINUTE];
	chHoursChannel = 		time[HOUR];
	chDofWChannel = 		time[WEEKDAY]+1;
		//((time[6] + (time[6]>>2) + (time[7]>>2) - 2*time[7] + (26*(time[5]+1)/10) + time[4] - 1) % 7) + 1;
	chDateChannel = 		time[DATE];
	chMonthChannel = 		time[MONTH];
	chYearChannel =			time[YEAR];
	chCenturyChannel = 	time[CENTURY];
	
	chRegB = regB;
}

/************************************************
 *             Functions of 74HC595             *
 ************************************************/
void Load_595(int dat_H, int dat_M, int dat_L){
	char i;
	for(i=0;i<16;i++){
		SER_595=dat_H&0x8000;
		dat_H=dat_H<<1;
		SRCLK_595=HIGH;
		SRCLK_595=LOW;
	}
	for(i=0;i<16;i++){
		SER_595=dat_M&0x8000;
		dat_M=dat_M<<1;
		SRCLK_595=HIGH;
		SRCLK_595=LOW;
	}
	for(i=0;i<16;i++){
		SER_595=dat_L&0x8000;
		dat_L=dat_L<<1;
		SRCLK_595=HIGH;
		SRCLK_595=LOW;
	}
}
void Output_595(){
	RCLK_595=HIGH;
	RCLK_595=LOW;
}



/************************************************
 *               Command Analyzer               *
 ************************************************/
void Command_Analyzer(void){
	switch(buffer_serial[0]){
		case 0x01:
			SetTime_DS12C887(buffer_serial+1);
			break;
		default:
			break;
	}
}



/************************************************
 *                Matrix Refresh                *
 ************************************************/
void Refresh(void){
	char i,j,k;
	for(i=0;i<8;i++){
		for(j=0;j<3;j++){
			switch(state){
				case DISPLAY_MINUTEANDHOUR:{
					digits[3] = time_DS12C887[HOUR]>>4;
					digits[2] = time_DS12C887[HOUR]&0x0F;
					digits[1] = time_DS12C887[MINUTE]>>4;
					digits[0] = time_DS12C887[MINUTE]&0x0F;
					ET0 = FALSE;
					vbuf[i][j] = digit3Code[digits[3]][i][j] &
											 digit2Code[digits[2]][i][j] &
											 colonCode[colonLightUp][i][j] &
											 digit1Code[digits[1]][i][j] &
											 digit0Code[digits[0]][i][j];
					vbuf[i][j] &= rowSelectCode[i][j];
					ET0 = TRUE;
					break;
				}
				case DISPLAY_SECOND:{
					digits[1] = time_DS12C887[SECOND]>>4;
					digits[0] = time_DS12C887[SECOND]&0x0F;
					ET0 = FALSE;
					vbuf[i][j] = colonCode[colonLightUp][i][j] &
											 digit1Code[digits[1]][i][j] &
											 digit0Code[digits[0]][i][j];
					vbuf[i][j] &= rowSelectCode[i][j];
					ET0 = TRUE;
					break;
				}
				case DISPLAY_DATEANDMONTH:{
					digits[3] = time_DS12C887[MONTH]>>4;
					digits[2] = time_DS12C887[MONTH]&0x0F;
					digits[1] = time_DS12C887[DATE]>>4;
					digits[0] = time_DS12C887[DATE]&0x0F;
					ET0 = FALSE;
					vbuf[i][j] = digit3Code[digits[3]][i][j] &
											 digit2Code[digits[2]][i][j] &
											 dotCode[i][j] &
											 digit1Code[digits[1]][i][j] &
											 digit0Code[digits[0]][i][j];
					vbuf[i][j] &= rowSelectCode[i][j];
					ET0 = TRUE;
					break;
				}
				case DISPLAY_WEEKDAY:{
					ET0 = FALSE;
					vbuf[i][j] = weekdayCode[time_DS12C887[WEEKDAY]][0][i][j] &
											 weekdayCode[time_DS12C887[WEEKDAY]][1][i][j] &
											 weekdayCode[time_DS12C887[WEEKDAY]][2][i][j];
					vbuf[i][j] &= rowSelectCode[i][j];
					ET0 = TRUE;
					break;
				}
				case DISPLAY_YEAR:{
					digits[3] = time_DS12C887[CENTURY]>>4;
					digits[2] = time_DS12C887[CENTURY]&0x0F;
					digits[1] = time_DS12C887[YEAR]>>4;
					digits[0] = time_DS12C887[YEAR]&0x0F;
					ET0 = FALSE;
					vbuf[i][j] = digit3Code[digits[3]][i][j] &
											 digit2Code[digits[2]][i][j] &
											 digit1Code[digits[1]][i][j] &
											 digit0Code[digits[0]][i][j];
					vbuf[i][j] &= rowSelectCode[i][j];
					ET0 = TRUE;
					break;
				}
				case OUTPUT_SERIAL:{
					if(i==0&&j==0){
						for(k=0;k<8;k++){
							SBUF=time_DS12C887[k];
							while(!TI);
							TI=FALSE;
						}
					}
					ET0=FALSE;
					vbuf[i][j] = uartCode[i][j]&rowSelectCode[i][j];
					ET0=TRUE;
					break;
				}
				default:
					break;
			}
		}
	}
}

/************************************************
 *                 Initializer                  *
 ************************************************/
void initializer(){
	//timers
	TMOD=0x21;
	TH0=SCAN_PERIOD_H;
	TL0=SCAN_PERIOD_L;
	TL1=0xFA;
	TH1=0xFA;//baudrate 9600
	
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
	IP = 0x12;
	
	//initialize DS12C887
	chRegA = 0x2F; //	UIP	DV2	DV1	DV0	RS3 	RS2	RS1		RS0
	chRegB = 0x0A; // SET	PIE	AIE	UIE	SQWE	DM	24/12	DSE
	
	//initialize 595
	OE_595=LOW;
	
	//reset BPC reciever
	RESET_BPC=HIGH;

	//eable IRQs
	ET0=TRUE;
	EX0=TRUE;
	EX1=TRUE;
	ES=TRUE;
	
	//eable interruption
	EA=TRUE;
}

void main(){
	initializer();
	while(TRUE){
		if(BUTTON==LOW){
			if(matrixLightUp){
				state = (state+1)%6;
				EX0=FALSE;
				Refresh();
				EX0=TRUE;
			}
			countDown_MatrixLightUp = COUNTDOWN_MATRIXLIGHTUP;
			while(BUTTON==LOW){
			}
		}
	}
}

void EX0_IRQ(void) interrupt 0{
	GetTime_DS12C887();
	colonLightUp = ~colonLightUp;
	matrixLightUp = (countDown_MatrixLightUp>0)|(PON_BPC==LOW&recentlySucceed);
	Refresh();
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
		interruptCounter_BytesReciever_PC = 0;
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
	if(countDown_MatrixLightUp>0){
		countDown_MatrixLightUp--;
	}
}
void Timer0_IRQ(void) interrupt 1{
	if(matrixLightUp){
		OE_595 = LOW;
		Load_595(
			vbuf[interruptCounter_MatrixScanner][0],
			vbuf[interruptCounter_MatrixScanner][1],
			vbuf[interruptCounter_MatrixScanner][2]
		);
		Output_595();
		
		interruptCounter_MatrixScanner = (interruptCounter_MatrixScanner+1)%8;
	}
	else{
		OE_595 = HIGH;
	}
	TH0=SCAN_PERIOD_H;
	TL0=SCAN_PERIOD_L;
}
void EX1_IRQ(void) interrupt 2{
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
			EX0=FALSE;
			
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
			
			SetTime_DS12C887(time_BPC);
			
			EX0=TRUE;
		}
		resetBPC = !recentlySucceed;
	}
}
void UART_IRQ() interrupt 4{
	recentlyRecieve=TRUE;
	
	// disable serial interruption
	ES = FALSE;
	// reset RI
	RI = FALSE;
	// read a byte from buffer
	buffer_serial[interruptCounter_BytesReciever_PC++] = SBUF;
	// analyzer the frame
	if(interruptCounter_BytesReciever_PC >= FRAME_LENGTH_PC){
		Command_Analyzer();
		interruptCounter_BytesReciever_PC = 0;
	}
	// enable serial interruption
	ES = TRUE;
}
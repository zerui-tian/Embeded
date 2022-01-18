#include "main.h"
#include "SerialPort.h"
#define BIN2BCD(bin) (((bin)%10)|(((bin)/10)<<4))

char portId[5];
char gszPort[20];

int main(int argc, char** argv) {


	printf("input the port COM number: \n");
	scanf("%s", &portId);
 
	strcpy(gszPort, "\\\\.\\com");
	strcat(gszPort, portId);
 
	if (OpenPort(gszPort)) {
 
		if (SetupPort()) {

			time_t currentTime;
		   	struct tm *currentTime_Formatted;
		   	time( &currentTime );
		   	currentTime_Formatted = localtime( &currentTime );

			SendByte(0x01);
			SendByte(BIN2BCD((uint8_t)(currentTime_Formatted->tm_sec)));
			SendByte(BIN2BCD((uint8_t)(currentTime_Formatted->tm_min)));
			SendByte(BIN2BCD((uint8_t)(currentTime_Formatted->tm_hour)));
			SendByte(BIN2BCD((uint8_t)(currentTime_Formatted->tm_wday)));
			SendByte(BIN2BCD((uint8_t)(currentTime_Formatted->tm_mday)));
			SendByte(BIN2BCD((uint8_t)(currentTime_Formatted->tm_mon+1)));
			SendByte(BIN2BCD((uint8_t)(currentTime_Formatted->tm_year%100)));
			SendByte(BIN2BCD((uint8_t)(currentTime_Formatted->tm_year/100+19)));
			SendByte(0xFF);
			SendByte(0xFF);
			SendByte(0xFF);
			SendByte(0xFF);
			SendByte(0xFF);
			SendByte(0xFF);
			SendByte(0xFF);
        }
 
            ClosePort();
    }
	return 0;
}

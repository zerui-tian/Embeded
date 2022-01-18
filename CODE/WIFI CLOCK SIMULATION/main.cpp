#include "main.h"
#include "SerialPort.h"
#define SENDCMD(cmd) SendByteStream(cmd,sizeof(cmd)-1)

int main(int argc, char** argv) {
	char gszPort[] = "\\\\.\\com5";
 
	if(!OpenPort(gszPort)){
		puts("Open com 5 failed\n");
		exit(1); 
	}
	if(!SetupPort()) {
		puts("Setup com 5 failed\n");
		exit(2); 			
    }
    SENDCMD("AT+RST\r\n");
    Sleep(20000);
    
    SENDCMD("AT+CWMODE=1\r\n");
    Sleep(10000);
    
    SENDCMD("AT+CIPSTART=\"UDP\",\"203.107.6.88\",123\r\n");
    Sleep(1000);
    
    SENDCMD("AT+CIPMODE=1\r\n");
    Sleep(1000);
    
    SENDCMD("AT+CIPSEND\r\n");
    Sleep(1000);
    
	ClosePort();
	return 0;
}

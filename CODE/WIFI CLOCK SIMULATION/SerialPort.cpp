#include "SerialPort.h"
#include "main.h"
HANDLE hComm;
//SYSTEMTIME etime;
 
bool OpenPort(char * gszPort) { 
    hComm = CreateFile(gszPort,                                     // pointer to name of the file
                       GENERIC_READ | GENERIC_WRITE,								// access (read-write) mode
                       0,                                                          // share mode
                       0,                                                          // pointer to security attributes
                       OPEN_EXISTING,												// how to create
                       0,                                                          // file attributes
                       0);															// handle to file with attributes to copy
    return hComm != INVALID_HANDLE_VALUE;
}
 
bool SetupPort() {
 
    DCB dcb;
 
    //printf("setting up DCB\n");
    //FillMemory(&dcb, sizeof(dcb), 0);   //initalize dcb
    //dcb.DCBlength = sizeof(dcb);
 
//	printf("getting DCB\n");
 
    if (!GetCommState(hComm, &dcb)) {
        return 0;
    }
 
    dcb.BaudRate = BAUDRATE;
    dcb.fParity = FALSE;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.ByteSize = 8;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
 
    //      if (!BuildCommDCB("9600,n,8,1", &dcb)) {
    //              printf("Port configuration failed\n");
    //              return FALSE;
    //  }
//	printf("DCB ready for use\n");
    if (!SetCommState(hComm, &dcb)) {
        return 0;
    } else {
        return 1;
    }
}
 
bool PurgePort() {
	return PurgeComm(hComm, PURGE_RXCLEAR);
}
 
bool ClosePort() {
    return CloseHandle(hComm);
}
 
bool SendByte(char byteToWrite) {
    DWORD dwWritten;
    return WriteFile(hComm, &byteToWrite, sizeof(byteToWrite), &dwWritten, 0);
}
 
char ReadByte() {
    DWORD dwRead;
    unsigned char lpBuf;
	
    while(ReadFile(hComm,                     // handle of file to read
             	&lpBuf,                         // address of buffer that receives data
             sizeof(lpBuf),                  // number of bytes to read
             &dwRead,                        // address of number of bytes read
             0) != EOF)                            // address of structure for data
	{
		printf("%c",lpBuf);
	}
    return lpBuf;
}

bool SendByteStream(char * stream, int stream_size){
	int cnt;
	for(cnt = 0; cnt < stream_size; cnt++){
		if(SendByte(stream[cnt])){
			/* do nothing */
		}
		else{
			return 0;
		}
	}
	return 1;
}

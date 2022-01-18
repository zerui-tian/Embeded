#include "SerialPort.h"

HANDLE hComm;
//SYSTEMTIME etime;
 
bool OpenPort(char * gszPort) {
    printf("try to open the port %s \n", gszPort);
 
    hComm = CreateFile(gszPort,                                     // pointer to name of the file
                       GENERIC_READ | GENERIC_WRITE,								// access (read-write) mode
                       0,                                                          // share mode
                       0,                                                          // pointer to security attributes
                       OPEN_EXISTING,												// how to create
                       0,                                                          // file attributes
                       0);															// handle to file with attributes to copy
 
    if (hComm == INVALID_HANDLE_VALUE) {
        printf("failed to open serial port %s \n", gszPort);
        return 0;
    } else {
        printf("serial port %s opened \n", gszPort);
        return 1;
    }
}
 
bool SetupPort() {
    printf("try to set up the port \n");
 
    DCB dcb;
 
    //printf("setting up DCB\n");
    //FillMemory(&dcb, sizeof(dcb), 0);   //initalize dcb
    //dcb.DCBlength = sizeof(dcb);
 
//	printf("getting DCB\n");
 
    if (!GetCommState(hComm, &dcb)) {
        printf("getDCB failed\n");
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
        printf("failed to set port state (%d)\n", GetLastError());
        return 0;
    } else {
        printf("Port setup complete\n");
        return 1;
    }
}
 
bool PurgePort() {
	if (PurgeComm(hComm, PURGE_RXCLEAR)) {
	 	//printf("Port purged\n");
		return 1;
	}
	else {
		printf("Port purge failed\n");
		return 0;
	}
}
 
bool ClosePort() {
    printf("try to close the port \n");
 
    if (CloseHandle(hComm)) {
        printf("Port closed\n");
        return 1;
    } else {
        printf("Port close failed\n");
        return 0;
    }
}
 
bool SendByte(uint8_t byteToWrite) {
    DWORD dwWritten;
    if (WriteFile(hComm, &byteToWrite, sizeof(byteToWrite), &dwWritten, 0)) {
    	printf("wrote byte %d to serial port\n", byteToWrite, byteToWrite);
        return 1;
    } else {
        return 0;
    }
}
 
uint8_t ReadSByte() {
    DWORD dwRead;
    unsigned char lpBuf;
	
    ReadFile(hComm,                     // handle of file to read
             &lpBuf,                         // address of buffer that receives data
             sizeof(lpBuf),                  // number of bytes to read
             &dwRead,                        // address of number of bytes read
             0);                             // address of structure for data
    //printf("Read byte %Xh from serial port\n",lpBuf);
    return lpBuf;
}

bool SendByteStream(const uint8_t * stream, const int32_t stream_size){
	int32_t cnt;
	for(cnt = 0; cnt < stream_size; cnt++){
		if(SendByte(stream[cnt])){
			/* do nothing */
		}
		else{
			printf("Transmission failed.\n");
			return 0;
		}
	}
	printf("Transmit successfully.\n");
	return 1;
}

#ifndef SERIALPORT_H

#define SERIALPORT_H

#include "main.h"
#include <stdbool.h>
#include <Windows.h>	//Contain definations for handle, DWORD and DCB

#define BAUDRATE 9600

//int portId;				//e.g Com#3 's portId is 3.
//TCHAR commPort[11];		//e.g "\\\\.\\com3"
 
/*
    Definitions for the methods about serial port operations.
*/
extern bool OpenPort(char *gszPort);
 
extern bool SetupPort();
 
extern bool PurgePort();
 
extern bool ClosePort();
 
extern bool SendByte(uint8_t byte);
 
extern uint8_t ReadSByte();

bool SendByteStream(const uint8_t * stream, const int32_t stream_size);
 
#endif  //SERIALPORT_H

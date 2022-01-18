#include<main.h>
#define GETTIME_SIZE				8

extern void DS12C887_Init();
extern uchar* DS12C887_GetTime();
extern void DS12C887_SetTime(uchar* aim);
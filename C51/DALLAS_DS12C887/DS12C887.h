#include<main.h>
#define GETTIME_SIZE				8

extern void DS12C887_Init();
extern void DS12C887_GetTime(char*);
extern void DS12C887_SetTime(char*);
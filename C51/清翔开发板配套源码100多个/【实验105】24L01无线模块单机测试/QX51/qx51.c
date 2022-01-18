/*
*功能：
*	初始化QX-X351开发板
*时间：
*	14-2-13
*/
#include "reg52.h"/*包含STC单片机头文件*/
#include "qx51.h"/*包含qx51开发板头文件*/
void qx51_init(void)
{
	wela=0;//关闭数码管显示位选
	dula=0;//关闭数码管显示段选
}
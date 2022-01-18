#ifndef DS1602_H
#define DS1602_H
unsigned char datstr[];
unsigned char datnum[];
unsigned char str[];//uchartostr函数转换的字符串 同时可以把16进制转成10进制 */

void write_com(unsigned char com);  //写命令
void write_data(unsigned char date);	  //写数据
void lcd_dis(unsigned char X,unsigned char Y,unsigned char *dis);	//显示数据数组
void lcd_distostr(unsigned char X,unsigned char Y,unsigned char dis);//显示字符串 数组
unsigned char *uchartostr(unsigned char num);	//将一个字节的数据转换为字符串 或10进制
void init_1602();//初始化1602


#endif
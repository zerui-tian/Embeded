#ifndef DS1602_H
#define DS1602_H
unsigned char datstr[];
unsigned char datnum[];
unsigned char str[];//uchartostr����ת�����ַ��� ͬʱ���԰�16����ת��10���� */

void write_com(unsigned char com);  //д����
void write_data(unsigned char date);	  //д����
void lcd_dis(unsigned char X,unsigned char Y,unsigned char *dis);	//��ʾ��������
void lcd_distostr(unsigned char X,unsigned char Y,unsigned char dis);//��ʾ�ַ��� ����
unsigned char *uchartostr(unsigned char num);	//��һ���ֽڵ�����ת��Ϊ�ַ��� ��10����
void init_1602();//��ʼ��1602


#endif
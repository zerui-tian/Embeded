#include <reg52.h> 
#define XTAL 11059200
#define baudrate 9600
 
void uart_init(){
	TMOD = 0x22;
	TH0=0x00;
	TH0=0x00;
	TH1=(unsigned char)(256 - (XTAL / (32L * 12L * baudrate)));
	TL1=(unsigned char)(256 - (XTAL / (32L * 12L * baudrate)));
	SCON = 0x50;
	PCON = 0x00;
	TR1 = 1;
	TR0 = 1;
	IT0=1;
	EX0=1;
	EA=1;
}
void uart_tx_byte(char byte){
	SBUF=byte;
	while(TI == 0);
	TI = 0;
}
void main(void){
    uart_init();
    while(1){
		}
}
void Random_Number_Generator() interrupt 0{
	unsigned char random = TL0;
	uart_tx_byte(random);
}
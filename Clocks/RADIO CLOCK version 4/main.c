#include <msp430.h>
#include "ht1621.h"
/**********
 *  Mode  *
 **********/
#define NORMAL          0
#define DISPLAYSEC      1
#define DISPLAYDAY      2
#define DISPLAYWEEK     3
#define DISPLAYYEAR     4
#define DISPLAYSTATUS   5
#define ADJUSTHOUR      6
#define ADJUSTMINUTE    7
#define RESETSEC        8
#define ADJUSTDAY       9
#define ADJUSTMONTH     10
#define ADJUSTWEEK      11
#define ADJUSTYEAR      12
unsigned char status = NORMAL;

/**********
 *  Time  *
 **********/
#define SECOND 0
#define MINUTE 1
#define HOUR 2
#define WEEKDAY 3
#define DAY 4
#define MONTH 5
#define YEAR 6
int innerClock[7] = {0,0,0,4,1,1,1970};
int receivedTimeBuffer1[7];
int receivedTimeBuffer2[7];
int *currentReceivedTime = receivedTimeBuffer1;
int *lastReceivedTime = receivedTimeBuffer2;
int *addrExchangeTempVar;
char refreshTimeFlag = 0;

/**************************************
 *  The number of days in each innerClock[MONTH]  *
 **************************************/
int monthdayAmount[12] = {
    31,28,31,30,31,30,31,31,30,31,30,31
};

/***************************
 *  Synchronization Flags  *
 ***************************/
char fipfop = 0;
char phaseSynchronized = 0;
char timeSynchronized = 0;
/******************************************
 *  Synchronization Counters and Buffers  *
 ******************************************/
unsigned char mainCounter = 0;
unsigned int  syncCounter = 0;
unsigned int  listenCounterDown;
unsigned char freqMatchCounter = 0;
unsigned char frameReceivedCounter = 0;
unsigned char decodeCounter = 0;
unsigned char decodeBuffer[20];
unsigned char decodePointer = 0;

void Refresh(){
    switch(status){
            case NORMAL:
                HT1621_Display(
                        innerClock[HOUR]/10,
                        innerClock[HOUR]%10,
                        innerClock[MINUTE]/10,
                        innerClock[MINUTE]%10,
                        fipfop, 0);
                break;
            case DISPLAYSEC:
                HT1621_Display(
                        0xFF,
                        0xFF,
                        innerClock[SECOND]/10,
                        innerClock[SECOND]%10,
                        fipfop, 0);
                break;
            case DISPLAYDAY:
                HT1621_Display(
                        innerClock[MONTH]/10,
                        innerClock[MONTH]%10,
                        innerClock[DAY]/10,
                        innerClock[DAY]%10,
                        0, 1);
                break;
            case DISPLAYWEEK:
                HT1621_Display(
                        0xFF,
                        0xFF,
                        0xFF,
                        innerClock[WEEKDAY]%10,
                        0, 0);
                break;
            case DISPLAYYEAR:
                HT1621_Display(
                    innerClock[YEAR]/1000,
                    (innerClock[YEAR]/100)%10,
                    (innerClock[YEAR]/10)%10,
                    innerClock[YEAR]%10,
                    0, 0);
                break;
            case DISPLAYSTATUS:
                HT1621_UpdateStatusDisplay(frameReceivedCounter,phaseSynchronized,timeSynchronized,listenCounterDown);
                break;
            case ADJUSTHOUR:
                HT1621_Display(
                        fipfop?(innerClock[HOUR]/10):0xFF,
                        fipfop?(innerClock[HOUR]%10):0xFF,
                        innerClock[MINUTE]/10,
                        innerClock[MINUTE]%10,
                        fipfop, 0);
                break;
            case ADJUSTMINUTE:
                HT1621_Display(
                        innerClock[HOUR]/10,
                        innerClock[HOUR]%10,
                        fipfop?(innerClock[MINUTE]/10):0xFF,
                        fipfop?(innerClock[MINUTE]%10):0xFF,
                        fipfop, 0);
                break;
            case RESETSEC:
                HT1621_Display(
                        0xFF,
                        0xFF,
                        fipfop?(innerClock[SECOND]/10):0xFF,
                        fipfop?(innerClock[SECOND]%10):0xFF,
                        fipfop, 0);
                break;
            case ADJUSTWEEK:
                HT1621_Display(
                        0xFF,
                        0xFF,
                        0xFF,
                        fipfop?(innerClock[WEEKDAY]%10):0xFF,
                        0, 0);
                break;
            case ADJUSTMONTH:
                HT1621_Display(
                        fipfop?(innerClock[MONTH]/10):0xFF,
                        fipfop?(innerClock[MONTH]%10):0xFF,
                        innerClock[DAY]/10,
                        innerClock[DAY]%10,
                        0, 1);
                break;
            case ADJUSTDAY:
                HT1621_Display(
                        innerClock[MONTH]/10,
                        innerClock[MONTH]%10,
                        fipfop?(innerClock[DAY]/10):0xFF,
                        fipfop?(innerClock[DAY]%10):0xFF,
                        0, 1);
                break;
            case ADJUSTYEAR:
                HT1621_Display(
                    fipfop?(innerClock[YEAR]/1000):0xFF,
                    fipfop?((innerClock[YEAR]/100)%10):0xFF,
                    fipfop?((innerClock[YEAR]/10)%10):0xFF,
                    fipfop?(innerClock[YEAR]%10):0xFF,
                    0, 0);
                break;
            default:
                break;
    }
}

void RunTwicePerSecond(){
    if(fipfop){

        //update time buffer
        innerClock[SECOND]++;
        if(innerClock[SECOND] >= 60){
            innerClock[SECOND] = 0;
            innerClock[MINUTE]++;
        }
        if(innerClock[MINUTE] >= 60){
            innerClock[MINUTE] = 0;
            innerClock[HOUR]++;
        }
        if(innerClock[HOUR] >= 24){
            innerClock[HOUR] = 0;
            innerClock[DAY]++;
            innerClock[WEEKDAY] = (innerClock[WEEKDAY]+1)%7;
        }
        if(innerClock[DAY] > monthdayAmount[innerClock[MONTH]-1]){
            innerClock[DAY] = 1;
            innerClock[MONTH]++;
        }
        if(innerClock[MONTH] > 12){
            innerClock[MONTH] = 1;
            innerClock[YEAR]++;
        }
        monthdayAmount[1] =
            ((innerClock[YEAR]%4==0)&&(innerClock[YEAR]%100!=0))||(innerClock[YEAR]%400==0) ? 29 : 28;

    }
    Refresh();
}

void RadioListeningOn(){
    __disable_interrupt();
    listenCounterDown = 1200;
    freqMatchCounter = 0;
    decodePointer = 0;
    frameReceivedCounter = 0;
    phaseSynchronized = 0;
    timeSynchronized = 0;

    P2OUT &= ~BIT1;
    P2IE |= BIT2; // enable P2.2 interrupt

    TA0CCR0 = 16 - 1;
    TA0CCTL0 = CCIE; // enable compare interrupt
    TA0CTL = TASSEL_1 + ID_3 + MC_1 + TACLR;
    __enable_interrupt();
}

void RadioListeningOff(){
    TA0CCR0 = 2048 - 1;
    TA0CCTL0 = CCIE; // enable compare interrupt
    TA0CTL = TASSEL_1 + ID_3 + MC_1 + TACLR;
    // input clock: ACLK/8 -> 4.096kHz;
    // timer overflow freq: 4.096k/(TA0CCR0+1) -> 2Hz
    // clear and start the timer, up mode
    P2OUT |= BIT1;
}

char UpdateTime(){
    int monthdayAmountTemp;
    if((currentReceivedTime[SECOND]==1)||(currentReceivedTime[SECOND]==21)||currentReceivedTime[SECOND]==41){
        if(currentReceivedTime[SECOND]==41){
            currentReceivedTime[SECOND] = 1;
            currentReceivedTime[MINUTE]++;
            if(currentReceivedTime[MINUTE] >= 60){
                currentReceivedTime[MINUTE] = 0;
                currentReceivedTime[HOUR]++;
            }
            if(currentReceivedTime[HOUR] >= 24){
                currentReceivedTime[HOUR] = 0;
                currentReceivedTime[DAY]++;
                currentReceivedTime[WEEKDAY] = (currentReceivedTime[WEEKDAY]+1)%7;
            }
            if(currentReceivedTime[MONTH]==2){
                monthdayAmountTemp =
                    ((currentReceivedTime[YEAR]%4==0)&&(currentReceivedTime[YEAR]%100!=0))||(currentReceivedTime[YEAR]%400==0) ? 29 : 28;
            }
            else{
                monthdayAmountTemp = monthdayAmount[currentReceivedTime[MONTH]-1];
            }
            if(currentReceivedTime[DAY] > monthdayAmountTemp){
                currentReceivedTime[DAY] = 1;
                currentReceivedTime[MONTH]++;
            }
            if(currentReceivedTime[MONTH] > 12){
                currentReceivedTime[MONTH] = 1;
                currentReceivedTime[YEAR]++;
            }
        }
        else{
            currentReceivedTime[SECOND]+=20;
        }

        innerClock[SECOND] = currentReceivedTime[SECOND];
        innerClock[MINUTE] = currentReceivedTime[MINUTE];
        innerClock[HOUR] = currentReceivedTime[HOUR];
        innerClock[WEEKDAY] = currentReceivedTime[WEEKDAY];
        innerClock[DAY] = currentReceivedTime[DAY];
        innerClock[MONTH] = currentReceivedTime[MONTH];
        innerClock[YEAR] = currentReceivedTime[YEAR];

        monthdayAmount[1] =((innerClock[YEAR]%4==0)&&(innerClock[YEAR]%100!=0))||(innerClock[YEAR]%400==0) ? 29 : 28;

        return 1;
    }
    else{
        return 0;
    }
}

unsigned char PulseReferee(unsigned char decodeCounter){
    if(decodeCounter <= 13)         return 0xFF;
    else if(decodeCounter <= 38)    return 0x00;
    else if(decodeCounter <= 64)    return 0x01;
    else if(decodeCounter <= 90)    return 0x02;
    else if(decodeCounter <= 115)   return 0x03;
    else                            return 0xFE;
}

char FrameDecoder(){
    int cnt;
    for(cnt = 1; cnt < 20; cnt++){
        if(decodeBuffer[(decodePointer+cnt)%20]==0xFF || decodeBuffer[(decodePointer+cnt)%20]==0xFE){
            return 0;
        }
    }

    //sets second
    switch(decodeBuffer[(decodePointer+1)%20]){
        case 0x00:
            currentReceivedTime[SECOND] = 1;
            break;
        case 0x01:
            currentReceivedTime[SECOND] = 21;
            break;
        case 0x02:
            currentReceivedTime[SECOND] = 41;
            break;
        default:
            return 0;
    }

    //sets minute
    currentReceivedTime[MINUTE] = decodeBuffer[(decodePointer+5)%20]*16+
                                                                decodeBuffer[(decodePointer+6)%20]*4+
                                                                decodeBuffer[(decodePointer+7)%20];

    //sets hour
    currentReceivedTime[HOUR] =     decodeBuffer[(decodePointer+3)%20]*4+
                                                                decodeBuffer[(decodePointer+4)%20]+
                                                                (((decodeBuffer[(decodePointer+10)%20]==0x00)||(decodeBuffer[(decodePointer+10)%20]==0x01))?0:12);

    //sets weekday
    currentReceivedTime[WEEKDAY] =  decodeBuffer[(decodePointer+8)%20]*4+
                                                                    decodeBuffer[(decodePointer+9)%20];

    //set day
    currentReceivedTime[DAY] =  decodeBuffer[(decodePointer+11)%20]*16+
                                                            decodeBuffer[(decodePointer+12)%20]*4+
                                                            decodeBuffer[(decodePointer+13)%20];

    //sets month
    currentReceivedTime[MONTH] =    decodeBuffer[(decodePointer+14)%20]*4+
                                                                decodeBuffer[(decodePointer+15)%20];

    //set year
    currentReceivedTime[YEAR] = decodeBuffer[(decodePointer+16)%20]*16+
                                                            decodeBuffer[(decodePointer+17)%20]*4+
                                                            decodeBuffer[(decodePointer+18)%20]+
                                                            2000;


    return 1;
}

char FrameComparator(){
    int i;
    int monthdayAmountTemp;
    if((lastReceivedTime[SECOND]==41)&&(currentReceivedTime[SECOND]==1)){
        lastReceivedTime[MINUTE]++;
        if(lastReceivedTime[MINUTE] >= 60){
            lastReceivedTime[MINUTE] = 0;
            lastReceivedTime[HOUR]++;
        }
        if(lastReceivedTime[HOUR] >= 24){
            lastReceivedTime[HOUR] = 0;
            lastReceivedTime[DAY]++;
            lastReceivedTime[WEEKDAY] = (lastReceivedTime[WEEKDAY]+1)%7;
        }
        if(lastReceivedTime[MONTH]==2){
            monthdayAmountTemp =
                ((lastReceivedTime[YEAR]%4==0)&&(lastReceivedTime[YEAR]%100!=0))||(lastReceivedTime[YEAR]%400==0) ? 29 : 28;
        }
        else{
            monthdayAmountTemp = monthdayAmount[lastReceivedTime[MONTH]-1];
        }
        if(lastReceivedTime[DAY] > monthdayAmountTemp){
            lastReceivedTime[DAY] = 1;
            lastReceivedTime[MONTH]++;
        }
        if(lastReceivedTime[MONTH] > 12){
            lastReceivedTime[MONTH] = 1;
            lastReceivedTime[YEAR]++;
        }
        for(i=1; i<7; i++){
            if(lastReceivedTime[i]!=currentReceivedTime[i]){
                return 0;
            }
        }
        return 1;
    }
    else if(
            ((lastReceivedTime[SECOND]==21)&&(currentReceivedTime[SECOND]==41))
        ||((lastReceivedTime[SECOND]==1)&&(currentReceivedTime[SECOND]==21))
    ){
        for(i=1; i<7; i++){
            if(lastReceivedTime[i]!=currentReceivedTime[i]){
                return 0;
            }
        }
        return 1;
    }
    else{
        return 0;
    }
}
char Decode(){

    if(FrameDecoder()){
        //frameReceivedCounter is zero means the values in lastReceivedTime is invalid
        if(frameReceivedCounter==0){
            addrExchangeTempVar = currentReceivedTime;
            currentReceivedTime = lastReceivedTime;
            lastReceivedTime = addrExchangeTempVar;
            frameReceivedCounter++;
            return 0;
        }
        else{
            if(FrameComparator()){
                frameReceivedCounter++;
                if(frameReceivedCounter>=4){
                    return 1;
                }
                else{
                    addrExchangeTempVar = currentReceivedTime;
                    currentReceivedTime = lastReceivedTime;
                    lastReceivedTime = addrExchangeTempVar;
                    return 0;
                }
            }
            else{
                addrExchangeTempVar = currentReceivedTime;
                currentReceivedTime = lastReceivedTime;
                lastReceivedTime = addrExchangeTempVar;
                frameReceivedCounter = 1;
                return 0;
            }
        }
    }
    else{
        frameReceivedCounter = 0;
        return 0;
    }
}

void P10_Onclick(){
    switch(status){
        case NORMAL:
            status = DISPLAYSEC;
            break;
        case DISPLAYSEC:
            status = DISPLAYDAY;
            break;
        case DISPLAYDAY:
            status = DISPLAYWEEK;
            break;
        case DISPLAYWEEK:
            status = DISPLAYYEAR;
            break;
        case DISPLAYYEAR:
            status = DISPLAYSTATUS;
            break;
        case DISPLAYSTATUS:
            status = NORMAL;
            break;
        case ADJUSTHOUR:
            status = NORMAL;
            break;
        case ADJUSTMINUTE:
            status = NORMAL;
            break;
        case RESETSEC:
            status = DISPLAYSEC;
            break;
        case ADJUSTDAY:
            status = DISPLAYDAY;
            break;
        case ADJUSTMONTH:
            status = DISPLAYDAY;
            break;
        case ADJUSTWEEK:
            status = DISPLAYWEEK;
            break;
        case ADJUSTYEAR:
            status = DISPLAYYEAR;
            break;
        default:
            break;
    }
}

void P13_Onclick(){
    switch(status){
        case NORMAL:
            status = ADJUSTHOUR;
            break;
        case ADJUSTHOUR:
            status = ADJUSTMINUTE;
            break;
        case ADJUSTMINUTE:
            status = ADJUSTHOUR;
            break;
        case DISPLAYSEC:
            status = RESETSEC;
            break;
        case DISPLAYDAY:
            status = ADJUSTMONTH;
            break;
        case ADJUSTMONTH:
            status = ADJUSTDAY;
            break;
        case ADJUSTDAY:
            status = ADJUSTMONTH;
            break;
        case DISPLAYWEEK:
            status = ADJUSTWEEK;
            break;
        case DISPLAYYEAR:
            status = ADJUSTYEAR;
            break;
        default:
            break;
    }
}

void P16_Onclick(){
    switch(status){
        case ADJUSTYEAR:
            innerClock[YEAR]++;
            break;
        case ADJUSTMONTH:
            innerClock[MONTH]++;
            if(innerClock[MONTH] > 12){
                innerClock[MONTH] = 1;
            }
            break;
        case ADJUSTDAY:
            innerClock[DAY]++;
            if(innerClock[DAY] > monthdayAmount[innerClock[MONTH]-1]){
                innerClock[DAY] = 1;
            }
            break;
        case ADJUSTWEEK:
            innerClock[WEEKDAY] = (innerClock[WEEKDAY]+1)%7;
        case ADJUSTHOUR:
            innerClock[HOUR]++;
            if(innerClock[HOUR] >= 24){
                innerClock[HOUR]=0;
            }
            break;
        case ADJUSTMINUTE:
            innerClock[MINUTE]++;
            if(innerClock[MINUTE] >= 60){
                innerClock[MINUTE]=0;
            }
            break;
        case RESETSEC:
            innerClock[SECOND]=0;
            fipfop = 1;
            break;
        default:
            break;
    }
    monthdayAmount[1] =((innerClock[YEAR]%4==0)&&(innerClock[YEAR]%100!=0))||(innerClock[YEAR]%400==0) ? 29 : 28;
}
void P17_Onclick(){
    switch(status){
        case ADJUSTYEAR:
            innerClock[YEAR]--;
            break;
        case ADJUSTMONTH:
            innerClock[MONTH]--;
            if(innerClock[MONTH] <= 0){
                innerClock[MONTH] = 12;
            }
            break;
        case ADJUSTDAY:
            innerClock[DAY]--;
            if(innerClock[DAY] <= 0){
                innerClock[DAY] =  monthdayAmount[innerClock[MONTH]-1];
            }
            break;
        case ADJUSTWEEK:
            innerClock[WEEKDAY] =
                    innerClock[WEEKDAY]==0?6:innerClock[WEEKDAY]-1;
        case ADJUSTHOUR:
            innerClock[HOUR]--;
            if(innerClock[HOUR] < 0){
                innerClock[HOUR] = 23;
            }
            break;
        case ADJUSTMINUTE:
            innerClock[MINUTE]--;
            if(innerClock[MINUTE] < 0){
                innerClock[MINUTE]=59;
            }
            break;
        case RESETSEC:
            innerClock[SECOND]=0;
            fipfop = 1;
            break;
        default:
            break;
    }
    monthdayAmount[1] =((innerClock[YEAR]%4==0)&&(innerClock[YEAR]%100!=0))||(innerClock[YEAR]%400==0) ? 29 : 28;
}
void SYS_init(){

    /* Set DCO to 16MHz */
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    // P2.1 -> PON
    // P2.3 -> DA
    // P2.4 -> WR
    // P2.5 -> CS
    P2DIR |= BIT1+BIT3+BIT4+BIT5;

    // P2.2 -> RC signal
    P2DIR &= ~BIT2;

    // P2.2 positive edge interrupt
    P2IES &= ~BIT2;

    //-----配合机械按键，启用内部上拉电阻-----
    P1REN |= BIT0+BIT3+BIT6+BIT7;   //启用上下拉电阻
    P1OUT |= BIT0+BIT3+BIT6+BIT7;   //将电阻设置为上拉
    //-----配置P1.3中断参数-----
    P1DIR &= ~(BIT0+BIT3+BIT6+BIT7);
    P1IES |= BIT0+BIT3+BIT6+BIT7;
    P1IE  |= BIT0+BIT3+BIT6+BIT7;
}


void Initializer(){
    SYS_init();
    HT1621_Init();
    RadioListeningOn();
    __enable_interrupt();
}

void main(){
    WDTCTL = WDTPW + WDTHOLD;
    Initializer();
    while(1){
        if(refreshTimeFlag){
            RunTwicePerSecond();
            refreshTimeFlag = 0;
        }
        if(timeSynchronized){
            LPM3;
        }
    }
}

//interrupt service routines
#pragma vector = TIMER0_A0_VECTOR
__interrupt void CCR0_ISR(void){
    if(!timeSynchronized && listenCounterDown>0){
        mainCounter++;
        if(phaseSynchronized){
            if((P2IN&BIT2)!=0 && fipfop){decodeCounter++;}
        }
        else{
            syncCounter++;
        }

        if(mainCounter >= 128){
            mainCounter = 0;
            fipfop = !fipfop;
            refreshTimeFlag = 1;
            listenCounterDown--;
            if(listenCounterDown > 0){
                //Time is not synchronized
                //Frequency is synchronized
                //A new second has just started
                if(phaseSynchronized && fipfop){

                    decodeBuffer[decodePointer] = PulseReferee(decodeCounter);

                    //0xFF is considered as the end of the last frame
                    if((decodeBuffer[decodePointer]==0xFF)&&Decode()&&UpdateTime()){
                        RadioListeningOff();
                        timeSynchronized = 1;
                    }
                    else{
                        decodePointer = (decodePointer+1)%20;
                    }

                    decodeCounter = 0;
                }
            }
            else{
                RadioListeningOff();
            }
        }
    }
    else{
        fipfop = !fipfop;
        RunTwicePerSecond();
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(syncCounter >= 255 && syncCounter <= 257){
        freqMatchCounter++;
    }
    else if(syncCounter < 10){
        listenCounterDown = 0;
        RadioListeningOff();
    }
    else{
        freqMatchCounter = 0;
    }
    if(freqMatchCounter >= 3){

        //synchronize the inner clock with the radio signal
        mainCounter = 0;
        fipfop = 1;
        //disable P2.2 interrupt
        P2IE &= ~BIT2;
        //frequency synchronized
        phaseSynchronized = 1;
    }

    //reset the counter
    syncCounter = 0;
    P2IFG=0;
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void){
    unsigned int Push_Key=0;
    //-----排除输出IO的干扰后，锁定唯一被触发的中断标志位-----
    Push_Key=P1IFG&(~P1DIR);
    //-----延时一段时间，避开机械抖动区域-----
    __delay_cycles(10000);
    //----判断按键状态是否与延时前一致-----
    if((P1IN&Push_Key)==0){
     //----判断具体哪个IO被按下，调用该IO的事件处理函数-----
      switch(Push_Key){
        case BIT0:
            P10_Onclick();
            break;
        case BIT3:
            P13_Onclick();
            break;
//        case BIT4:
//            break;
//        case BIT5:
//            break;
        case BIT6:
            if(status == DISPLAYSTATUS)
                RadioListeningOn();
            else
                P16_Onclick();
            break;
        case BIT7:
            if(status == DISPLAYSTATUS)
                RadioListeningOn();
            else
                P17_Onclick();
            break;
        default:
            break;      //任何情况下均加上default
     }
   }
    P1IFG=0;                                        //退出中断前必须手动清除IO口中断标志
}

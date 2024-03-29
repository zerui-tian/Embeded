/*********************************************************************************
* 【编写时间】： 2014年3月5日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】： http://www.qxmcu.com/ 
* 【淘宝店铺】： http://qxmcu.taobao.com/
* 【实验平台】： QX-MCS51 单片机开发板
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52
* 【编译环境】： Keil μVisio3	
* 【程序功能】： 			   			            			    
* 【使用说明】： 实现楼道间照明灯的方便控制，一个照明灯，在楼上和楼下各有一个（闭合式）开关控制。
			开关的闭合并不直接决定灯的亮灭，而开关状态的改变才是决定灯亮灭的关键。即不管两个开关当前的状态
			如何，只要有一个开关状态发生变化，都会使照明灯在打亮或者熄灭两种状态间切换。
			比如当有人要上楼，若之前灯是灭的，在按下楼下开关后（此后开关为闭合状态），灯会打亮；当到楼上后，
			再按楼上开关，灯会熄灭。而当后来又有人上楼时，再按次楼下开关（此后开关为打开状态），灯同样会打亮；
			此试验中，用P1口相连的第一个led来模拟照明灯，而用s2和s3来模拟两个开关。注意，因为实际楼道中的
			开关为闭合式的，而QX-MCS51的开关为反弹式的，所以实验时，请用手一直按着按键演示开关的闭合。
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

#include <reg52.h>

sbit light=P1^0;
sbit s2=P3^0;
sbit s3=P3^1;

bit change,s2sta,s3sta;//s2sta,s3sta为开关s2和s3前一时刻的状态，用来和当前的s1与s2状态进行对比，
					   //进而判决两个开关的状态有无发生变化。
bit keyscan()
{
	if((s2^s2sta)||(s3^s3sta)) //^为异或，若两者不同，则结果为1。即若两开关中，任一开关的状态发生变化，则此条件即为真。
		change=1;
	return change;
}

void main()
{	
	s2sta=s2;
	s3sta=s3;
	while(1)
	{
		keyscan();		   //扫描按键状态是否变化，即是否有人更改了按键状态。
		if(change)
		{
			change=0;	   //清零
			light=~light;  //一旦发现有开关状态发生了变化，则灯的状态就取反。（若以前亮，则熄灭；反之则打亮）
			s2sta=s2;	   //同时，将s2和s3的当前状态送给s2sta和s3sta，为以后的比较做准备。
			s3sta=s3;
		}	
	}
}
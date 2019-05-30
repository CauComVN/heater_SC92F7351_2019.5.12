#include "H/Function_Init.H" 
#include "stdio.h"

#define UART0_BUFF_LENGTH	  7	//UART0接受缓冲区的容量（Byte) 00001 \0
uchar Uart0BuffNumber=0;
uchar Uart0Buff[UART0_BUFF_LENGTH];

void UART_SentChar(uchar chr);
void UART_SendString(uchar *str);
	
void Uart0_Init(void);
bit UartSendFlag = 0; //发送中断标志位
bit UartReceiveFlag = 0; //接收中断标志位

uchar receivchar=0x00;

char putchar(char c)//用于重写printf
{
	SBUF = c;
	while(!UartSendFlag);
	UartSendFlag=0;
	return c;
}

/*****************************************************
*函数名称：void Uart0_Test(void)
*函数功能：Uart0测试
*入口参数：void
*出口参数：void
*****************************************************/
void Uart0_Test(void)
{
	Uart0BuffNumber = 0;
	
	Uart0_Init();
	while(1)
	{
		/*
		SBUF = 0x55;
		while(!UartSendFlag);
		UartSendFlag = 0;
		*/		
		
		/*
		printf("00100\n");
		
		if(Uart0BuffNumber>=(UART0_BUFF_LENGTH-1))				//接收计数
	  {	
			UART_SendString(Uart0Buff);
			//用户可以在这期间执行其他代码			
			Uart0BuffNumber=0	;				 //将缓冲数组指向开始
	  }
		*/
		
		/*
		if(Uart0BuffNumber>=(UART0_BUFF_LENGTH-1))				//接收计数
	  {	
			UART_SendString(Uart0Buff);
			//用户可以在这期间执行其他代码			
			Uart0BuffNumber=0	;				 //将缓冲数组指向开始
	  }
		*/
		
		if(UartReceiveFlag)
		{
			UartReceiveFlag=0;
			
			receivchar=SBUF;
			receivchar+=0x10;
				
			SBUF = receivchar;	
			while(!UartSendFlag);
			UartSendFlag = 0;		
			
		}
	}
}
/*****************************************************
*函数名称：void Uart0_Init(void)
*函数功能：Uart0中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：Freq,baud
*****************************************************/
void Uart0_Init(void)    //选择Timer1作为波特率信号发生器
{
//	P1CON = 0X08;
//	P1PH = 0X04;	 //TX为强推输出，RX为带上拉输入
	
	P1CON |= 0x08; // p13 TX 强推输出
  P1CON &= 0xfb; // p12 RX 输入
  P1PH |= 0x04;	 //TX为强推输出，RX为带上拉输入	
	
	P13 = 1;		 //TX初始高电平；
	SCON = 0X50;     //方式1，允许接收数据
	PCON |= 0X80; 
	T2CON = 0x00;    //使用定时器1作UART时钟
	TMOD = 0X20;     //定时器1  8位自动重载
	TMCON = 0X02;    //定时器1   Fsys；
	TL1 = 217;		
	TH1 = 217;		 //UART 波特率24M情况下=38400；
	TR1 = 0;
	ET1 = 1;		 //Timer1使能
	TR1 = 1;		 //启动Timer0
	EUART = 1;	     //允许UART中断
	EA = 1;		     //开总中断
}

/*
void Uart0_Init(void)    //选择Timer2作为波特率信号发生器
{
	P1CON = 0X08;
	P1PH = 0X04;	 //TX为强推输出，RX为带上拉输入；
	P13 = 1;
	SCON  = 0X50;    //设置通信方式为模式一，允许接收
	PCON |= 0X80; 
	TMCON |= 0X04;
	T2MOD = 0X00;
	T2CON = 0X30;
	RCAP2H = 0xFF;
	RCAP2L = 0xB2;
	TR2 = 0;
	ET2 = 1;
	TR2 = 1;
	EUART = 1;     //开启Uart0中断
	EA = 1;
}
*/

/*****************************************************
*函数名称：void UartInt(void) interrupt 4
*函数功能：Uart0中断函数
*入口参数：void
*出口参数：void
*****************************************************/
void UartInt(void) interrupt 4
{
	if(TI)
	{
		TI = 0;	
		UartSendFlag = 1;		
	}
	if(RI)
	{
		RI = 0;	
		UartReceiveFlag = 1;
		
		Uart0Buff[Uart0BuffNumber] = SBUF; //将接收的数据存入缓冲区
		Uart0BuffNumber++;
	}	
}


void UART_SentChar(uchar chr)
{
      //发送一个字节
  SBUF = chr;
  while( TI == 0);
  TI = 0;
}
void UART_SendString(uchar *str)
{
  while(*str != '\0')
  {
      UART_SentChar(*str++);
  }
}
//--------------------- 
//作者：qq_41549389 
//来源：CSDN 
//原文：https://blog.csdn.net/qq_41549389/article/details/80742032 
//版权声明：本文为博主原创文章，转载请附上博文链接！


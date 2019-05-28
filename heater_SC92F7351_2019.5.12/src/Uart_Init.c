#include "Function_Init.H" 

void Uart0_Init(void);
void Uart0_Test(void);
void UART_SentChar(uchar chr);

//下位机热水器通信协议
uchar Protocol_Heater_Default						= 0x00;			//初始值
uchar Protocol_Heater_Start						  = 0x01; 		//启动热水器
uchar Protocol_Heater_Stop  						= 0x02; 		//停止热水器
uchar Protocol_Heater_Increases_Power   = 0x03;			//热水器增加功率
uchar Protocol_Heater_Reduce_Power			= 0x04;			//热水器减少功率 

//下位机热水器通信协议接收到的命令协议数据
volatile uchar Protocol_Heater_Receive_Data      = 0x00;
	

bit UartSendFlag = 0; //发送中断标志位
bit UartReceiveFlag = 0; //接收中断标志位
/*****************************************************
*函数名称：void Uart0_Test(void)
*函数功能：Uart0测试
*入口参数：void
*出口参数：void
*****************************************************/
void Uart0_Test(void)
{
	Uart0_Init();
	UART_SentChar(0x56);
	while(1)
	{
		SBUF = 0x55;
		while(!UartSendFlag);
		UartSendFlag = 0;
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
	P1CON = 0X08;
	P1PH = 0X04;	 //TX为强推输出，RX为带上拉输入；
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
	}	
}

void UART_SentChar(uchar chr)
{
  //发送一个字节
  SBUF = chr;
  while( TI == 0);
  TI = 0;
}

//void UART_SendString(uchar *str)
//{
//  while(*str != '\0')
//  {
//      UART_SentChar(*str++);
//  }
//}

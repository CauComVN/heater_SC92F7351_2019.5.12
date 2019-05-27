//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: main.c
//	模块功能	: 主函数
//  说明        : 发送字符串接收字符串示例,将一定时间内接收到的字符串发出
//************************************************************

#include "SC92F8003_C.H"
#include "stdio.h"
#include "Uart.H"
#include "TIMER0.h"

int main(void)
{	
	unsigned char i;
	Timer0Init();
	Uart0Init(115200);
	EA = 1;
	printf("**************************************\n");
	printf("   Welcome to use SinOne Chip!		  \n");
	printf("**************************************\n\n");
	while(1)
	{
		 if(Uart0BuffNumber==1) //接收到第一个数据就开启定时器
		 {
		 	TR0 = 1;	 //开启定时器
		 }
		 if(Timer0Flag)	 //接收时间到后将接收的字符串发出
		 {
		 	TR0 = 0;	//关闭定时器
			TL0 = TIM0InitialValue % 256;
			TH0 = TIM0InitialValue / 256;
			Timer0Flag = 0;
			Uart_SendString(Uart0Buff,Uart0BuffNumber);	//发送字符串
			Uart0BuffNumber = 0;//发送完毕后将接收编号清0准备下一次的接收
			for(i=0;i<UART0_BUFF_LENGTH;i++)	//清空接收缓存准备下一次的接收
			{
				Uart0Buff[i] = 0;
			}
		 }
	}

}



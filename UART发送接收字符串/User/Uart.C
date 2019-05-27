//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: Uart.c
//	模块功能	: 串口控制程序
//  说明        : 发送字符串接收字符串示例
//************************************************************

#include "SC92F8003_C.H"
#include "Uart.H"
#include "stdio.h"

bit UartReceFlag = 0;
bit UartSendFlag = 0;
unsigned char Uart0Buff[UART0_BUFF_LENGTH];
unsigned char Uart0BuffNumber = 0;

char putchar(char c)//用于重写printf
{
	SBUF = c;
	while(!UartSendFlag);
	UartSendFlag=0;
	return c;
}

void Uart0Init(unsigned long BAUD)
{
	P1PH= 0x60;	   //通讯口上拉
	SCON = 0x50;	
	REN = 1;
	EUART = 1;
	IP |= 0X10;
	//定时器1波特率发生器  
	TMOD = 0X10;    //设定T1定时器工作方式1
	TMCON |= 0X02; 
	TH1 = (FSOC/BAUD) / 256;
	TL1 = (FSOC/BAUD) % 256;
	TR1=0;
		
}

void Uart_SendString(unsigned char *str,unsigned char strlen)
{
    unsigned char i; 
	for(i=0;i<strlen;i++)
	{
	   SBUF = *(str++);
	   while(!UartSendFlag);
	   UartSendFlag=0;
	} 
}

void Uart0Work() interrupt 4
{	
	if(TI==1)
	{
	   TI = 0;
	   UartSendFlag=1;
	}
	
	if(RI==1)
	{
		RI = 0;
		UartReceFlag = 1;
		Uart0Buff[Uart0BuffNumber] = SBUF; //将接收的数据存入缓冲区
		Uart0BuffNumber++;
	}
		
}
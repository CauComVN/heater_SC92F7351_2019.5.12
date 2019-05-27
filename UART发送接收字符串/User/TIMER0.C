//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: TIMER0.c
//	模块功能	: 定时器控制程序
//  说明        : 
//************************************************************
#include "SC92F8003_C.H"
#include "TIMER0.h"

bit Timer0Flag = 0;
unsigned char Timer0Buff = 0;

void Timer0Init()
{
	TMCON |= 0X01;
	TMOD &= 0XFC;					
	TMOD |= 0X01;
	TL0 = TIM0InitialValue % 256;
	TH0 = TIM0InitialValue / 256;
	ET0 = 1;
}

void Timer0Interrupt()		interrupt 1			   
{
	TL0 = TIM0InitialValue % 256;
	TH0 = TIM0InitialValue / 256;
	if(Timer0Buff>=SC_WorkTime)
	{
		Timer0Buff = 0;
		Timer0Flag = 1;//置起标志，表示时间到了
	}
	else
	{
		Timer0Buff++;
	}
	
}
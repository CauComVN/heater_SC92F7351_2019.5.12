//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: TIMER0.H
//	模块功能	: 定时器控制程序
//  说明        : 
//************************************************************
#ifndef _TIMER0_H_
#define	_TIMER0_H_

#define	SC_SystemClock	      16//系统主频16M
#define SC_SmallestTime     1000//最小时间片时间长度（单位us）
#define SC_WorkTime			  30//定时时间（单位ms）
#define TIM0InitialValue    (65536-SC_SmallestTime*SC_SystemClock)  //主频16M模式下，TIM0初值设置，Timer0工作模式1，初值为49536，定时1ms

extern bit Timer0Flag;
void Timer0Init();

#endif
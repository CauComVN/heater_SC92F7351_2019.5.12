#include "Function_Init.H"
#include "SC92F735x_C.H"
/*****************************************************
*函数名称：void IO_Init(void)
*函数功能：IO初始化
*入口参数：void
*出口参数：void
*****************************************************/
void IO_Init(void)
{	
	//SC92F7352引脚定义
	P0CON = 0xFF;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0x00;  //设置P2为高阻带上拉模式
	P2PH  = 0xFF;
	SC92F7351_NIO_Init(); //未引出IO口配置		
}
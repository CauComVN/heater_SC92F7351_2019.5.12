#include "Function_Init.H"
#include "MATH.H"

//Water Detection

//霍尔水流传感器->外部中断计数，内部计时器定时
//INT25 P21

unsigned int number = 0;   //记录触发中断个数

void Water_Detection_EX_Init(void);
void Water_Detection_EXTI_Test(void);
void Water_Detection_Timer_Test(void);
void Water_Detection_EX2_Handle();
void Water_Detection_Timer0_Handle();

uchar Water_Detection_INT2_Flag = 0x00;
uchar water_flow_flag=0x00;
/*****************************************************
*函数名称：void Water_Detection_EXTI_Test(void)
*函数功能：外部中断测试
*入口参数：void
*出口参数：void
*****************************************************/
void Water_Detection_EXTI_Test(void)
{
    Water_Detection_EX_Init();
    while(1)
    {
    }
}
/*****************************************************
*函数名称：void Water_Detection_EX_Init(void)
*函数功能：外部中断初始化
*入口参数：void
*出口参数：void
*****************************************************/
void Water_Detection_EX_Init(void)
{
    //配置中断口25
    P2CON &= 0XFD;     //中断IO口设置为高阻输入
    P2PH  |= 0x02;     //中断IO口设置为高阻带上拉

    //配置INT25下降沿中断
    //下降沿设置
    INT2F = 0X00 ;    //0000 xxxx  0关闭 1使能
    //上升沿设置
    INT2R = 0X20 ;    //0000 xxxx  0关闭 1使能

    //外部中断优先级设置
    IE1 |= 0x08;	//0000 x000  INT2使能
    IP1 |= 0X00;
    EA = 1;
}

void Water_Detection_EX2_Handle()
{
    IE1 &= 0xf7;	//0000 x000  INT2使关闭 关闭霍尔水流传感器->外部中断

    //中断2有两路输入，需要区分 ？？？
    number++; //霍尔水流传感器->外部中断计数

    //如果中断2有两路输入，根据上升沿或者下降沿来确认，这里是下降沿
    if(P20 == 0)
    {
        Water_Detection_INT2_Flag = 0x10; //INT12产生中断
    }
    if(P21 == 0) //INT25 P21 水流检测计数
    {
        Water_Detection_INT2_Flag = 0x20; //INT13产生中断
    }

    IE1 |= 0x08;	//0000 x000  INT2使能
}

//////////////////////////////////////////////////////////////
void Water_Detection_Timer_Init(void);
/*****************************************************
*函数名称：void Water_Detection_Timer_Test(void);
*函数功能：T0/T1/T2测试
*入口参数：void
*出口参数：void
*****************************************************/
void Water_Detection_Timer_Test(void)
{
    Water_Detection_Timer_Init();
    while(1)
    {
    }
}
/*****************************************************
*函数名称：void Water_Detection_Timer_Init(void)
*函数功能：T0/T1/T2初始化
*入口参数：void
*出口参数：void
*****************************************************/
void Water_Detection_Timer_Init(void)
{
    water_flow_flag = 0; //无水流

    TMCON = 0X01;    //------001 ;Timer0选择时钟Fsys

    //T0设置
    TMOD |= 0x01;                 //0000 0001;Timer0设置工作方式1
    TL0 = (65536 - 24000)%256;    //溢出时间：时钟为Fsys，则24000*（1/Fsys）=1ms;
    TH0 = (65536 - 24000)/256;

//	TL0 = (65536 - 48000)%256;    //溢出时间：时钟为Fsys，则48000*（1/Fsys）=2ms;
//  TH0 = (65536 - 48000)/256;
//
    TR0 = 0;
    ET0 = 1;//定时器0允许
    TR0 = 1;//打开定时器0

    EA = 1;
}

void Water_Detection_Timer0_Handle()
{
    //    TL0 = (65536 - 24000)%256;
//	TH0 = (65536 - 24000)/256;
//	P02 = ~P02;

    //根据仿真确认阈值
    unsigned int threshold=100; //??????

    //定时到，关闭中断，统计霍尔水流传感器->外部中断计数，分析水流 ???
    EA = 0;

    IE1 &= 0xf7;	//0000 x000  INT2使关闭 关闭霍尔水流传感器->外部中断
    TR0=0; //关闭定时器0

    if(number > threshold) {
        water_flow_flag = 1; //有水流
    }
    else {
        water_flow_flag = 0; //无水流
    }

    //霍尔水流传感器->外部中断计数清零
    number=0;
}

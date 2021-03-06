//Zero Crossing detection
//SCR Driver

#include "Function_Init.H"

//INT24 P20

//HEAT TRA PWM1 P01

//HEAT RLY 继电器控制 P02

//T/S1 温度保险 HEAT ERROR COM3/P03
//如果检测到温度过高，T/S1会自动跳闸，断开电路 
//继电器停止工作 保护整个可控硅驱动电路
//HEAT ERROR 为输入端，如果等于高电平，表明热水器温度过高
//如果为低电平，表明热水器温度在正常范围内

//当前热水器运行或停止状态 控制继电器动作 0：停止 1：运行
bit heater_relay_on=0;

//热水器内部异常状态
Enum_Heater_Exception_Flag Heater_Exception_Flag;

//35度~60度 自动调节  最佳：40 - 55
uint good_temperature_out_low=40;
uint good_temperature_out_high=55;

void Zero_Crossing_EXTI_Test(void);
void Zero_Crossing_EX_Init(void);
void Zero_Crossing_EX2_Handle();

void Scr_Driver_PWM_Init(void);
void Scr_Driver_PWMInt_Handle();
int Scr_Driver_Check_Heat_Error();//检测温度保险
void Scr_Driver_Control_Heat_RLY(int on);//继电器控制 HEAT RLY P02

//HEAT TRA PWM1 功率调节方式 flag 0:不用调节 1：增加功率 Duty增大 2：减少功率 Duty减少
void Scr_Driver_PWM_Adjust(uint flag);

uchar Zero_Crossing_INT1_flag = 0x00;
/*****************************************************
*函数名称：void EXTI_Test(void)
*函数功能：外部中断测试
*入口参数：void
*出口参数：void
*****************************************************/
void Zero_Crossing_EXTI_Test(void)
{
    Zero_Crossing_EX_Init();
    while(1)
    {
    }
}
/*****************************************************
*函数名称：void EX_Init(void)
*函数功能：外部中断初始化
*入口参数：void
*出口参数：void
*****************************************************/
void Zero_Crossing_EX_Init(void)
{
    //配置中断口INT24
    P2CON &= 0XFE;     //中断IO口设置为高阻输入
    P2PH  |= 0x01;     //中断IO口设置为高阻带上拉

    //INT24上升中断
    //下降沿设置
    INT2F &= 0x2F; //= 0X00 ;    //0000 xxxx  0关闭 1使能
    //上升沿设置
    INT2R |= 0X10 ;    //0000 xxxx  0关闭 1使能

    //外部中断优先级设�
    IE1 |= 0x08;	//0000 x000  INT2使能
    IP1 |= 0X00;
    EA = 1;
}

void Zero_Crossing_EX2_Handle()
{
	
    //如果中断2有两路输入，根据上升沿或者下降沿来确认，上升沿中断，所以端口电平是1
    //if(P20 == 1) //INT24 P20 过零检测到零点
    {
        //PWM计数值重置
        Scr_Driver_PWM_Init();
    }
 /*   if(P21 == 1) //INT25 P21 水流检测计数
    {

    }
	*/
}

//HEAT TRA PWM1
void Scr_Driver_PWM_Init(void)
{
	/**/
    //50hz 20ms 半个周期10ms 10ms/10=1ms PWM时钟为Fsys/128 187*128/24=997.33 187-1=186
    PWMCON  = 0x16;		//PWM输出到IO，PWM时钟为Fsys/128 HEAT TRA PWM1
    PWMPRD  = 186;		//PWM周期=(186+1)*(1*128/24us)=997.33≈=1ms;
    PWMCFG  = 0x10;		//PWM1输出反向,输出至P01
    PWMDTY1 = 15;     //PWM1的Duty = 15/60 =1/4
    PWMDTYA = 0x00;		//PWM占空比微调寄存器，这里不微调
    PWMCON |= 0x80;     //开启PWM
    IE1 |= 0x02;        //开启PWM中断
    EA = 1;
	
}

//HEAT TRA PWM1 功率调节方式 flag 0:不用调节 1：增加功率 Duty增大 2：减少功率 Duty减少
void Scr_Driver_PWM_Adjust(uint flag)
{
	if(flag==1 || flag==2)
	{
		EA=0;
		IE1 &= 0xfd;        //关闭PWM中断		
		
		if(flag==1){ //增加功率
			PWMDTY1=PWMDTY1+3;
			if(PWMDTY1>59)
			{
				PWMDTY1=59;
			}
		}
		else if(flag==2)
		{
			PWMDTY1=PWMDTY1-3;
			if(PWMDTY1<1)
			{				
				PWMDTY1=1;
			}
		}			
		
		IE1 |= 0x02;        //开启PWM中断
		EA=1;
	}
}

void Scr_Driver_PWMInt_Handle()
{
    //根据出水/进水温度自动调节PWM1的Duty
	int i=0;
}

//检测温度保险 HEAT ERROR 直接检测端口值 P03   轮询方式
int Scr_Driver_Check_Heat_Error()
{	
	if(P03==0)
	{
		//温度正常范围内，温度保险不跳闸
		return 0;
	}
	else if(P03==1)
	{
		//温度异常范围内，温度保险已跳闸
		return -1;
	}
	return -1;
}

//继电器控制 HEAT RLY P02
void Scr_Driver_Control_Heat_RLY(int on)
{
	P0VO = P0VO&0xfb; //P02端口设置成普通I/O口  1111 1011
	if(on == 1)
	{
		P02=1;
	}
	else
	{
		P02=0;
	}
}

#include "Function_Init.H"

/**************************************************************
说明：
1、Options for Target‘Target1’：BL51 Locate->Code Range:0x100，烧录选项请选择DISRST，复位脚作为普通IO使用；
2、改变TEST的定义，可以分别测试对应的功能；
3、注意：先在Function.H里面选择测试型号（SC92F7320无LCD/LED和PWM功能）
***************************************************************/
#define Test  6  //0:进水/出水温度 1:水流检测 2：漏电检测 3：水流温度检测定时器 4：过零检测 5：检测温度保险 6:继电器控制


void main(void)
{
	int ret=0;
	int relay_on=1; //继电器动作，热水器开始工作
	WDTCON |= 0x10;		    //清看门狗
	switch(Test)
	{
		case 0:
			ADC_Test();
			break;
		case 1:
			Water_Detection_EXTI_Test();
			break;
		case 2:
			Leakage_EXTI_Test();
			break;
		case 3:
			Water_Detection_Timer_Test();
			break;
		case 4:
			Zero_Crossing_EXTI_Test();
			break;
		case 5:
			ret=Scr_Driver_Check_Heat_Error();
			break;
		case 6:
			Scr_Driver_Control_Heat_RLY(relay_on);//继电器控制 HEAT RLY P02
			break;
		default:
			break;
	}
}
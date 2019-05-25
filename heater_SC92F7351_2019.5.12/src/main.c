#include "Function_Init.H"

/**************************************************************
说明：
1、Options for Target‘Target1’：BL51 Locate->Code Range:0x100，烧录选项请选择DISRST，复位脚作为普通IO使用；
2、改变TEST的定义，可以分别测试对应的功能；
3、注意：先在Function.H里面选择测试型号（SC92F7320无LCD/LED和PWM功能）
***************************************************************/
//int AppStatus=-1;  //1: 正常运行 -1:进水/出水温度 -2:有水流检测 -3：漏电检测 -4：水流温度检测定时器 -5：过零检测 -6：检测温度保险 -7:继电器控制

//启动或停止热水器，主控板通过串口发送指令
int Heater_Control_Flag=1;
//当前热水器运行或停止状态 0：停止 1：运行
int Heater_Current_Flag=0;

volatile Enum_Heater_Exception_Flag Heater_Exception_Flag;


//启动热水器
int Start_Heater();
//停止热水器
int Stop_Heater();

void main(void)
{	
		int AppStatus=1;  //1: 正常运行 -1:进水/出水温度 -2:有水流检测 -3：漏电检测 -4：水流温度检测定时器 -5：过零检测 -6：检测温度保险 -7:继电器控制

    int ret=0;
    int test_relay_on=1; //继电器动作，热水器开始工作
	  int relay_on=1;///开启热水器
    
	
		IO_Init();
		WDTCON |= 0x10;		    //清看门狗

    //测试。。。
    if(AppStatus<0) {
        switch(AppStatus)
        {
        case -1:
            ADC_Test();
            break;
        case -2:
            Water_Detection_EXTI_Test();
            break;
        case -3:
            Leakage_EXTI_Test();
            break;
        case -4:
            Water_Detection_Timer_Test();
            break;
        case -5:
            Zero_Crossing_EXTI_Test();
            break;
        case -6:
            ret=Scr_Driver_Check_Heat_Error();
            break;
        case -7:
            Scr_Driver_Control_Heat_RLY(test_relay_on);//继电器控制 HEAT RLY P02
            break;
				case -8:
					Scr_Driver_PWM_Init();
        default:
            break;
        }
    }

    //正常运行。。。
    if(AppStatus==1)
    {
			Scr_Driver_Control_Heat_RLY(relay_on);
			/**/
        //漏电检测
        Leakage_EX_Init();
        while(1)
        {
            Start_Heater();
            Stop_Heater();

            //主控发送启动热水器指令
            if(Heater_Control_Flag==1 && Heater_Current_Flag==0)
            {
                Start_Heater();
                Heater_Current_Flag=1;
            }
            //主控发送停止热水器指令
            if(Heater_Control_Flag==0 && Heater_Current_Flag==1)
            {
                Stop_Heater();
                Heater_Current_Flag=0;
            }
            //热水器内部异常，将异常标志发送到主控处理
            if(Heater_Exception_Flag>Heater_Exception_Normal)
            {
                //停止热水器
                Stop_Heater();
                Heater_Current_Flag=0;

                //发送热水器内部异常标志
                //??????

                //漏电检测异常
                if(Heater_Exception_Flag==Heater_Exception_Leakage) {
                }
            }
        }
				
    }

}


//启动热水器
int Start_Heater()
{
    return 0;
}

//停止热水器
int Stop_Heater()
{
    return 0;
}
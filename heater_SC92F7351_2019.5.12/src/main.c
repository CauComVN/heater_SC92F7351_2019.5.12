#include "Function_Init.H"

/**************************************************************
说明：
1、Options for Target‘Target1’：BL51 Locate->Code Range:0x100，烧录选项请选择DISRST，复位脚作为普通IO使用；
2、改变TEST的定义，可以分别测试对应的功能；
3、注意：先在Function.H里面选择测试型号（SC92F7320无LCD/LED和PWM功能）
***************************************************************/

void main(void)
{
    int AppStatus=1;  //1: 正常运行 -1:进水/出水温度 -2:有水流检测 -3：漏电检测 -4：水流温度检测定时器 -5：过零检测 -6：检测温度保险 -7:继电器控制 -8：PWM测试 -9：串口发送

    int ret=0;
    int test_relay_on=1; //继电器动作，热水器开始工作
	
		uint nADWtrTempValue=0;
		int water_temperature_in=26;
		int water_temperature_out=26;
		

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
            break;
        case -9:
            Uart0_Test();
            break;
        default:
            break;
        }
    }

    //正常运行。。。
    if(AppStatus==1)
    {   /**/

        //漏电检测
        Leakage_EX_Init();
			
				//水流检测
				Water_Detection_EX_Init();
				//水流检测定时器
				Water_Detection_Timer_Init();
			
				//测试。。。 注意：一定要先进水，防止干烧**************
				//Protocol_Heater_Receive_Data = Protocol_Heater_Start;
			
        while(1)
        {
            //主控启动热水器
            if(Protocol_Heater_Receive_Data == Protocol_Heater_Start
                    && heater_relay_on==0) {
                heater_relay_on=1;
                Scr_Driver_Control_Heat_RLY(heater_relay_on);
            }

            //主控发送停止热水器指令
            if(Protocol_Heater_Receive_Data == Protocol_Heater_Stop && heater_relay_on==1)
            {
                heater_relay_on=0;
                Scr_Driver_Control_Heat_RLY(heater_relay_on);
            }
						
						//进水温度
						ADC_Init(AIN9);
						nADWtrTempValue=ADC_Convert(); //启动ADC转换，获得转换值
						ret = get_temperature_from_table(nADWtrTempValue,&water_temperature_in);
						
						if(ret==-1){ //通知检测温度异常，超过最低温度，发送主板BEEP报警
							Heater_Exception_Flag=Heater_Ex_In_Water_Temp_Low;
						}
						else if(ret==-2){ //通知检测温度异常，超过最高温度发送主板BEEP报警
							Heater_Exception_Flag=Heater_Ex_In_Water_Temp_High;
						}
						else{
							//35度~60度 自动调节
						}

						//出水温度
						ADC_Init(AIN8);
						nADWtrTempValue=ADC_Convert(); //启动ADC转换，获得转换值
						ret = get_temperature_from_table(nADWtrTempValue,&water_temperature_out);
						
						if(ret==-1){ //通知检测温度异常，超过最低温度，发送主板BEEP报警
							Heater_Exception_Flag=Heater_Ex_Out_Water_Temp_Low;
						}
						else if(ret==-2){ //通知检测温度异常，超过最高温度发送主板BEEP报警
							Heater_Exception_Flag=Heater_Ex_Out_Water_Temp_High;
						}
						else{
							//35度~60度 自动调节
						}

            //热水器内部异常，将异常标志发送到主控处理
            if(Heater_Exception_Flag>Heater_Ex_Normal)
            {
                //漏电检测异常
                if(Heater_Exception_Flag==Heater_Ex_Leakage) {
                }
            }
        }
    }
}

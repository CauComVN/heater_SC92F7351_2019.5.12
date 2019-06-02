#include "Function_Init.H"

/**************************************************************
说明：
1、Options for Target‘Target1’：BL51 Locate->Code Range:0x100，烧录选项请选择DISRST，复位脚作为普通IO使用；
2、改变TEST的定义，可以分别测试对应的功能；
3、注意：先在Function.H里面选择测试型号（SC92F7320无LCD/LED和PWM功能）
***************************************************************/

void main(void)
{
	int AppStatus=3;  //3：PWM测试 2:调节可控硅控制功率调节灯泡亮度  1: 正常运行 -1:进水/出水温度 -2:有水流检测 -3：漏电检测 -4：水流温度检测定时器 -5：过零检测 -6：检测温度保险 -7:继电器控制 -8：PWM测试 -9：串口发送接收

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
    {

        //漏电检测
//        Leakage_EX_Init();

        //水流检测
        Water_Detection_EX_Init();
        //水流检测定时器
        Water_Detection_Timer_Init();

//        //启动可控硅控制
//        Zero_Crossing_EX_Init();

        //串口通信初始化
//        Uart0_Init();

        //测试。。。 注意：一定要先进水，防止干烧**************
        Protocol_Heater_Receive_Data = Protocol_Heater_Start;

        while(1)
        {
            //实时监控水流，一定要多水流，防止干烧，危险。。。
//            if(water_flow_flag != 2)
//						{ //水流状态标记 0：无水流 1：少水流 2：多水流，正常
//
//								heater_relay_on=0;
//								Scr_Driver_Control_Heat_RLY(heater_relay_on);
//						}

            //主控启动热水器
            if(Protocol_Heater_Receive_Data == Protocol_Heater_Start
                    && heater_relay_on==0)
            {
//#ifdef SERIAL_TEST
//                UART_SentChar(Protocol_Heater_Receive_Data+0x20);
//#endif
                //if(water_flow_flag == 2)
                {
                    //水流状态标记 0：无水流 1：少水流 2：多水流，正常
                    heater_relay_on=1;
                    Scr_Driver_Control_Heat_RLY(heater_relay_on);

                    //主控发送指令已处理，串口接收数据设置成默认状态
                    Protocol_Heater_Receive_Data=Protocol_Heater_Default;
									
									Zero_Crossing_EX_Init();

										
                }
            }

            //主控发送停止热水器指令
//            if(Protocol_Heater_Receive_Data == Protocol_Heater_Stop && heater_relay_on==1)
//            {
////#ifdef SERIAL_TEST
////                UART_SentChar(Protocol_Heater_Receive_Data+0x20);
////#endif
//                heater_relay_on=0;
//                Scr_Driver_Control_Heat_RLY(heater_relay_on);

//                //主控发送指令已处理，串口接收数据设置成默认状态
////                Protocol_Heater_Receive_Data=Protocol_Heater_Default;
//            }

//            if(Protocol_Heater_Receive_Data == Protocol_Heater_Increases_Power && heater_relay_on==1)
//            {
//#ifdef SERIAL_TEST
//                UART_SentChar(Protocol_Heater_Receive_Data+0x20);
//#endif
//                //调高功率
//                Scr_Driver_PWM_Adjust(1); //flag=1
//            }
//            else if(Protocol_Heater_Receive_Data == Protocol_Heater_Reduce_Power && heater_relay_on==1)
//            {
//#ifdef SERIAL_TEST
//                UART_SentChar(Protocol_Heater_Receive_Data+0x20);
//#endif
//                //调低功率
//                Scr_Driver_PWM_Adjust(2); //flag=2
//            }

            //进水温度
            ADC_Init(AIN9);
            nADWtrTempValue=ADC_Convert(); //启动ADC转换，获得转换值
            ret = get_temperature_from_table(nADWtrTempValue,&water_temperature_in);

            if(ret==-1) { //通知检测温度异常，超过最低温度，发送主板BEEP报警
                Heater_Exception_Flag=Heater_Ex_In_Water_Temp_Low;
            }
            else if(ret==-2) { //通知检测温度异常，超过最高温度发送主板BEEP报警
                Heater_Exception_Flag=Heater_Ex_In_Water_Temp_High;
            }
            else {
                //35度~60度 自动调节
            }

            //出水温度
            ADC_Init(AIN8);
            nADWtrTempValue=ADC_Convert(); //启动ADC转换，获得转换值
            ret = get_temperature_from_table(nADWtrTempValue,&water_temperature_out);

            if(ret==-1) { //通知检测温度异常，超过最低温度，发送主板BEEP报警
                Heater_Exception_Flag=Heater_Ex_Out_Water_Temp_Low;
            }
            else if(ret==-2) { //通知检测温度异常，超过最高温度发送主板BEEP报警
                Heater_Exception_Flag=Heater_Ex_Out_Water_Temp_High;
            }
            else {
//                //35度~60度 自动调节
//                if(water_temperature_out<good_temperature_out_low && heater_relay_on==1)
//                {
//                    //调高功率
//                    Scr_Driver_PWM_Adjust(1); //flag=1
//                }
//                else if(water_temperature_out>good_temperature_out_high  && heater_relay_on==1)
//                {
//                    //调低功率
//                    Scr_Driver_PWM_Adjust(2); //flag=2
//                }
//                else {
//                }
            }

            //检测温度保险 HEAT ERROR 直接检测端口值 P03   轮询方式
//            ret=Scr_Driver_Check_Heat_Error();
//            if(ret==-1 && heater_relay_on==1)
//            {
//                Heater_Exception_Flag=Heater_Ex_Thermal_Switch_Error;

//                heater_relay_on=0;
//                Scr_Driver_Control_Heat_RLY(heater_relay_on);
//            }

//            //热水器内部异常，将异常标志发送到主控处理
//            if(Heater_Exception_Flag>Heater_Ex_Normal)
//            {
//                //漏电检测异常
//                if(Heater_Exception_Flag==Heater_Ex_Leakage) {
//                }
//                //无水流或少水流
//                if(Heater_Exception_Flag == Heater_Ex_Water_No_Flow
//                        || Heater_Exception_Flag == Heater_Ex_Water_Little_Flow)
//                {
//                    //停止热水器
//										if(heater_relay_on == 1)
//										{
//											heater_relay_on=0;
//											Scr_Driver_Control_Heat_RLY(heater_relay_on);
//										}
//                }
//            }
        }
    }
		
		if(AppStatus==2)
		{
			///用串口P12 P13模拟控制功率
			////////////////////////////////////////////////////////////
			//配置中断口INT00 P10
			P1CON &= 0XF3;     //中断IO口设置为高阻输入
			P1PH  |= 0x0c;     //中断IO口设置为高阻带上拉
		 
			//配置INT00上升沿中断
				//下降沿设置	
			INT0F |= 0x0c;//= 0X00 ;    //xxxx 0000  0关闭 1使能
				//上升沿设置	
			INT0R &= 0Xf3;    //xxxx 0000  0关闭 1使能 
			
			//外部中断优先级设置
			IE  |= 0x01;	//0000 0x0x  INT0使能
			IP  |= 0X00;
			
			EA = 1;
			//////////////////////////////////////////////////////////
			while(1)
			{
			}
		}
		if(AppStatus==3)
		{
			Scr_Driver_PWM_Init();
			while(1)
			{					
			}
		}
}

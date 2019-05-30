#ifndef _Function_Init_H_
#define _Function_Init_H_

#include "SC92F732x_C.H"
#include "intrins.H"
#define uchar unsigned char
#define uint unsigned int

#define IapROM  0x00	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作
#define IapEPPROM  0x02	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作

/***********SC92F7321 ADC采样口选择*************/
enum Channel {AIN0=0,AIN1,AIN2,AIN3,AIN8=8,AIN9,VDD4=15};

//下位机热水器通信协议
enum Protocol_Heater 
{
	Protocol_Heater_Default						= 0x00,			//初始值
	Protocol_Heater_Start						  = 0x01, 		//启动热水器
	Protocol_Heater_Stop  						= 0x02, 		//停止热水器
	Protocol_Heater_Increases_Power   = 0x03,			//热水器增加功率
	Protocol_Heater_Reduce_Power			= 0x04,			//热水器减少功率 
};

//下位机热水器通信协议接收到的命令协议数据
extern uchar Protocol_Heater_Receive_Data;

//当前热水器运行或停止状态 继电器动作 0：停止 1：运行
extern bit heater_relay_on;


//热水器内部异常状态
typedef enum Heater_Exception
{
    Heater_Ex_Normal=0,
    Heater_Ex_Leakage,
    Heater_Ex_In_Water_Temp_Low,
    Heater_Ex_In_Water_Temp_High,
    Heater_Ex_Out_Water_Temp_Low,
    Heater_Ex_Out_Water_Temp_High,
		Heater_Ex_Water_No_Flow,
		Heater_Ex_Water_Little_Flow,
		Heater_Ex_Thermal_Switch_Error,
} Enum_Heater_Exception_Flag;

//热水器内部异常状态
extern Enum_Heater_Exception_Flag Heater_Exception_Flag;

//水流状态标记 0：无水流 1：少水流 2：多水流，正常
extern int water_flow_flag;

//最佳出水温度低值
extern uint good_temperature_out_low;
//最佳出水温度高值
extern uint good_temperature_out_high;


extern void IO_Init(void);

extern void ADC_Test(void);
extern void ADC_Init(uint Channel);
extern int ADC_Convert(void);
extern void ADC_channel(unsigned char channel);
extern int get_temperature_from_table(uint nADValue, int* temperature);
extern void ADC_Interrupt_Handle(void);

extern void Leakage_EXTI_Test(void);
extern void Leakage_EX_Init(void);
extern void Leakage_EX0_Handle(void);

extern void Water_Detection_EXTI_Test(void);
extern void Water_Detection_Timer_Test(void);
extern void Water_Detection_EX_Init(void);
extern void Water_Detection_Timer_Init(void);
extern void Water_Detection_EX2_Handle();
extern void Water_Detection_Timer0_Handle();

extern void Zero_Crossing_EX_Init(void);
extern void Zero_Crossing_EXTI_Test(void);
extern void Zero_Crossing_EX2_Handle();

extern void Scr_Driver_PWM_Init(void);
extern void Scr_Driver_PWMInt_Handle();
extern int Scr_Driver_Check_Heat_Error();//检测温度保险
extern void Scr_Driver_Control_Heat_RLY(int on);//继电器控制 HEAT RLY P02

//HEAT TRA PWM1 功率调节方式 flag 0:不用调节 1：增加功率 Duty增大 2：减少功率 Duty减少
extern void Scr_Driver_PWM_Adjust(uint flag);

extern void Uart0_Init(void);
extern void Uart0_Test(void);
extern void UART_SentChar(uchar chr);

//串口调试通信协议开关
//#define SERIAL_TEST 

/*****************************************
说明：选择测试IC
*****************************************/
#define SC92F7323  0
#define SC92F7322  1
#define SC92F7321  2
#define SC92F7320  3

#define IC_MODEL  SC92F7321

#endif
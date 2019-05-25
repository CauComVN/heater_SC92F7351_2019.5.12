#ifndef _Function_Init_H_
#define _Function_Init_H_

#include "SC92F732x_C.H"
#include "intrins.H"
#define uchar unsigned char
#define uint unsigned int

#define IapROM  0x00	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作
#define IapEPPROM  0x02	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作

//热水器内部异常状态
typedef enum Heater_Exception
{
	Heater_Exception_Normal=0,
	Heater_Exception_Leakage,
	
}Enum_Heater_Exception_Flag;

extern volatile Enum_Heater_Exception_Flag Heater_Exception_Flag;

extern void IO_Init(void);

extern void ADC_Test(void);
extern int ADC_Convert(void);
extern void ADC_channel(unsigned char channel);
extern void ADC_Interrupt_Handle(void);

extern void Leakage_EXTI_Test(void);
extern void Leakage_EX_Init(void);
extern void Leakage_EX0_Handle(void);

extern void Water_Detection_EXTI_Test(void);
extern void Water_Detection_Timer_Test(void);
extern void Water_Detection_EX2_Handle();
extern void Water_Detection_Timer0_Handle();

extern void Zero_Crossing_EXTI_Test(void);
extern void Zero_Crossing_EX2_Handle();

extern void Scr_Driver_PWM_Init(void);
extern void Scr_Driver_PWMInt_Handle();
extern int Scr_Driver_Check_Heat_Error();//检测温度保险
extern void Scr_Driver_Control_Heat_RLY(int on);//继电器控制 HEAT RLY P02

extern void Uart0_Init(void);
extern void Uart0_Test(void);

/*****************************************
说明：选择测试IC
*****************************************/
#define SC92F7323  0
#define SC92F7322  1
#define SC92F7321  2
#define SC92F7320  3

#define IC_MODEL  SC92F7321

#endif
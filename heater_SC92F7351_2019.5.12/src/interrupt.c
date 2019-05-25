#include "Function_Init.H"

void ADC_Interrupt(void) interrupt 6
{
    ADC_Interrupt_Handle();
}

void EX0_Interrupt() interrupt	0
{
    Leakage_EX0_Handle();
}

void EX2_Interrupt() interrupt	10
{
	static int s_flag=0;
	if(s_flag==0){
	Uart0_Test();
		s_flag=1;
	}
	
	if(1){
	if(!(P2CON&0x02) && (P21==1) ){		
		//INT25 �˿�P21 �������ж� P21=1
    Water_Detection_EX2_Handle();
	}
	if(!(P2CON&0x01) && (P20==1)){	
	//INT24 �˿�P20 �������ж� P20=1
    Zero_Crossing_EX2_Handle();
	}
}
	
	
}

void Timer0_Interrupt() interrupt 1
{
    Water_Detection_Timer0_Handle();
}

void PWMInt(void) interrupt 8
{	
		Scr_Driver_PWMInt_Handle();
}
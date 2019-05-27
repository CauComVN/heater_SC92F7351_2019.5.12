//************************************************************
//  Copyright (c) ��������Ԫ΢�������޹�˾
//	�ļ�����	: main.c
//	ģ�鹦��	: ������
//  ˵��        : �����ַ��������ַ���ʾ��,��һ��ʱ���ڽ��յ����ַ�������
//************************************************************

#include "SC92F8003_C.H"
#include "stdio.h"
#include "Uart.H"
#include "TIMER0.h"

int main(void)
{	
	unsigned char i;
	Timer0Init();
	Uart0Init(115200);
	EA = 1;
	printf("**************************************\n");
	printf("   Welcome to use SinOne Chip!		  \n");
	printf("**************************************\n\n");
	while(1)
	{
		 if(Uart0BuffNumber==1) //���յ���һ�����ݾͿ�����ʱ��
		 {
		 	TR0 = 1;	 //������ʱ��
		 }
		 if(Timer0Flag)	 //����ʱ�䵽�󽫽��յ��ַ�������
		 {
		 	TR0 = 0;	//�رն�ʱ��
			TL0 = TIM0InitialValue % 256;
			TH0 = TIM0InitialValue / 256;
			Timer0Flag = 0;
			Uart_SendString(Uart0Buff,Uart0BuffNumber);	//�����ַ���
			Uart0BuffNumber = 0;//������Ϻ󽫽��ձ����0׼����һ�εĽ���
			for(i=0;i<UART0_BUFF_LENGTH;i++)	//��ս��ջ���׼����һ�εĽ���
			{
				Uart0Buff[i] = 0;
			}
		 }
	}

}



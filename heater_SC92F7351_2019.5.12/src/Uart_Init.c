#include "Function_Init.H" 

void Uart0_Init(void);
void Uart0_Test(void);
void UART_SentChar(uchar chr);

//��λ����ˮ��ͨ��Э��
uchar Protocol_Heater_Default						= 0x00;			//��ʼֵ
uchar Protocol_Heater_Start						  = 0x01; 		//������ˮ��
uchar Protocol_Heater_Stop  						= 0x02; 		//ֹͣ��ˮ��
uchar Protocol_Heater_Increases_Power   = 0x03;			//��ˮ�����ӹ���
uchar Protocol_Heater_Reduce_Power			= 0x04;			//��ˮ�����ٹ��� 

//��λ����ˮ��ͨ��Э����յ�������Э������
volatile uchar Protocol_Heater_Receive_Data      = 0x00;
	

bit UartSendFlag = 0; //�����жϱ�־λ
bit UartReceiveFlag = 0; //�����жϱ�־λ
/*****************************************************
*�������ƣ�void Uart0_Test(void)
*�������ܣ�Uart0����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void Uart0_Test(void)
{
	Uart0_Init();
	UART_SentChar(0x56);
	while(1)
	{
		SBUF = 0x55;
		while(!UartSendFlag);
		UartSendFlag = 0;
	}
}
/*****************************************************
*�������ƣ�void Uart0_Init(void)
*�������ܣ�Uart0�жϳ�ʼ��
*��ڲ�����Freq-��Ƶ��baud-������
*���ڲ�����Freq,baud
*****************************************************/
void Uart0_Init(void)    //ѡ��Timer1��Ϊ�������źŷ�����
{
	P1CON = 0X08;
	P1PH = 0X04;	 //TXΪǿ�������RXΪ���������룻
	P13 = 1;		 //TX��ʼ�ߵ�ƽ��
	SCON = 0X50;     //��ʽ1�������������
	PCON |= 0X80; 
	T2CON = 0x00;    //ʹ�ö�ʱ��1��UARTʱ��
	TMOD = 0X20;     //��ʱ��1  8λ�Զ�����
	TMCON = 0X02;    //��ʱ��1   Fsys��
	TL1 = 217;		
	TH1 = 217;		 //UART ������24M�����=38400��
	TR1 = 0;
	ET1 = 1;		 //Timer1ʹ��
	TR1 = 1;		 //����Timer0
	EUART = 1;	     //����UART�ж�
	EA = 1;		     //�����ж�
}

/*
void Uart0_Init(void)    //ѡ��Timer2��Ϊ�������źŷ�����
{
	P1CON = 0X08;
	P1PH = 0X04;	 //TXΪǿ�������RXΪ���������룻
	P13 = 1;
	SCON  = 0X50;    //����ͨ�ŷ�ʽΪģʽһ���������
	PCON |= 0X80; 
	TMCON |= 0X04;
	T2MOD = 0X00;
	T2CON = 0X30;
	RCAP2H = 0xFF;
	RCAP2L = 0xB2;
	TR2 = 0;
	ET2 = 1;
	TR2 = 1;
	EUART = 1;     //����Uart0�ж�
	EA = 1;
}
*/

/*****************************************************
*�������ƣ�void UartInt(void) interrupt 4
*�������ܣ�Uart0�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void UartInt(void) interrupt 4
{
	if(TI)
	{
		TI = 0;	
		UartSendFlag = 1;		
	}
	if(RI)
	{
		RI = 0;	
		UartReceiveFlag = 1;
	}	
}

void UART_SentChar(uchar chr)
{
  //����һ���ֽ�
  SBUF = chr;
  while( TI == 0);
  TI = 0;
}

//void UART_SendString(uchar *str)
//{
//  while(*str != '\0')
//  {
//      UART_SentChar(*str++);
//  }
//}

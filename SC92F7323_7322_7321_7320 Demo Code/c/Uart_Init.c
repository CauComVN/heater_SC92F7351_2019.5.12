#include "H/Function_Init.H" 
#include "stdio.h"

#define UART0_BUFF_LENGTH	  7	//UART0���ܻ�������������Byte) 00001 \0
uchar Uart0BuffNumber=0;
uchar Uart0Buff[UART0_BUFF_LENGTH];
//void Uart_SendString(unsigned char *str,unsigned char strlen);

void UART_SentChar(uchar chr);
void UART_SendString(uchar *str);
	
void Uart0_Init(void);
bit UartSendFlag = 0; //�����жϱ�־λ
bit UartReceiveFlag = 0; //�����жϱ�־λ

char putchar(char c)//������дprintf
{
	SBUF = c;
	while(!UartSendFlag);
	UartSendFlag=0;
	return c;
}

/*****************************************************
*�������ƣ�void Uart0_Test(void)
*�������ܣ�Uart0����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void Uart0_Test(void)
{
	Uart0BuffNumber = 0;
	
	Uart0_Init();
	while(1)
	{
		/*
		SBUF = 0x55;
		while(!UartSendFlag);
		UartSendFlag = 0;
		*/
		
		//Uart_SendString("00001",UART0_BUFF_LENGTH);
		printf("00100\n");
		
		if(Uart0BuffNumber>=(UART0_BUFF_LENGTH-1))				//���ռ���
	  {	    
			//Uart_SendString(Uart0Buff,UART0_BUFF_LENGTH);		
			Uart_SendString(Uart0Buff);
			//�û����������ڼ�ִ����������			
			Uart0BuffNumber=0	;				 //����������ָ��ʼ
	  }
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
		
		Uart0Buff[Uart0BuffNumber] = SBUF; //�����յ����ݴ��뻺����
		Uart0BuffNumber++;
	}	
}


void UART_SentChar(uchar chr)
{
      //����һ���ֽ�
  SBUF = chr;
  while( TI == 0);
  TI = 0;
}
void UART_SendString(uchar *str)
{
  while(*str != '\0')
  {
      UART_SentChar(*str++);
  }
}
//--------------------- 
//���ߣ�qq_41549389 
//��Դ��CSDN 
//ԭ�ģ�https://blog.csdn.net/qq_41549389/article/details/80742032 
//��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�

//void Uart_SendString(unsigned char *str,unsigned char strlen)
//{
//  unsigned char i; 
//	for(i=0;i<strlen;i++)
//	{
//	   SBUF = *(str++);
//	   while(!UartSendFlag);
//	   UartSendFlag=0;
//	} 
//}

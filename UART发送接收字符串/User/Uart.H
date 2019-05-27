#ifndef _UART0_H_
#define	_UART0_H_

#define UART0_BUFF_LENGTH	  30	//UART0接受缓冲区的容量（Byte)
#define	FSOC				  16000000
extern bit UartReceFlag;
extern bit UartSendFlag;
extern unsigned char Uart0BuffNumber;
extern unsigned char Uart0Buff[UART0_BUFF_LENGTH];
void Uart0Init(unsigned long BAUD);
void Uart_SendString(unsigned char *str,unsigned char strlen);

#endif
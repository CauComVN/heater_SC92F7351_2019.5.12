#include "Function_Init.H"
/*****************************************************
*�������ƣ�void IO_Init(void)
*�������ܣ�IO��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/

void IO_Init(void)
{
	#if (IC_MODEL == SC92F7323)  //SC92F7323���Ŷ���
	P0CON = 0x00;  //����P0Ϊ���������ģʽ
	P0PH  = 0xFF;
	P1CON = 0x00;  //����P1Ϊ��������ģʽ
	P1PH  = 0x00;
	P2CON = 0xFF;  //����P2Ϊǿ����ģʽ
	P2PH  = 0x00;
	P5CON = 0xFF;  //����P5Ϊǿ����ģʽ
	P5PH  = 0x00;
	#endif
	
	#if (IC_MODEL == SC92F7322)  //SC92F7323���Ŷ���
	P0CON = 0x00;  //����P0Ϊ���������ģʽ
	P0PH  = 0xFF;
	P1CON = 0x00;  //����P1Ϊ��������ģʽ
	P1PH  = 0x00;
	P2CON = 0xFF;  //����P2Ϊǿ����ģʽ
	P2PH  = 0x00;
	SC92F7322_NIO_Init(); //δ����IO������
	#endif
	
	#if (IC_MODEL == SC92F7321)  //SC92F7321���Ŷ���
	/*
	// Demo code
	P0CON = 0x00;  //����P0Ϊǿ����ģʽ
	P0PH  = 0xFF;
	P1CON = 0x00;  //����P1Ϊ��������ģʽ
	P1PH  = 0x00;
	P2CON = 0xFF;  //����P2Ϊ���������ģʽ
	P2PH  = 0x00;
	SC92F7321_NIO_Init(); //δ����IO������
	*/
	
	// ��ʼ�� �̵����رգ�HEAT RLY P02����ˮ��ֹͣ״̬
	P0CON = P0CON|0x04;  //����P02Ϊǿ����ģʽ
	P0VO = P0VO&0xfb; //P02�˿����ó���ͨI/O��  1111 1011
	P02=0;
	
//	//����¶ȱ��� HEAT ERROR ֱ�Ӽ��˿�ֵ P03   ��ѯ��ʽ
//	P0CON = P0CON&0xf7; //����P03Ϊ����ģʽ
//	P0PH  = P0PH&0xf7;	//��������ģʽ����������ر�
//	P0VO = P0VO&0xf7; //P03�˿����ó���ͨI/O�� ��ѯ��ʽ
	
	#endif
	
	#if (IC_MODEL == SC92F7320)  //SC92F7320���Ŷ���
	P1CON = 0x00;  //����P1Ϊ��������ģʽ
	P1PH  = 0x00;
	P2CON = 0xFF;  //����P2Ϊǿ����ģʽ
	P2PH  = 0x00;
	SC92F7320_NIO_Init(); //δ����IO������
	#endif
}
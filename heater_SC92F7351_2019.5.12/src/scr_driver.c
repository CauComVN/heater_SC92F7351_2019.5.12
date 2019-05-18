//Zero Crossing detection
//SCR Driver

#include "Function_Init.H"

//INT24 P20

void Zero_Crossing_EXTI_Test(void);
void Zero_Crossing_EX_Init(void);
void Zero_Crossing_EX2_Handle();

uchar Zero_Crossing_INT1_flag = 0x00;
/*****************************************************
*�������ƣ�void EXTI_Test(void)
*�������ܣ��ⲿ�жϲ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void Zero_Crossing_EXTI_Test(void)
{
	Zero_Crossing_EX_Init();
	while(1)
	{					
	}
}
/*****************************************************
*�������ƣ�void EX_Init(void)
*�������ܣ��ⲿ�жϳ�ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void Zero_Crossing_EX_Init(void)
{	
	//�����жϿ�INT24
	P2CON &= 0XFE;     //�ж�IO������Ϊ��������
	P2PH  |= 0x01;     //�ж�IO������Ϊ���������
 
	//INT24�����ж�
    //�½�������
    INT2F = 0X00 ;    //0000 xxxx  0�ر� 1ʹ��
    //����������
	INT2R = 0X10 ;    //0000 xxxx  0�ر� 1ʹ��	
	
	//�ⲿ�ж����ȼ����
	IE1 |= 0x08;	//0000 x000  INT2ʹ��
	IP1 |= 0X00;
	EA = 1;
}

void Zero_Crossing_EX2_Handle()
{
	//����ж�2����·���룬���������ػ����½�����ȷ�ϣ��������жϣ����Զ˿ڵ�ƽ��1
    if(P20 == 1)
    {
        
    }
    if(P21 == 1) //INT25 P21 ˮ��������
    {
        
    }
}

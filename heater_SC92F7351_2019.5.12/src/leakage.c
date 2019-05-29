#include "Function_Init.H"
#include "MATH.H"


////////////////////////////////////////////////////////
//�����жϿ�INT00��©�籣��������˿�p1.0

void Leakage_EXTI_Test(void);
void Leakage_EX_Init(void);
void Leakage_EX0_Handle(void);

/*****************************************************
*�������ƣ�void Leakage_EXTI_Test(void)
*�������ܣ��ⲿ�жϲ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void Leakage_EXTI_Test(void)
{
	Leakage_EX_Init();
	while(1)
	{					
	}
}
/*****************************************************
*�������ƣ�void Leakage_EX_Init(void)
*�������ܣ��ⲿ�жϳ�ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void Leakage_EX_Init(void)
{	
	//�����жϿ�INT00 P10
	P1CON &= 0XFE;     //�ж�IO������Ϊ��������
	P1PH  |= 0x01;     //�ж�IO������Ϊ���������
 
	//����INT00�������ж�
    //�½�������	
	INT0F &= 0x0e;//= 0X00 ;    //xxxx 0000  0�ر� 1ʹ��
    //����������	
	INT0R |= 0X01 ;    //xxxx 0000  0�ر� 1ʹ�� 
	
	//�ⲿ�ж����ȼ�����
	IE  |= 0x01;	//0000 0x0x  INT0ʹ��
	IP  |= 0X00;
	
	EA = 1;
}

void Leakage_EX0_Handle(void)
{
	//����INT00�����ش����жϣ���������жϺ���©�����壬P10 == 1��
	//���Ҵ���20ms����©����������
	if(P10 == 1)
	{
		//����©�����쳣��־
		Heater_Exception_Flag = Heater_Ex_Leakage;
	}
}

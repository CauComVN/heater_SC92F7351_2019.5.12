//Zero Crossing detection
//SCR Driver

#include "Function_Init.H"

//INT24 P20

//HEAT TRA PWM1

void Zero_Crossing_EXTI_Test(void);
void Zero_Crossing_EX_Init(void);
void Zero_Crossing_EX2_Handle();

void Scr_Driver_PWM_Init(void);
void Scr_Driver_PWMInt_Handle();

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
    if(P20 == 1) //INT24 P20 �����⵽���
    {
        //PWM����ֵ����
        Scr_Driver_PWM_Init();
    }
    if(P21 == 1) //INT25 P21 ˮ��������
    {

    }
}


void Scr_Driver_PWM_Init(void)
{
    //50hz 20ms �������10ms 10ms/10=1ms PWMʱ��ΪFsys/128 187*128/24=997.33 187-1=186
    PWMCON  = 0x16;		//PWM�����IO��PWMʱ��ΪFsys/128 HEAT TRA PWM1
    PWMPRD  = 186;		//PWM����=(186+1)*(1*128/24us)=997.33��=1ms;
    PWMCFG  = 0x10;		//PWM1�������,�����P01
    PWMDTY1 = 15;     //PWM1��Duty = 15/60 =1/4
    PWMDTYA = 0x00;		//PWMռ�ձ�΢���Ĵ��������ﲻ΢��
    PWMCON |= 0x80;     //����PWM
    IE1 |= 0x02;        //����PWM�ж�
    EA = 1;
}

void Scr_Driver_PWMInt_Handle()
{
    //���ݳ�ˮ/��ˮ�¶��Զ�����PWM1��Duty
}

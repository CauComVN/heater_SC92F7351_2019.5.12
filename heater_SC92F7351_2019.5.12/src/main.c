#include "Function_Init.H"

/**************************************************************
˵����
1��Options for Target��Target1����BL51 Locate->Code Range:0x100����¼ѡ����ѡ��DISRST����λ����Ϊ��ͨIOʹ�ã�
2���ı�TEST�Ķ��壬���Էֱ���Զ�Ӧ�Ĺ��ܣ�
3��ע�⣺����Function.H����ѡ������ͺţ�SC92F7320��LCD/LED��PWM���ܣ�
***************************************************************/
//int AppStatus=-1;  //1: �������� -1:��ˮ/��ˮ�¶� -2:��ˮ����� -3��©���� -4��ˮ���¶ȼ�ⶨʱ�� -5�������� -6������¶ȱ��� -7:�̵�������

//������ֹͣ��ˮ�������ذ�ͨ�����ڷ���ָ��
int Heater_Control_Flag=1;
//��ǰ��ˮ�����л�ֹͣ״̬ 0��ֹͣ 1������
int Heater_Current_Flag=0;

volatile Enum_Heater_Exception_Flag Heater_Exception_Flag;


//������ˮ��
int Start_Heater();
//ֹͣ��ˮ��
int Stop_Heater();

void main(void)
{	
		int AppStatus=1;  //1: �������� -1:��ˮ/��ˮ�¶� -2:��ˮ����� -3��©���� -4��ˮ���¶ȼ�ⶨʱ�� -5�������� -6������¶ȱ��� -7:�̵�������

    int ret=0;
    int test_relay_on=1; //�̵�����������ˮ����ʼ����
	  int relay_on=1;///������ˮ��
    
	
		IO_Init();
		WDTCON |= 0x10;		    //�忴�Ź�

    //���ԡ�����
    if(AppStatus<0) {
        switch(AppStatus)
        {
        case -1:
            ADC_Test();
            break;
        case -2:
            Water_Detection_EXTI_Test();
            break;
        case -3:
            Leakage_EXTI_Test();
            break;
        case -4:
            Water_Detection_Timer_Test();
            break;
        case -5:
            Zero_Crossing_EXTI_Test();
            break;
        case -6:
            ret=Scr_Driver_Check_Heat_Error();
            break;
        case -7:
            Scr_Driver_Control_Heat_RLY(test_relay_on);//�̵������� HEAT RLY P02
            break;
				case -8:
					Scr_Driver_PWM_Init();
        default:
            break;
        }
    }

    //�������С�����
    if(AppStatus==1)
    {
			Scr_Driver_Control_Heat_RLY(relay_on);
			/**/
        //©����
        Leakage_EX_Init();
        while(1)
        {
            Start_Heater();
            Stop_Heater();

            //���ط���������ˮ��ָ��
            if(Heater_Control_Flag==1 && Heater_Current_Flag==0)
            {
                Start_Heater();
                Heater_Current_Flag=1;
            }
            //���ط���ֹͣ��ˮ��ָ��
            if(Heater_Control_Flag==0 && Heater_Current_Flag==1)
            {
                Stop_Heater();
                Heater_Current_Flag=0;
            }
            //��ˮ���ڲ��쳣�����쳣��־���͵����ش���
            if(Heater_Exception_Flag>Heater_Exception_Normal)
            {
                //ֹͣ��ˮ��
                Stop_Heater();
                Heater_Current_Flag=0;

                //������ˮ���ڲ��쳣��־
                //??????

                //©�����쳣
                if(Heater_Exception_Flag==Heater_Exception_Leakage) {
                }
            }
        }
				
    }

}


//������ˮ��
int Start_Heater()
{
    return 0;
}

//ֹͣ��ˮ��
int Stop_Heater()
{
    return 0;
}
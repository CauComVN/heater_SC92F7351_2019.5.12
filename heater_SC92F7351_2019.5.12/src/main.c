#include "Function_Init.H"

/**************************************************************
˵����
1��Options for Target��Target1����BL51 Locate->Code Range:0x100����¼ѡ����ѡ��DISRST����λ����Ϊ��ͨIOʹ�ã�
2���ı�TEST�Ķ��壬���Էֱ���Զ�Ӧ�Ĺ��ܣ�
3��ע�⣺����Function.H����ѡ������ͺţ�SC92F7320��LCD/LED��PWM���ܣ�
***************************************************************/

void main(void)
{
    int AppStatus=1;  //1: �������� -1:��ˮ/��ˮ�¶� -2:��ˮ����� -3��©���� -4��ˮ���¶ȼ�ⶨʱ�� -5�������� -6������¶ȱ��� -7:�̵������� -8��PWM���� -9�����ڷ���

    int ret=0;
    int test_relay_on=1; //�̵�����������ˮ����ʼ����
	
		uint nADWtrTempValue=0;
		int water_temperature_in=26;
		int water_temperature_out=26;
		

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
            break;
        case -9:
            Uart0_Test();
            break;
        default:
            break;
        }
    }

    //�������С�����
    if(AppStatus==1)
    {   /**/

        //©����
        Leakage_EX_Init();
			
				//ˮ�����
				Water_Detection_EX_Init();
				//ˮ����ⶨʱ��
				Water_Detection_Timer_Init();
			
				//���ԡ����� ע�⣺һ��Ҫ�Ƚ�ˮ����ֹ����**************
				//Protocol_Heater_Receive_Data = Protocol_Heater_Start;
			
        while(1)
        {
            //����������ˮ��
            if(Protocol_Heater_Receive_Data == Protocol_Heater_Start
                    && heater_relay_on==0) {
                heater_relay_on=1;
                Scr_Driver_Control_Heat_RLY(heater_relay_on);
            }

            //���ط���ֹͣ��ˮ��ָ��
            if(Protocol_Heater_Receive_Data == Protocol_Heater_Stop && heater_relay_on==1)
            {
                heater_relay_on=0;
                Scr_Driver_Control_Heat_RLY(heater_relay_on);
            }
						
						//��ˮ�¶�
						ADC_Init(AIN9);
						nADWtrTempValue=ADC_Convert(); //����ADCת�������ת��ֵ
						ret = get_temperature_from_table(nADWtrTempValue,&water_temperature_in);
						
						if(ret==-1){ //֪ͨ����¶��쳣����������¶ȣ���������BEEP����
							Heater_Exception_Flag=Heater_Ex_In_Water_Temp_Low;
						}
						else if(ret==-2){ //֪ͨ����¶��쳣����������¶ȷ�������BEEP����
							Heater_Exception_Flag=Heater_Ex_In_Water_Temp_High;
						}
						else{
							//35��~60�� �Զ�����
						}

						//��ˮ�¶�
						ADC_Init(AIN8);
						nADWtrTempValue=ADC_Convert(); //����ADCת�������ת��ֵ
						ret = get_temperature_from_table(nADWtrTempValue,&water_temperature_out);
						
						if(ret==-1){ //֪ͨ����¶��쳣����������¶ȣ���������BEEP����
							Heater_Exception_Flag=Heater_Ex_Out_Water_Temp_Low;
						}
						else if(ret==-2){ //֪ͨ����¶��쳣����������¶ȷ�������BEEP����
							Heater_Exception_Flag=Heater_Ex_Out_Water_Temp_High;
						}
						else{
							//35��~60�� �Զ�����
						}

            //��ˮ���ڲ��쳣�����쳣��־���͵����ش���
            if(Heater_Exception_Flag>Heater_Ex_Normal)
            {
                //©�����쳣
                if(Heater_Exception_Flag==Heater_Ex_Leakage) {
                }
            }
        }
    }
}

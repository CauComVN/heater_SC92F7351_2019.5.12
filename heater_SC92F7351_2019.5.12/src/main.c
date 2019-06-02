#include "Function_Init.H"

/**************************************************************
˵����
1��Options for Target��Target1����BL51 Locate->Code Range:0x100����¼ѡ����ѡ��DISRST����λ����Ϊ��ͨIOʹ�ã�
2���ı�TEST�Ķ��壬���Էֱ���Զ�Ӧ�Ĺ��ܣ�
3��ע�⣺����Function.H����ѡ������ͺţ�SC92F7320��LCD/LED��PWM���ܣ�
***************************************************************/

void main(void)
{
	int AppStatus=3;  //3��PWM���� 2:���ڿɿع���ƹ��ʵ��ڵ�������  1: �������� -1:��ˮ/��ˮ�¶� -2:��ˮ����� -3��©���� -4��ˮ���¶ȼ�ⶨʱ�� -5�������� -6������¶ȱ��� -7:�̵������� -8��PWM���� -9�����ڷ��ͽ���

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
    {

        //©����
//        Leakage_EX_Init();

        //ˮ�����
        Water_Detection_EX_Init();
        //ˮ����ⶨʱ��
        Water_Detection_Timer_Init();

//        //�����ɿع����
//        Zero_Crossing_EX_Init();

        //����ͨ�ų�ʼ��
//        Uart0_Init();

        //���ԡ����� ע�⣺һ��Ҫ�Ƚ�ˮ����ֹ����**************
        Protocol_Heater_Receive_Data = Protocol_Heater_Start;

        while(1)
        {
            //ʵʱ���ˮ����һ��Ҫ��ˮ������ֹ���գ�Σ�ա�����
//            if(water_flow_flag != 2)
//						{ //ˮ��״̬��� 0����ˮ�� 1����ˮ�� 2����ˮ��������
//
//								heater_relay_on=0;
//								Scr_Driver_Control_Heat_RLY(heater_relay_on);
//						}

            //����������ˮ��
            if(Protocol_Heater_Receive_Data == Protocol_Heater_Start
                    && heater_relay_on==0)
            {
//#ifdef SERIAL_TEST
//                UART_SentChar(Protocol_Heater_Receive_Data+0x20);
//#endif
                //if(water_flow_flag == 2)
                {
                    //ˮ��״̬��� 0����ˮ�� 1����ˮ�� 2����ˮ��������
                    heater_relay_on=1;
                    Scr_Driver_Control_Heat_RLY(heater_relay_on);

                    //���ط���ָ���Ѵ������ڽ����������ó�Ĭ��״̬
                    Protocol_Heater_Receive_Data=Protocol_Heater_Default;
									
									Zero_Crossing_EX_Init();

										
                }
            }

            //���ط���ֹͣ��ˮ��ָ��
//            if(Protocol_Heater_Receive_Data == Protocol_Heater_Stop && heater_relay_on==1)
//            {
////#ifdef SERIAL_TEST
////                UART_SentChar(Protocol_Heater_Receive_Data+0x20);
////#endif
//                heater_relay_on=0;
//                Scr_Driver_Control_Heat_RLY(heater_relay_on);

//                //���ط���ָ���Ѵ������ڽ����������ó�Ĭ��״̬
////                Protocol_Heater_Receive_Data=Protocol_Heater_Default;
//            }

//            if(Protocol_Heater_Receive_Data == Protocol_Heater_Increases_Power && heater_relay_on==1)
//            {
//#ifdef SERIAL_TEST
//                UART_SentChar(Protocol_Heater_Receive_Data+0x20);
//#endif
//                //���߹���
//                Scr_Driver_PWM_Adjust(1); //flag=1
//            }
//            else if(Protocol_Heater_Receive_Data == Protocol_Heater_Reduce_Power && heater_relay_on==1)
//            {
//#ifdef SERIAL_TEST
//                UART_SentChar(Protocol_Heater_Receive_Data+0x20);
//#endif
//                //���͹���
//                Scr_Driver_PWM_Adjust(2); //flag=2
//            }

            //��ˮ�¶�
            ADC_Init(AIN9);
            nADWtrTempValue=ADC_Convert(); //����ADCת�������ת��ֵ
            ret = get_temperature_from_table(nADWtrTempValue,&water_temperature_in);

            if(ret==-1) { //֪ͨ����¶��쳣����������¶ȣ���������BEEP����
                Heater_Exception_Flag=Heater_Ex_In_Water_Temp_Low;
            }
            else if(ret==-2) { //֪ͨ����¶��쳣����������¶ȷ�������BEEP����
                Heater_Exception_Flag=Heater_Ex_In_Water_Temp_High;
            }
            else {
                //35��~60�� �Զ�����
            }

            //��ˮ�¶�
            ADC_Init(AIN8);
            nADWtrTempValue=ADC_Convert(); //����ADCת�������ת��ֵ
            ret = get_temperature_from_table(nADWtrTempValue,&water_temperature_out);

            if(ret==-1) { //֪ͨ����¶��쳣����������¶ȣ���������BEEP����
                Heater_Exception_Flag=Heater_Ex_Out_Water_Temp_Low;
            }
            else if(ret==-2) { //֪ͨ����¶��쳣����������¶ȷ�������BEEP����
                Heater_Exception_Flag=Heater_Ex_Out_Water_Temp_High;
            }
            else {
//                //35��~60�� �Զ�����
//                if(water_temperature_out<good_temperature_out_low && heater_relay_on==1)
//                {
//                    //���߹���
//                    Scr_Driver_PWM_Adjust(1); //flag=1
//                }
//                else if(water_temperature_out>good_temperature_out_high  && heater_relay_on==1)
//                {
//                    //���͹���
//                    Scr_Driver_PWM_Adjust(2); //flag=2
//                }
//                else {
//                }
            }

            //����¶ȱ��� HEAT ERROR ֱ�Ӽ��˿�ֵ P03   ��ѯ��ʽ
//            ret=Scr_Driver_Check_Heat_Error();
//            if(ret==-1 && heater_relay_on==1)
//            {
//                Heater_Exception_Flag=Heater_Ex_Thermal_Switch_Error;

//                heater_relay_on=0;
//                Scr_Driver_Control_Heat_RLY(heater_relay_on);
//            }

//            //��ˮ���ڲ��쳣�����쳣��־���͵����ش���
//            if(Heater_Exception_Flag>Heater_Ex_Normal)
//            {
//                //©�����쳣
//                if(Heater_Exception_Flag==Heater_Ex_Leakage) {
//                }
//                //��ˮ������ˮ��
//                if(Heater_Exception_Flag == Heater_Ex_Water_No_Flow
//                        || Heater_Exception_Flag == Heater_Ex_Water_Little_Flow)
//                {
//                    //ֹͣ��ˮ��
//										if(heater_relay_on == 1)
//										{
//											heater_relay_on=0;
//											Scr_Driver_Control_Heat_RLY(heater_relay_on);
//										}
//                }
//            }
        }
    }
		
		if(AppStatus==2)
		{
			///�ô���P12 P13ģ����ƹ���
			////////////////////////////////////////////////////////////
			//�����жϿ�INT00 P10
			P1CON &= 0XF3;     //�ж�IO������Ϊ��������
			P1PH  |= 0x0c;     //�ж�IO������Ϊ���������
		 
			//����INT00�������ж�
				//�½�������	
			INT0F |= 0x0c;//= 0X00 ;    //xxxx 0000  0�ر� 1ʹ��
				//����������	
			INT0R &= 0Xf3;    //xxxx 0000  0�ر� 1ʹ�� 
			
			//�ⲿ�ж����ȼ�����
			IE  |= 0x01;	//0000 0x0x  INT0ʹ��
			IP  |= 0X00;
			
			EA = 1;
			//////////////////////////////////////////////////////////
			while(1)
			{
			}
		}
		if(AppStatus==3)
		{
			Scr_Driver_PWM_Init();
			while(1)
			{					
			}
		}
}

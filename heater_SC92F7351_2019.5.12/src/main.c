#include "Function_Init.H"

/**************************************************************
˵����
1��Options for Target��Target1����BL51 Locate->Code Range:0x100����¼ѡ����ѡ��DISRST����λ����Ϊ��ͨIOʹ�ã�
2���ı�TEST�Ķ��壬���Էֱ���Զ�Ӧ�Ĺ��ܣ�
3��ע�⣺����Function.H����ѡ������ͺţ�SC92F7320��LCD/LED��PWM���ܣ�
***************************************************************/
#define Test  0  //0:��ˮ/��ˮ�¶� 1:ˮ����� 2��©���� 3��ˮ���¶ȼ�ⶨʱ�� 4��������


void main(void)
{
	WDTCON |= 0x10;		    //�忴�Ź�
	switch(Test)
	{
		case 0:
			ADC_Test();
			break;
		case 1:
			Water_Detection_EXTI_Test();
			break;
		case 2:
			Leakage_EXTI_Test();
			break;
		case 3:
			Water_Detection_Timer_Test();
			break;
		case 4:
			Zero_Crossing_EXTI_Test();
		default:
			break;
	}
}
#include "Function_Init.H"
#include "MATH.H"
#include "ADC.h"

void ADC_Init(uint Channel);
void ADC_Test(void);
int ADC_Convert(void);
void ADC_channel(unsigned char channel);
void ADC_Interrupt_Handle(void);

int get_temperature_from_table(uint nADValue, float* fTemperature);

unsigned int ADCValue = 0x0000;
bit AdcFlag = 0;

////////////////////////////////////////////////////////////
//AIN9: AD in ��ˮ�����¶�
//AIN8: AD out ��ˮ�����¶�


//������β���ת��������Ҫ���ڻ����7
float ADC_Value0,ADC_Value1,ADC_Value2;

/***********SC92F7351 ADC������ѡ��*************/
enum Channel {AIN0=0,AIN1,AIN4=4,AIN5,AIN8=8,AIN9,VDD4=15};

/*****************************************************
*�������ƣ�void ADC_Test(void)
*�������ܣ�ADC����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void ADC_Test(void)
{
    int t8=0.0,t9=0.0;
    int ret =0;
    float fTemperature=26.0;
		int flag=0;

    while(1)
    {
        //ADC_Init(AIN9);
				//t9=ADC_Convert(); //����ADCת�������ת��ֵ
				//ret = get_temperature_from_table(t9,&fTemperature);

        ADC_Init(AIN8);
        t8=ADC_Convert(); //����ADCת�������ת��ֵ
        ret = get_temperature_from_table(t8,&fTemperature);
			
				flag=1;
				
    }
}
/*****************************************************
*�������ƣ�void ADC_Init(uint Channel)
*�������ܣ�ADC��ʼ��
*��ڲ�����Channel
*���ڲ�����void
*****************************************************/
void ADC_Init(uint Channel)
{
    ADCCON = 0X80|Channel;		//����ADC��ADC����Ƶ��Ϊ2M ,ѡ��ChannelλADC������
    if(Channel<8)
    {
        ADCCFG0 = 1<<Channel;   //����Channel��Ϊ������
    }
    else
    {
        ADCCFG1 = 1<<(Channel-8);   //����Channel��Ϊ������
    }
    IE |= 0X40;        //����ADC�ж�
    EA = 1;
}

void ADC_Interrupt_Handle(void)
{
	ADCCON &= ~(0X10);  //���жϱ�־λ
    AdcFlag = 1;
}

////////////////////////////////////////////////////////////
int search(uint arry[],uint n,uint key)
{
    uint low = 0,high = n-1; // n������ -1��high���ܳ�����Ԫ�ظ��������ޣ�
    uint mid,count=0;

    if( key>=arry[high] )
        return high;
    if( key<=arry[low] )
        return low;
    while(low<=high)
    {
        mid = (low+high)/2;
        if( key>=arry[mid] && key <= arry[mid+1] )
            return mid;
        if(arry[mid]<key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0;
}

int get_temperature_from_table(uint nADValue, float* fTemperature)
{
    if(nADValue>=NTC_AD_VALUE_MAX)
    {
        //֪ͨ����¶��쳣����������BEEP����
        return -1;
    }
    else if(nADValue<=NTC_AD_VALUE_MIN)
    {
        //֪ͨ����¶��쳣����������BEEP����
        return 0;
    }
    else
    {
        *fTemperature =search(bufTable_NTC_AD, sizeof(bufTable_NTC_AD)/2,	111);

        return 1;
    }
}


//8 AIN8 AD-out ��ˮ�¶�
//7 AIN9 AD-in	��ˮ�¶�
/*****************************************************
*�������ƣ�int ADC_Convert(void)
*�������ܣ����ؼ���¶�ֵ
*��ڲ�����void
*���ڲ�����int
*****************************************************/
int ADC_Convert(void)
{

    uint Tad=0;
    uint MinAd=0x0fff,MaxAd=0x0000,TempAdd=0;
    uchar t=0;
    for(t=0; t<7; t++)
    {
        ADCCON |= 0x40; //��ʼADCת��
        while(!AdcFlag); //�ȴ� ADCת�����
        AdcFlag = 0;
        Tad = ((unsigned int)(ADCVH<<4))+(ADCVL>>4); //ȡ��һ��ת��ֵ

        //��һ��ֵ���ɿ���ȥ��
        if(t==0) {
            continue;
        }

        if(Tad>MaxAd) {
            MaxAd=Tad; //��õ�ǰ�����ֵ
        }
        if(Tad<MinAd) {
            MinAd=Tad; //��õ�ǰ����Сֵ
        }
        TempAdd+=Tad; //ת��ֵ�ۼ�
    }
    TempAdd-=MinAd; //ȥ����Сֵ

    TempAdd-=MaxAd; //ȥ�����ֵ

    //TempAdd=TempAdd/4; //��ƽ��ֵ
    TempAdd>>=2; //��ƽ��ֵ��������7�Σ�ȥ��3�Σ���Ϊ4��4��2�η�����������2

    //12λADC 2(12)=4096  5v=5v;
    //volta=(float)TempAdd*5/4095;
    //Rt = 10000*volta/(5-volta); //����������ֵ

    return TempAdd;
}


/*****************************************************
*�������ƣ�int get_temperature_from_formula(unsigned int nADValue, float* fTemperature)
*�������ܣ���ʽ����ȡ����¶�
*��ڲ�����nADValue: ����ADֵ
*���ڲ�����0��ʧ�� 1���ɹ�
*****************************************************/
//int get_temperature_from_formula(uint nADValue, float* fTemperature)
//{
//    //------- ��ʽ����8λ��Ԫ��Ƭ�������ã�������̫�� ------

//    float T2=(273.15+25.0); //T2

//    //R������������T2�����µı����ֵ��100K����������25���ֵΪ100K����R=100K����T2=(273.15+25)
//    int R=100000; //100k

//    uint Rp=10000;// �������� 10k

//    float Bx=3950.0; //B
//    float Ka=273.15;
//    float ret = 0.0;
//    float Rt = 0.0;
//    float volta = 0.0;

//    //д��ʽ����ѹת��������ת�����¶�ת����
//    //12λADC 2(12)=4096  5v=5v;
//    volta=(float)nADValue*5.0/4095;

//    if((5.0-volta)> 0.01) {
//        Rt = (float)(Rp*volta)/(5.0-volta);

//        ret=(1.0/(log(Rt/R)/Bx+(1/T2)))-273.15+0.5;

//        //RES: -10C~100C
//        if(ret>100 && ret<-10)
//        {
//            *fTemperature = ret;
//            return 1;
//        }
//        else
//        {
//            return 0;
//        }
//    }
//    else
//    {
//        return 0;
//    }
//}


//void ADC_channel(unsigned char channel)
//{
//    ADCCON=ADCCON&0xe0|channel; //ADC����ѡ��channel��
//}

/*
void ADC_Multichannel()
{
	ADCCFG0 = 0x07; //����AIN0��AIN1��AN2ΪADC�ڣ����Զ������������Ƴ�
	ADCCON |= 0x80; //����ADCģ���Դ

	ADC_channel(0); //ADC����л���AIN0��
	ADC_Value0=ADC_Convert(); //����ADCת�������ת��ֵ

	ADC_channel(1); //ADC����л���AIN1��
	ADC_Value1=ADC_Convert(); //����ADCת�������ת��ֵ

	ADC_channel(2); //ADC����л���AIN2��
	ADC_Value2=ADC_Convert(); //����ADCת�������ת��ֵ
}
*/
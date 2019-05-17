#include "Function_Init.H"
#include "MATH.H"
#include "ADC.h"

void ADC_Init(uint Channel);
void ADC_Test(void);
float ADC_Convert(void);
void ADC_channel(unsigned char channel);

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
//	ADC_Init(AIN0);
//	while(1)
//	{
//		ADCCON |= 0X40;   //��ʼADCת��
//		while(!AdcFlag);	         		 //�ȴ� ADCת�����;
//		AdcFlag = 0;
//		ADCValue = (ADCVH<<4)+(ADCVL>>4);
//	}

    float t8=0.0,t9=0.0;
    int ret =0;

//    ADC_Init(AIN9);
//    t9=ADC_Convert(); //����ADCת�������ת��ֵ

    ADC_Init(AIN8);
    t8=ADC_Convert(); //����ADCת�������ת��ֵ

    ret = 0;
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


void ADC_Interrupt(void) interrupt 6
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

/*****************************************************
*�������ƣ�void list_adc(void)
*�������ܣ�ADC����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void list_adc(void)
{
	/**/
    uint i=0;
    static uchar loopCnt=0;

//    if(bufSys.step !=3)
//        return;
    read_ad(AN_NTC1 );
    buf_adValue=read_ad(AN_NTC1 );
    for(i=0; i<AD_NUMS-1; i++ )
        buf_Rdy_AD1[i]=buf_Rdy_AD1[i+1];
    buf_Rdy_AD1[AD_NUMS-1]=buf_adValue;
    if(++loopCnt>12)
    {
        loopCnt=0;
        buf_Sum=0;
        for(i=0; i<AD_NUMS; i++ )
            buf_Sum+=buf_Rdy_AD1[i];
        buf_Sum=buf_Sum/10;
        if(buf_Sum>=NTC_AD_VALUE_MAX)
        {
            b_Err_ntc_Open=true;
            if(buf_NTCErrDly==0 )
            {
                buf_NTCErrDly=TIME_ERR_NTC_BEEP; //@*100ms
                Buzzer_set(2);
            }
        }
        else if(buf_Sum<=NTC_AD_VALUE_MIN)
        {
            b_Err_ntc_Short=true;
            if(buf_NTCErrDly==0 )
            {
                buf_NTCErrDly=TIME_ERR_NTC_BEEP; //@*100ms
                Buzzer_set(2);
            }
        }
        else  if(buf_NTCErrDly==0)
        {
            b_Err_ntc_Open=false;
            b_Err_ntc_Short=false;
            buf_adValue=buf_Sum;
            buf_UseTempr =search(bufTable_NTC_AD, sizeof(bufTable_NTC_AD)/2,	buf_Sum);
        }
        nop;
    }
	
}


//8 AIN8 AD-out ��ˮ�¶�
//7 AIN9 AD-in	��ˮ�¶�
/*****************************************************
*�������ƣ�float ADC_Convert(void)
*�������ܣ����ؼ���¶�ֵ
*��ڲ�����void
*���ڲ�����float
*****************************************************/
float ADC_Convert(void)
{
    float T2=(273.15+25.0); //T2

    //R������������T2�����µı����ֵ��100K����������25���ֵΪ100K����R=100K����T2=(273.15+25)
    int R=100000; //100k

    int R8=10000; //10k R8=Rp ��������
    //int R8=100000.0; //100k
	
		unsigned int Rp=10000;// �������� 10k

    float Bx=3950.0; //B
    float Ka=273.15;

    float test = 0.0;
    float ret = 0.0;
    float Rt = 0.0;
    float volta = 0.0;
    unsigned int Tad=0;
    unsigned int MinAd=0x0fff,MaxAd=0x0000,TempAdd=0;
    unsigned char t=0;
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
//    volta=(float)TempAdd*5/4095;
//		Rt = 10000*volta/(5-volta);
		
		ret=26; //�����¶�ֵ

////////////////////////////////////////////////
///////////////////////////////////////////////
// ��ʽ����8λ��Ԫ��Ƭ�������ã�������̫��
#if 0
    //д��ʽ����ѹת��������ת�����¶�ת����
    //12λADC 2(12)=4096  5v=5v;
    volta=(float)TempAdd*5.0/4095;

    if((5.0-volta)> 0.01) {
        Rt = (float)(R8*volta)/(5.0-volta);

        ret=(1.0/(log(Rt/R)/Bx+(1/T2)))-273.15+0.5;
        test=0;
        return ret;
    }
    else
    {
        //test
        volta=3.0; //�ٶ����õ�ѹֵ
        Rt = (R8*volta)/(5.0-volta);

        ret=(1/(log(Rt/R)/Bx+(1/T2)))-273.15+0.5;
        test=0;
        return ret;
    }
#endif
}

void ADC_channel(unsigned char channel)
{
    ADCCON=ADCCON&0xe0|channel; //ADC����ѡ��channel��
}

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
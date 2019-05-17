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
//AIN9: AD in 进水采样温度
//AIN8: AD out 出水采用温度


//连续多次采样转换，设置要等于或大于7
float ADC_Value0,ADC_Value1,ADC_Value2;

/***********SC92F7351 ADC采样口选择*************/
enum Channel {AIN0=0,AIN1,AIN4=4,AIN5,AIN8=8,AIN9,VDD4=15};

/*****************************************************
*函数名称：void ADC_Test(void)
*函数功能：ADC测试
*入口参数：void
*出口参数：void
*****************************************************/
void ADC_Test(void)
{
//	ADC_Init(AIN0);
//	while(1)
//	{
//		ADCCON |= 0X40;   //开始ADC转换
//		while(!AdcFlag);	         		 //等待 ADC转换完成;
//		AdcFlag = 0;
//		ADCValue = (ADCVH<<4)+(ADCVL>>4);
//	}

    float t8=0.0,t9=0.0;
    int ret =0;

//    ADC_Init(AIN9);
//    t9=ADC_Convert(); //启动ADC转换，获得转换值

    ADC_Init(AIN8);
    t8=ADC_Convert(); //启动ADC转换，获得转换值

    ret = 0;
}
/*****************************************************
*函数名称：void ADC_Init(uint Channel)
*函数功能：ADC初始化
*入口参数：Channel
*出口参数：void
*****************************************************/
void ADC_Init(uint Channel)
{
    ADCCON = 0X80|Channel;		//开启ADC，ADC采样频率为2M ,选择Channel位ADC采样口
    if(Channel<8)
    {
        ADCCFG0 = 1<<Channel;   //设置Channel作为采样口
    }
    else
    {
        ADCCFG1 = 1<<(Channel-8);   //设置Channel作为采样口
    }
    IE |= 0X40;        //开启ADC中断
    EA = 1;
}


void ADC_Interrupt(void) interrupt 6
{
    ADCCON &= ~(0X10);  //清中断标志位
    AdcFlag = 1;
}

////////////////////////////////////////////////////////////
int search(uint arry[],uint n,uint key)
{
    uint low = 0,high = n-1; // n个数据 -1，high不能超数组元素个数的上限；
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
*函数名称：void list_adc(void)
*函数功能：ADC测试
*入口参数：void
*出口参数：void
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


//8 AIN8 AD-out 出水温度
//7 AIN9 AD-in	进水温度
/*****************************************************
*函数名称：float ADC_Convert(void)
*函数功能：返回检测温度值
*入口参数：void
*出口参数：float
*****************************************************/
float ADC_Convert(void)
{
    float T2=(273.15+25.0); //T2

    //R是热敏电阻在T2常温下的标称阻值。100K的热敏电阻25℃的值为100K（即R=100K）。T2=(273.15+25)
    int R=100000; //100k

    int R8=10000; //10k R8=Rp 上拉电阻
    //int R8=100000.0; //100k
	
		unsigned int Rp=10000;// 上拉电阻 10k

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
        ADCCON |= 0x40; //开始ADC转换
        while(!AdcFlag); //等待 ADC转换完成
        AdcFlag = 0;
        Tad = ((unsigned int)(ADCVH<<4))+(ADCVL>>4); //取得一次转换值

        //第一次值不可靠，去掉
        if(t==0) {
            continue;
        }

        if(Tad>MaxAd) {
            MaxAd=Tad; //获得当前的最大值
        }
        if(Tad<MinAd) {
            MinAd=Tad; //获得当前的最小值
        }
        TempAdd+=Tad; //转换值累加
    }
    TempAdd-=MinAd; //去掉最小值

    TempAdd-=MaxAd; //去掉最大值

    //TempAdd=TempAdd/4; //求平均值
    TempAdd>>=2; //求平均值，如果检测7次，去掉3次，变为4，4是2次方，所以右移2
		
		//12位ADC 2(12)=4096  5v=5v;
//    volta=(float)TempAdd*5/4095;
//		Rt = 10000*volta/(5-volta);
		
		ret=26; //返回温度值

////////////////////////////////////////////////
///////////////////////////////////////////////
// 公式法对8位赛元单片机不适用，计算量太大
#if 0
    //写公式（电压转换，电阻转换，温度转换）
    //12位ADC 2(12)=4096  5v=5v;
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
        volta=3.0; //假定采用电压值
        Rt = (R8*volta)/(5.0-volta);

        ret=(1/(log(Rt/R)/Bx+(1/T2)))-273.15+0.5;
        test=0;
        return ret;
    }
#endif
}

void ADC_channel(unsigned char channel)
{
    ADCCON=ADCCON&0xe0|channel; //ADC输入选择channel口
}

/*
void ADC_Multichannel()
{
	ADCCFG0 = 0x07; //设置AIN0，AIN1，AN2为ADC口，并自动将上拉电阻移除
	ADCCON |= 0x80; //开启ADC模块电源

	ADC_channel(0); //ADC入口切换至AIN0口
	ADC_Value0=ADC_Convert(); //启动ADC转换，获得转换值

	ADC_channel(1); //ADC入口切换至AIN1口
	ADC_Value1=ADC_Convert(); //启动ADC转换，获得转换值

	ADC_channel(2); //ADC入口切换至AIN2口
	ADC_Value2=ADC_Convert(); //启动ADC转换，获得转换值
}
*/
#include "Function_Init.H"
#include "MATH.H"

void ADC_Test(void);
void ADC_Init(uint Channel);
float ADC_Convert(void);
void ADC_channel(unsigned char channel);
	
unsigned int ADCValue = 0x0000;
bit AdcFlag = 0;

////////////////////////////////////////////////////////////
const unsigned int NUM = 6; //连续多次采样转换，设置要等于或大于6
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
    ADC_Init(AIN0);
    while(1)
    {
        ADCCON |= 0X40;   //开始ADC转换
        while(!AdcFlag);	         		 //等待 ADC转换完成;
        AdcFlag = 0;
        ADCValue = (ADCVH<<4)+(ADCVL>>4);
    }
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

/*****************************************************
*函数名称：float ADC_Convert(void)
*函数功能：返回检测温度值
*入口参数：void
*出口参数：float
*****************************************************/
float ADC_Convert(void)
{
    float T2=(273.15+25.0); //T2
    float Rp=100000.0; //100k
    float R8=10000.0; //10k
    float Bx=3950.0; //B
    float Ka=273.15;

    float ret = 0.0;
    float Rt = 0.0;
    float volta = 0.0;
    unsigned int nRemoveNum=0;
    unsigned int Tad=0;
    unsigned int MinAd=0x0fff,MaxAd=0x0000,TempAdd=0;
    unsigned char t=0;
    for(t=0; t<NUM; t++)
    {
        ADCCON |= 0x40; //开始ADC转换
        while(!AdcFlag); //等待 ADC转换完成
        AdcFlag = 0;
        Tad = ((unsigned int)(ADCVH<<4))+(ADCVL>>4); //取得一次转换值

        //第一次值不可靠，去掉
        if(t==0) {
            nRemoveNum++;
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
    nRemoveNum++;
    TempAdd-=MaxAd; //去掉最大值
    nRemoveNum++;
    TempAdd>>=(NUM-nRemoveNum); //求平均值

    //写公式（电压转换，电阻转换，温度转换）
    //12位ADC 2(12)=4096  5v=5v;
    volta=(float)TempAdd*5.0/4095;
    Rt = (R8*volta)/(5.0-volta);

    ret=(1/(log(Rt/Rp)/Bx+(1/T2)))-273.15+0.5;
    return ret;
}

void ADC_channel(unsigned char channel)
{
    ADCCON=ADCCON&0xe0|channel; //ADC输入选择channel口
}

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
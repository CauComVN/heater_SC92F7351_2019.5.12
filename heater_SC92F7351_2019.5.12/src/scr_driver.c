//Zero Crossing detection
//SCR Driver

#include "Function_Init.H"

//INT24 P20

//HEAT TRA PWM1 P01

//HEAT RLY ¼ÌµçÆ÷¿ØÖÆ P02

//T/S1 ÎÂ¶È±£ÏÕ HEAT ERROR COM3/P03
//Èç¹û¼ì²âµ½ÎÂ¶È¹ı¸ß£¬T/S1»á×Ô¶¯ÌøÕ¢£¬¶Ï¿ªµçÂ· 
//¼ÌµçÆ÷Í£Ö¹¹¤×÷ ±£»¤Õû¸ö¿É¿Ø¹èÇı¶¯µçÂ·
//HEAT ERROR ÎªÊäÈë¶Ë£¬Èç¹ûµÈÓÚ¸ßµçÆ½£¬±íÃ÷ÈÈË®Æ÷ÎÂ¶È¹ı¸ß
//Èç¹ûÎªµÍµçÆ½£¬±íÃ÷ÈÈË®Æ÷ÎÂ¶ÈÔÚÕı³£·¶Î§ÄÚ

void Zero_Crossing_EXTI_Test(void);
void Zero_Crossing_EX_Init(void);
void Zero_Crossing_EX2_Handle();

void Scr_Driver_PWM_Init(void);
void Scr_Driver_PWMInt_Handle();
int Scr_Driver_Check_Heat_Error();//¼ì²âÎÂ¶È±£ÏÕ
void Scr_Driver_Control_Heat_RLY(int on);//¼ÌµçÆ÷¿ØÖÆ HEAT RLY P02

uchar Zero_Crossing_INT1_flag = 0x00;
/*****************************************************
*º¯ÊıÃû³Æ£ºvoid EXTI_Test(void)
*º¯Êı¹¦ÄÜ£ºÍâ²¿ÖĞ¶Ï²âÊÔ
*Èë¿Ú²ÎÊı£ºvoid
*³ö¿Ú²ÎÊı£ºvoid
*****************************************************/
void Zero_Crossing_EXTI_Test(void)
{
    Zero_Crossing_EX_Init();
    while(1)
    {
    }
}
/*****************************************************
*º¯ÊıÃû³Æ£ºvoid EX_Init(void)
*º¯Êı¹¦ÄÜ£ºÍâ²¿ÖĞ¶Ï³õÊ¼»¯
*Èë¿Ú²ÎÊı£ºvoid
*³ö¿Ú²ÎÊı£ºvoid
*****************************************************/
void Zero_Crossing_EX_Init(void)
{
    //ÅäÖÃÖĞ¶Ï¿ÚINT24
    P2CON &= 0XFE;     //ÖĞ¶ÏIO¿ÚÉèÖÃÎª¸ß×èÊäÈë
    P2PH  |= 0x01;     //ÖĞ¶ÏIO¿ÚÉèÖÃÎª¸ß×è´øÉÏÀ­

    //INT24ÉÏÉıÖĞ¶Ï
    //ÏÂ½µÑØÉèÖÃ
    INT2F = 0X00 ;    //0000 xxxx  0¹Ø±Õ 1Ê¹ÄÜ
    //ÉÏÉıÑØÉèÖÃ
    INT2R = 0X10 ;    //0000 xxxx  0¹Ø±Õ 1Ê¹ÄÜ

    //Íâ²¿ÖĞ¶ÏÓÅÏÈ¼¶ÉèÖ
    IE1 |= 0x08;	//0000 x000  INT2Ê¹ÄÜ
    IP1 |= 0X00;
    EA = 1;
}

void Zero_Crossing_EX2_Handle()
{
	/*
    //Èç¹ûÖĞ¶Ï2ÓĞÁ½Â·ÊäÈë£¬¸ù¾İÉÏÉıÑØ»òÕßÏÂ½µÑØÀ´È·ÈÏ£¬ÉÏÉıÑØÖĞ¶Ï£¬ËùÒÔ¶Ë¿ÚµçÆ½ÊÇ1
    if(P20 == 1) //INT24 P20 ¹ıÁã¼ì²âµ½Áãµã
    {
        //PWM¼ÆÊıÖµÖØÖÃ
        Scr_Driver_PWM_Init();
    }
    if(P21 == 1) //INT25 P21 Ë®Á÷¼ì²â¼ÆÊı
    {

    }
	*/
}

//HEAT TRA PWM1
void Scr_Driver_PWM_Init(void)
{
	/**/
    //50hz 20ms °ë¸öÖÜÆÚ10ms 10ms/10=1ms PWMÊ±ÖÓÎªFsys/128 187*128/24=997.33 187-1=186
    PWMCON  = 0x16;		//PWMÊä³öµ½IO£¬PWMÊ±ÖÓÎªFsys/128 HEAT TRA PWM1
    PWMPRD  = 186;		//PWMÖÜÆÚ=(186+1)*(1*128/24us)=997.33¡Ö=1ms;
    PWMCFG  = 0x10;		//PWM1Êä³ö·´Ïò,Êä³öÖÁP01
    PWMDTY1 = 15;     //PWM1µÄDuty = 15/60 =1/4
    PWMDTYA = 0x00;		//PWMÕ¼¿Õ±ÈÎ¢µ÷¼Ä´æÆ÷£¬ÕâÀï²»Î¢µ÷
    PWMCON |= 0x80;     //¿ªÆôPWM
    IE1 |= 0x02;        //¿ªÆôPWMÖĞ¶Ï
    EA = 1;
	
}

void Scr_Driver_PWMInt_Handle()
{
    //¸ù¾İ³öË®/½øË®ÎÂ¶È×Ô¶¯µ÷½ÚPWM1µÄDuty
}

//¼ì²âÎÂ¶È±£ÏÕ HEAT ERROR Ö±½Ó¼ì²â¶Ë¿ÚÖµ P03
int Scr_Driver_Check_Heat_Error()
{
	P0VO = P0VO&0xf7; //P03¶Ë¿ÚÉèÖÃ³ÉÆÕÍ¨I/O¿Ú
	if(P03==0)
	{
		//ÎÂ¶ÈÕı³£·¶Î§ÄÚ£¬ÎÂ¶È±£ÏÕ²»ÌøÕ¢
		return 1;
	}
	else if(P03==1)
	{
		//ÎÂ¶ÈÒì³£·¶Î§ÄÚ£¬ÎÂ¶È±£ÏÕÒÑÌøÕ¢
		return 0;
	}
	return 0;
}

//¼ÌµçÆ÷¿ØÖÆ HEAT RLY P02
void Scr_Driver_Control_Heat_RLY(int on)
{
	P0VO = P0VO&0xfb; //P02¶Ë¿ÚÉèÖÃ³ÉÆÕÍ¨I/O¿Ú  1111 1011
	if(on == 1)
	{
		P02=1;
	}
	else
	{
		P02=0;
	}
}

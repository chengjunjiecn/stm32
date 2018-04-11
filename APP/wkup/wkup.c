#include "wkup.h"

/*******************************************************************************
* º¯ Êı Ãû         : Enter_Standby_Mode
* º¯Êı¹¦ÄÜ		   : ½øÈë´ı»úÄ£Ê½
* Êä    Èë         : ÎŞ
* Êä    ³ö         : ÎŞ
*******************************************************************************/
void Enter_Standby_Mode(void)
{
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//Ê¹ÄÜPWRÍâÉèÊ±ÖÓ
	
	PWR_ClearFlag(PWR_FLAG_WU);//Çå³ıWake-up ±êÖ¾
	
	//PWR_WakeUpPinCmd(ENABLE);//Ê¹ÄÜ»½ĞÑ¹Ü½Å	Ê¹ÄÜ»òÕßÊ§ÄÜ»½ĞÑ¹Ü½Å¹¦ÄÜ
	
	
	PWR_EnterSTANDBYMode();//½øÈë´ı»úÄ£Ê
	//PWR_EnterSleepMode(0);
}



 void myPWR_EnterSleepMode(void)
{
	printf("½øÈëÏµÍ³´ı»úÄ£Ê½ %d\r\n");
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,DISABLE);
	NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT,DISABLE);
	__WFI();                                                 /* Request Wait For Interrupt */
}
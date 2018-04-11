#include "wkup.h"

/*******************************************************************************
* �� �� ��         : Enter_Standby_Mode
* ��������		   : �������ģʽ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Enter_Standby_Mode(void)
{
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//ʹ��PWR����ʱ��
	
	PWR_ClearFlag(PWR_FLAG_WU);//���Wake-up ��־
	
	//PWR_WakeUpPinCmd(ENABLE);//ʹ�ܻ��ѹܽ�	ʹ�ܻ���ʧ�ܻ��ѹܽŹ���
	
	
	PWR_EnterSTANDBYMode();//�������ģ�
	//PWR_EnterSleepMode(0);
}



 void myPWR_EnterSleepMode(void)
{
	printf("����ϵͳ����ģʽ %d\r\n");
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,DISABLE);
	NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT,DISABLE);
	__WFI();                                                 /* Request Wait For Interrupt */
}
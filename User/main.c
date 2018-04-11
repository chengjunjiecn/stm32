

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "wkup.h"


/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
int main()
{
	int i = 0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	
	while(1)
	{
		led1=0;
		delay_ms(1000);	//��1����ʾ����
		led1=1;
		delay_ms(1000);
		i++;
		if(i > 10)
		{
			myPWR_EnterSleepMode();
			i = 0;
		}
	}
}

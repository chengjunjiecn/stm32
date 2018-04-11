

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "wkup.h"


/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{
	int i = 0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	USART1_Init(9600);
	
	while(1)
	{
		led1=0;
		delay_ms(1000);	//隔1秒显示计数
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

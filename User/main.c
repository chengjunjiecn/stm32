

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "wkup.h"


int main()
{
	int i = 0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	LED_Init();
	USART1_Init(9600);
	while(1)
	{
		if(getSystemActive())
		{
			ledSlink();
			if(isReceivedFrame())
			{
				//printf("received a frame\n");
				handData();
			}
		}
		else
		{
			gotoSleepMode();
		}
	}
}

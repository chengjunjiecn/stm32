

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "wkup.h"


/*******************************************************************************
* º¯ Êý Ãû         : main
* º¯Êý¹¦ÄÜ		   : Ö÷º¯Êý
* Êä    Èë         : ÎÞ
* Êä    ³ö         : ÎÞ
*******************************************************************************/
int main()
{
	int i = 0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //ÖÐ¶ÏÓÅÏÈ¼¶·Ö×é ·Ö2×é
	LED_Init();
	USART1_Init(9600);
	while(1)
	{
		if(getSystemActive())
		{
			ledSlink();
			if(isReceivedFrame())
			{
				printf("received a frame");
				handData();
			}
		}
		else
		{
			gotoSleepMode();
		}
	}
}

#include "common.h"

void ledSlink()
{
		led1=0;
		delay_ms(1000);	//¸ô1ÃëÏÔÊ¾¼ÆÊý
		led1=1;
		delay_ms(1000);
}



int struct_array(char* des, Data *src)
{
    int num;
    char *temp = (char *)  src ;
    for(num=0;num<sizeof(Data);num++)
        *(des+num)= temp[num];
    return num;

}




void show_student(Data * name)
{
    printf("id is : %d\n",name->len);

}


void show_array(char *str ,int len )
{
    printf("len is : %d \n" ,len);

    while(len--)
    {
        putchar('|');
        putchar(*str++);
    }
    printf("|\n");
}

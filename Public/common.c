#include "common.h"		 

void ledSlink()
{
		led1=0;
		delay_ms(1000);	//¸ô1ÃëÏÔÊ¾¼ÆÊı
		led1=1;
		delay_ms(1000);
}

void handData()
{
}

int struct_array(char* des, Data *src)
{
    int num;
    char *temp = (char *)  src ;
    for(num=0;num<sizeof(Data);num++)
        *(des+num)= temp[num];
    return num;

}


void array_struct(Data * des ,char * src)
{

    char* temp = (char *) des;
    int num;
    for(num=0 ;num < sizeof(Data);num++)
        *(temp++)= src[num] ;
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

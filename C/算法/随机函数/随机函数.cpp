/*
srand()包含在stdlib.h中 
srand（i）用于初始化随机函数rand（） 
   当i相同时，会产生相同的随机数 

*/ 
#include<stdio.h>
#include<stdlib.h>
int main()
{
    int i;
    for(i=0;i<=3;i++)
    {
       srand(i);                      
       for(int j=0;j<=20;j++)              
           printf("%d  ",rand()%100);          
       putchar('\n');
    }
    putchar('\n');
    putchar('\n');
    for(i=0;i<=3;i++)
    {
       srand(i);                      
       for(int j=0;j<=20;j++)              
           printf("%d  ",rand()%100);          
       putchar('\n');
    }
    getchar();
    return 0;
}

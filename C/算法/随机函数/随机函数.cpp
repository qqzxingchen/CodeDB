/*
srand()������stdlib.h�� 
srand��i�����ڳ�ʼ���������rand���� 
   ��i��ͬʱ���������ͬ������� 

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

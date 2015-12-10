/*
输入一个数字n，可以输出n级杨辉三角
*/
#include<stdio.h>
#include<conio.h>
int main()
{
    int a[100]={1,1};
    int i,j,c;
    printf("please input:\n");
    scanf("%d",&c);
    system("cls"); 
    for(i=1;i<=c;i++)
    {
       for(j=i;j>=2;j--)
          a[j]=a[j]+a[j-1];
       for(j=1;j<=i;j++)
          printf("%d\t",a[j]);
       putchar('\n');
    }
    getchar();
    getchar();
    return 0; 
}

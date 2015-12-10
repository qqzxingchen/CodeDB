#include<stdio.h>
int move(int n,char a,char b,char c)
{
  if(n==1)  
     printf("将第%-4d个从%c移动到%c\n",n,a,c);  
  else 
     {
       move(n-1,a,c,b);                                       
       printf("将第%-4d个从%c移动到%c\n",n,a,c);                                       
       move(n-1,b,a,c);                                                                             
     }    
    
}
int main()
{
  int n;
  scanf("%d",&n);  
  move(n,'A','B','C');  
  getchar();
  getchar();  
}

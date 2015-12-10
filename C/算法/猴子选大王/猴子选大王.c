/*
猴子选大王
一共有num个猴子，做成一圈，开始数数，每数到t时该猴子失去竞选资格
num  猴子总数
t    数数间隔  
*/ 

#include<stdio.h>
#include<stdlib.h>
#define num 20
#define t 5
int main()
{
  int heding(int *p);
  int i;
  int j=0;
  int s[num+1]={0};
  for(i=1;i<=num;i++)
     s[i]=1;
  while(heding(s)>1)
     {
      i=1;
      while(i<=t)
          {
           j++;
           if(j>num)  j=j-num;
           if(s[j]==1)  i++;    
          }
      s[j]=0;
     }
  for(i=1;i<=num;i++)
     {
      if(s[i]==1) s[0]=i;                
     }
  printf("the last one is %d",s[0]);  
  system("pause");  
    
    
}
int heding(int *p)
{
 int i;
 int sum=0; 
 for(i=1;i<=num;i++)
     sum=sum+*(p+i);
 return(sum);   
}









 

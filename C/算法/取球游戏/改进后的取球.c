#include<stdio.h>
#include<windows.h>
int getball[10000]={0,0,1,0,1,1,1,0,1};
int main()
{
   int N;
   int max=0; 
   int s[100]={0};    
   int i,j;
   
   scanf("%d",&N); 
   for(i=1;i<=N;i++)
   {
      scanf("%d",&s[i]);
      if(max<s[i])
         max=s[i];                 
   }
   for(i=9;i<=max;i++)
   {
      getball[i]=!getball[i-1] ||
                 !getball[i-3] ||                   
                 !getball[i-7] ||     
                 !getball[i-8];                           
   }
   for(i=1;i<=N;i++)
   {
      printf("\n%d",getball[s[i]]);                 
   }
   system("pause");
   return 0; 
} 

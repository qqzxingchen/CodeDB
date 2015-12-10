#include<stdio.h>
#include<windows.h>
#define false 0
#define true  1
int getball(int n)
{
   if(n>=1)    
   {
      switch(n)
      {
      case 1:return false;
      case 3:return false;
      case 7:return false;
      case 8:return true;         
      default: return (
                       !getball(n-1) ||
                       !getball(n-3) ||
                       !getball(n-7) ||
                       !getball(n-8)    
                       );         
      }
   }
   else 
      return false; 
}
int main()
{
   int N;
   int s[100]={0};    
   int i,j;
   
   scanf("%d",&N); 
   for(i=1;i<=N;i++)
   {
      scanf("%d",&s[i]);                 
   }
   for(i=1;i<=N;i++)
   {      
      printf("%d\n",getball(s[i]));
   }
   system("pause");
   return 0; 
} 

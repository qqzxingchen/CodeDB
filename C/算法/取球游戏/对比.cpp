#include<stdio.h>
#include<windows.h>
int getball[10000]={0,0,1,0,1,1,1,0,1};
int get(int n)
{
   if(n>=1)    
   {
      switch(n)
      {
      case 1:return false;
      case 3:return false;
      case 7:return false;
      case 8:return true;         
      default:return (
                       !get(n-1) ||
                       !get(n-3) ||
                       !get(n-7) ||
                       !get(n-8)    
                      );         
      }
   }
   else 
      return false; 
}



int main()
{
   int N;     
   int i,j;
   scanf("%d",&N); 
   for(i=9;i<=N;i++)
   {
      getball[i]=!getball[i-1] ||
                 !getball[i-3] ||                   
                 !getball[i-7] ||     
                 !getball[i-8];
      if(getball[i]==get(i))
         printf("%3d ʱ,a==b\n",i);                           
   }  
   system("pause");
   return 0; 
} 

#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
char stemp[100];
int s[100];
int NUM;
int N;
int input()
{
   FILE *fp;
   fp=fopen("input.txt","r");
   fgets(stemp,100,fp); 
   fscanf(fp,"%d",&NUM);
   fclose(fp);
   int i=0;
   while(stemp[i] && stemp[i]!='\n')
   {
      s[i]=stemp[i]-'0';
      i++;               
   } 
   N=i;
   return 0;
}
int randinput()
{
   srand((int)GetCurrentTime());  
     
   N=rand()%90+4; 
   NUM=rand()%(N-2);
   int i;
   s[0]=rand()%9+1;
   for(i=1;i<N;i++) 
      s[i]=rand()%10;
   return 0;
}
int delnum()
{
   int i,j;
   int l[100][100]; 
   int visit[100]={0};
   for(i=0;i<N;i++)
   {
      for(j=0;j<i;j++)                
         l[i][j]=s[j];
      for(j=i;j<N-1;j++)
         l[i][j]=s[j+1];          
   }
   for(j=0;j<N-1;j++)
   {
      if(visit[j]==0 && visit[j+1]==0)
      {
         if(l[j+1][j]>l[j][j])
            for(i=j+1;i<N;i++)
               visit[i]=1;
         else if(l[j+1][j]<l[j][j])
            for(i=0;i<=j;i++)
               visit[i]=1;
      }
   }
   for(i=0;i<N;i++)
      printf("%d",s[i]);
   putchar('\n');
   for(i=0;i<N;i++)
      if(visit[i]==0)
         break;
   for(j=0;j<N-1;j++)
      s[j]=l[i][j];      
   N--;
   while(s[0]==0)
   {
      for(i=1;i<N;i++)              
         s[i-1]=s[i];        
      N--;           
   }
   return 0;
}
int output()
{
   int i;    
   FILE *fp; 
   fp=fopen("out.txt","w"); 
   for(i=0;i<N;i++)
      fprintf(fp,"%d",s[i]);
   fclose(fp);
   return 0; 
}
int main()
{
   input(); 
   int i;
   for(i=1;i<=NUM;i++)
      delnum();
   output(); 
   return 0;    
}

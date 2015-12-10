/*
*算法设计与分析 
*p128 
*
*/
#include<stdio.h>
int s[100][2];
int visit[100];
int N;
int minnum;
int input()
{
   int i;
   FILE *fp;    
   fp=fopen("input.txt","r");
   fscanf(fp,"%d",&N); 
   for(i=0;i<N;i++)
      fscanf(fp,"%d %d",&s[i][0],&s[i][1]);
   fclose(fp);
   return 0; 
}
int clear()
{
   int i;
   for(i=0;i<100;i++)    
      visit[i]=0;
   minnum=0;
   return 0;    
}
int output()
{
   FILE *fp;    
   fp=fopen("out.txt","w"); 
   fprintf(fp,"%d",minnum);
   fclose(fp);
   return 0; 
}
int sort()
{
   int i,j;
   for(i=0;i<N-1;i++)
      for(j=i+1;j<N;j++)
         if(s[i][0]>s[j][0])
         {
            s[i][0]=s[i][0]+s[j][0];                         
            s[j][0]=s[i][0]-s[j][0];
            s[i][0]=s[i][0]-s[j][0];
            
            s[i][1]=s[i][1]+s[j][1];
            s[j][1]=s[i][1]-s[j][1];
            s[i][1]=s[i][1]-s[j][1];                     
         }
   return 0;    
}
int getminnum()   //对实验数据进行处理 
{
   
   int start;
   int i=0;
   while(visit[i])   i++;
   if(i>=N)   return 0;
   start=i;
   visit[start]=1;
   
   int min;   
   int sign=1;
   while(sign)
   {
      sign=0;
      for(i=start+1;i<N;i++)        
         if(!visit[i] && s[i][0]>=s[start][1])
         {
            visit[i]=2;
            sign=1;
         } 
      min=-1;
      for(i=start+1;i<N;i++)
         if(visit[i]==2)
         {
            visit[i]=0;
            if(min==-1)               
               min=i;
            else if(s[min][0]>s[i][0])
               min=i;         
         }
      visit[min]=1;   
      start=min;     
   }
   return 1;
}
int main()
{
   clear();
   input(); 
   sort();
   while(getminnum())
   {
      printf("...\n");
      minnum++; 
   }
   output(); 
   return 0; 
}

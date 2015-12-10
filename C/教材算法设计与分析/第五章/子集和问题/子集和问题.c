#include<stdio.h>
int s[100];
int SUM;
int N;
int n=0;
int l[100]={0};
int num=0;
int now=0;
FILE *fp;
int input()
{
   int i;  
   fp=fopen("input.txt","r");    
   fscanf(fp,"%d %d",&N,&SUM); 
   for(i=0;i<N;i++) 
      fscanf(fp,"%d",&s[i]);
   fclose(fp);
   fp=fopen("out.txt","w");
   return 0;
}
int output()
{
   if(n==0) 
      fprintf(fp,"No Solution!");
   else
   {
      int i;
      for(i=0;i<num;i++)    
         fprintf(fp,"%d  ",l[i]);
      fprintf(fp,"\n");
   }    
   return 0;
}
int find(int step)
{
   if(now==SUM)
   {
      n++;
      output();      
      return 0;
   }  
   if(step>N)
      return 0;
   if(now+s[step]<=SUM)
   {
       now+=s[step];
       l[num]=s[step];                
       num++;
       find(step+1);             
       num--;
       now-=s[step];             
   } 
   find(step+1); 
    
}
int main()
{
   input(); 
   find(0); 
   return 0; 
}

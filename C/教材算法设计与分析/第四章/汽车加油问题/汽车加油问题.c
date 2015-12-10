#include<stdio.h>
int s[100];
int N;
int K; 
int MINNUM=0;
int input()
{
   int i;    
   FILE *fp; 
   fp=fopen("input.txt","r"); 
   fscanf(fp,"%d %d",&N,&K);
   for(i=0;i<=K;i++)
      fscanf(fp,"%d",&s[i]);
   fclose(fp);
   return 0;
}

int output(int i)
{
   
   FILE *fp; 
   fp=fopen("out.txt","w");
   if(i==0)
      fprintf(fp,"%d",MINNUM);
   else
      fprintf(fp,"No Solution!");
   fclose(fp);
   return 0; 
}
int getnum()
{
   int sum=0;    
   int i;
   for(i=0;i<=K;i++)
   {
      if(s[i]>N)
         return 1;
      sum+=s[i];                 
      if(sum+s[i+1]>N && i+1<=K)              
      {
         MINNUM++;
         sum=0;
      }                
   } 
   return 0; 
}
int main()
{
   input();
   output(getnum());
   return 0; 
}

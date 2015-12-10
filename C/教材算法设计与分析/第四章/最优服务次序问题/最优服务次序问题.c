#include<stdio.h>
int s[100];
int N;
float waittime;
int input()
{
   int i;    
   FILE *fp; 
   fp=fopen("input.txt","r"); 
   fscanf(fp,"%d",&N);
   for(i=0;i<N;i++)
      fscanf(fp,"%d",&s[i]);
   fclose(fp);
   return 0;
}
int gettime()
{
   int i,j,time=0; 
   for(i=0;i<N-1;i++)
      for(j=i+1;j<N;j++)
         if(s[i]>s[j])
         {
            s[i]=s[i]+s[j];             
            s[j]=s[i]-s[j];
            s[i]=s[i]-s[j];          
         } 
   for(i=0;i<N;i++)
   {
      time+=(N-i)*s[i];     
   }
   waittime=(float)time/N;
   return 0;
}
int output()
{
   FILE *fp; 
   fp=fopen("out.txt","w");
   fprintf(fp,"%f",waittime);
   fclose(fp);
   return 0; 
}

int main()
{
   input(); 
   gettime(); 
   output();
   return 0; 
}

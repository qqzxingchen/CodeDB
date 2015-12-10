#include<stdio.h>
int s[100];
int N;
int NUM; 
float waittime;
int input()
{
   int i;    
   FILE *fp; 
   fp=fopen("input.txt","r"); 
   fscanf(fp,"%d %d",&N,&NUM);
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
   int l[10][100]={{0}};
   int num[10]={0};
   int minnum;
   for(i=0;i<N;i++)
   {
      minnum=0;
      for(j=1;j<NUM;j++)                
         if(num[minnum]>num[j])          
            minnum=j;
      l[minnum][0]++;
      l[minnum][ l[minnum][0] ]=s[i];
      num[minnum]+=s[i];       
   }
   for(i=0;i<NUM;i++)
      for(j=1;j<=l[i][0];j++)
         time+=(l[i][0]-j+1)*l[i][j];
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

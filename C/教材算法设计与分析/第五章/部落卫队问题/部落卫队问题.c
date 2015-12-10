#include<stdio.h>
int N;
int M;
int s[100][100]={{0}};
int l[100]={0};
int input()
{
   int i;
   int x,y;
   FILE *fp;
   fp=fopen("input.txt","r");    
   fscanf(fp,"%d %d",&N,&M); 
   for(i=0;i<M;i++)
   {
      fscanf(fp,"%d %d",&x,&y);
      s[x][y]=1;
      s[y][x]=1; 
   } 
   fclose(fp);
   return 0;
}
int find()
{
   int i,j;
   for(i=1;i<=N;i++)
      for(j=1;j<=N;j++)
      {
         if(s[i][j]==1)
            goto loop;        
      } 
   return 0;
   int sum[100]={0};
   loop:
   
   sum[0]=1;
   int maxi;
   for(i=1;i<=N;i++)
   {
      sum[i]=0;
      for(j=1;j<=N;j++)                 
         sum[i]+=s[i][j];           
   }
   for(i=2;i<=N;i++)
      if(sum[i]>sum[sum[0]])
         sum[0]=i;
   l[sum[0]]=1;
   for(i=1;i<=N;i++)
   {
      s[sum[0]][i]=0;
      s[i][sum[0]]=0;
   }
   find();
}
int output()
{
   FILE *fp; 
   fp=fopen("out.txt","w"); 
   int i;
   for(i=1;i<=N;i++)
      fprintf(fp,"%d ",!l[i]);
   fclose(fp); 
   return 0;    
}

int main()
{
   input(); 
   find(); 
   output(); 
   return 0; 
}

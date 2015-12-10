/*
*算法设计与分析 
*p 90 
*数字三角形 
*/ 
#include<stdio.h>
int s[100][100];
int N;
int findlongestway(int i,int j)
{
   if(i==N-1)   return s[i][j];
   int a=findlongestway(i+1,j);
   int b=findlongestway(i+1,j+1);    
   if(a>b)  return a+s[i][j]; 
   else     return b+s[i][j]; 
}
int main()
{
   int i,j;
   FILE *fp;
   fp=fopen("input.txt","r");  
   fscanf(fp,"%d",&N);
   for(i=0;i<N;i++)
      for(j=0;j<=i;j++) 
         fscanf(fp,"%d",&s[i][j]);
   fclose(fp);
   fp=fopen("output.txt","w");
   fprintf(fp,"%d\n",findlongestway(0,0));
   fclose(fp);
   return 0;
}

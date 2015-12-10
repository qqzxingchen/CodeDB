/*
*算法设计与分析 
*p 89
*编辑距离问题 
*/

#include<stdio.h>
#include<stdlib.h>
char A[100];
char B[100];
int Anum,Bnum;
int C[100][100];
int D[100][100];
int getnum(char *s)
{
   int i=0;
   while(s[i] && s[i]!='\n')
      i++;
   int num=i;
   while(i>=1)
   {
      s[i]=s[i-1];
      i--;           
   }  
   s[i]='\0'; 
   return num; 
}
int find()
{
   int i,j;    
   for(i=0;i<=Anum;i++)  C[i][0]=0;
   for(i=0;i<=Bnum;i++)  C[0][i]=0;
   for(i=1;i<=Anum;i++)
      for(j=1;j<=Bnum;j++)
      {
         if(A[i]==B[j])                { C[i][j]=C[i-1][j-1]+1; D[i][j]=1; }                 
         else if(C[i-1][j]>=C[i][j-1]) { C[i][j]=C[i-1][j]; D[i][j]=2; }                
         else                          { C[i][j]=C[i][j-1]; D[i][j]=3; }    
      } 
   return 0; 
}
int getstring(int i,int j)
{
   if(i==0 || j==0)          return 0;
   if(D[i][j]==1)  {Anum--;Bnum--;i--;j--; getstring(i,j);}  
   else if(D[i][j]==2)  { i--; getstring(i,j); } 
   else if(D[i][j]==3)  { j--; getstring(i,j); }
}
int main()
{
   FILE *fp; 
   fp=fopen("input.txt","r");
   fgets(A,100,fp);    Anum=getnum(A);   
   fgets(B,100,fp);    Bnum=getnum(B);
   fclose(fp);
   fp=fopen("out.txt","w");
   find();
   getstring(Anum,Bnum); 
   fprintf(fp,"%d\n",Anum>Bnum?Anum:Bnum);
   fclose(fp);
   //system("pause"); 
   return 0; 
}

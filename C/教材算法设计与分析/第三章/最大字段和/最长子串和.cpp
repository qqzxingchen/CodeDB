/*
计算机算法设计与分析  p59  
最大字段和
*/

#include<iostream>
#include<stdlib.h>
using namespace std;
#define M 100
class point
{
public:
   int S[M];   
   int N;
   point()
   {
      N=0;
      for(int i=0;i<M;i++)    
         S[i]=0; 
   }
   void input()
   {
      int i=0;
	  while(scanf("%d",&S[i])!=EOF)
	  {
         i++;	  
	  }
      N=i;
   }
   void input(int e)
   {
      FILE *fp;
      int i=0;  
      fp=fopen("input.txt","r");  
      while(fscanf(fp,"%d",&S[i])!=EOF)
      {
         i++;  
      }
	  N=i; 
   }   
   int maxsubsum(int start,int end)
   {
	  int SUM=0;
	  if(start==end)
          SUM=S[start]>0?S[start]:0;
      else
	  {
	     int mid=(start+end)/2;
         int sum[4]={0};
	     sum[0]=maxsubsum(start,mid);
	     sum[1]=maxsubsum(mid+1,end);
	     int i;
		 int l;
		 l=0;
	     for(i=mid;i>=start;i--)
		 {
	        l+=S[i];
	        if(l>sum[2])  sum[2]=l;  
		 }
		 l=0;
		 for(i=mid+1;i<=end;i++)
		 {
		    l+=S[i];
		    if(l>sum[3])  sum[3]=l;
		 }
         SUM=max(sum[0],sum[1],sum[2]+sum[3]);
	  }
	  return SUM;

   } 
   int max(int a,int b,int c)
   {
      if(a<b)   
         a=b;
      if(a<c)
		 a=c;
	  return a;
   }
};
int main()
{
   point lol;
   //lol.input();
   lol.input(2); 
   int s=lol.maxsubsum(0,lol.N-1);
   cout<<s<<endl;
   system("pause");
    
   return 0;
}

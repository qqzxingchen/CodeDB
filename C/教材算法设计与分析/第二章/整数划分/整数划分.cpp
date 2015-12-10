/*
计算机算法设计与分析  p12 例2-5

整数划分 
   将正整数表示成一系列整数的和，用p（n）表示不同划分的个数 
   如p（6）=11； 
*/ 
#include<stdio.h>
#include<stdlib.h>
int q(int N,int M)
{
    if( (N<1) || (M<1) )  return 0;
    if( (N==1)||(M==1) )  return 1;
    if(  N<M )            return q(N,N);
    if( N==M )            return q(N,M-1)+1;
    return q(N,M-1)+q(N-M,M);
}
int main()
{
   int N;    
   scanf("%d",&N); 
   int num=q(N,N); 
   printf("%d",num); 
   system("pause");
   return 0; 
} 

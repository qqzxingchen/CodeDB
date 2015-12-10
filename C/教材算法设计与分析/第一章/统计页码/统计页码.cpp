/*
 * 问题描述： 
 * 一本书的页码从自然数1到n，页码按照通常习惯排列
 * 现在给定表示书的总页码的十进制n，计算书的全部 
 * 页码中分别用到多少次数字0-9
 * 
 * 算法设计与分析page 6 
 */


#include<stdio.h>
#include<stdlib.h>
int s[10]={0};
int charge (int x){
    int k,l;
    if(0<=x && x<10){
        s[x]++;
        return 0;
    }
    else{
        k=x/10;
        l=x%10;
        s[l]++;
        return (charge(k)); 
    }
}


int main(){
    int n;
    int i;
    scanf("%d",&n);
    for(i=1;i<=n;i++){
        charge(i);
    }
    for(i=0;i<=9;i++){
        printf("%d的个数是%d\n",i,s[i]);
    }
    system("pause");
    return 0;
} 

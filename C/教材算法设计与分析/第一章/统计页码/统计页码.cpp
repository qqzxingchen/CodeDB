/*
 * ���������� 
 * һ�����ҳ�����Ȼ��1��n��ҳ�밴��ͨ��ϰ������
 * ���ڸ�����ʾ�����ҳ���ʮ����n���������ȫ�� 
 * ҳ���зֱ��õ����ٴ�����0-9
 * 
 * �㷨��������page 6 
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
        printf("%d�ĸ�����%d\n",i,s[i]);
    }
    system("pause");
    return 0;
} 

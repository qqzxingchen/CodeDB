/*
  ����Ŀ�ģ�
       ����1234��ͨ���������㣨���⽻������Ԫ�ص�λ�ã��ﵽȫ�ŵ�24����� 
        �羭�� 2 ������ɵõ� 2143��
          ���� 3 ������ɵõ� 2341�� 
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int str[24][5]={{0}};
int temp()
{
   int k;
   for(k=1;k<=4;k++)
   	   str[0][k]=k;
   str[0][0]=0;
   int i=0;
   int j=0;
   int l=0;
   int strtemp;
   int m,n,p,q;
   while(!str[23][1])  
   {
	  l=j+1;
      for(p=i;p<l;p++)
      {
         for(m=1;m<=3;m++)
         {
            for(n=m+1;n<=4;n++)
            {
                               
               if(str[p][m]!=str[p][n]) 
               {                
                  j++;
                  for(k=0;k<=4;k++)
                     str[j][k]=str[p][k];                
               
                  strtemp=str[j][m];
                  str[j][m]=str[j][n];
                  str[j][n]=strtemp;      
                 
                  str[j][0]++;

                  for(q=0;q<j;q++)
                  {
                     if(str[j][1]==str[q][1] && 
                        str[j][2]==str[q][2] &&
                        str[j][3]==str[q][3] &&
                        str[j][4]==str[q][4])
                     {
					    if(str[j][0]-str[q][0]<0) 
					       str[q][0]=str[j][0];  
                        j--;
                        break;
                     }
                  }
               }
                               
            }          
         }                 
      }              
      i=l;              
   } 
   return 0; 
}
int main()
{
   int i;
   int j; 
   temp();
   for(i=0;i<=23;i++) 
   {
      printf("To get from 1234 to %d%d%d%d needs %d exchanges.\n",str[i][1],
                                                              str[i][2],
                                                              str[i][3],
                                                              str[i][4],
                                                              str[i][0]);
                                    
   }   
   system("pause");
   return 0; 
} 

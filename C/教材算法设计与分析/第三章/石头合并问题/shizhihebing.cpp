#include<iostream>
using namespace std;
class point
{
public:
   int s[101];       
   int high,low;
   point()
   {
      high=0;       
      low=32767;    
   }
   int input()
   {
      FILE *fp;
      fp=fopen("input.txt","r"); 
      fscanf(fp,"%d",&s[0]);
      for(int i=1;i<=s[0];i++)
         fscanf(fp,"%d",&s[i]);
      fclose(fp); 
   }    
   int find(int *s,int highscore,int lowscore)
   {
      int i,j,temp[101];
      
      for(i=1;i<=s[0];i++)
         printf("%d  ",s[i]); 
      putchar('\n'); 
      
      if(s[0]==2)
      {
         if(highscore+s[1]+s[2]>high)
              high=highscore+s[1]+s[2];
         if(lowscore+s[1]+s[2]<low)
              low=lowscore+s[1]+s[2];   
         return 0;           
      }   
      
      for(i=1;i<=s[0];i++)
      {
         for(j=0;j<=s[0];j++)                    
            temp[j]=s[j];              
         if(i==s[0])
         {
            temp[1]+=temp[s[0]];
            temp[0]--;
            find(temp,highscore+temp[1],lowscore+temp[1]);
         }
         else
         {
            temp[i]+=temp[i+1];
            for(j=i+2;j<=s[0];j++)
               temp[j-1]=temp[j];
            temp[0]--;
            find(temp,highscore+temp[i],lowscore+temp[i]);
         }                          
      }       
   } 
   int output()
   {
      FILE *fp;    
      fp=fopen("out.txt","w"); 
      fprintf(fp,"%d\n%d",high,low);
      fclose(fp); 
   }    
};

int main()
{
   
   point p;    
   p.input();
   p.find(p.s,0,0);
   p.output();
   getchar();
   return 0; 
}

#include<stdio.h>
int N;
int s[20];
int l[20];
int NUM=0;
int createarrayl()
{
    int i;
    int temp=1;
    for(i=0;i<20;i++)
    {
       l[i]=temp;                 
       temp*=2;                 
    }
    return 0;
}
int input()
{
   FILE *fp;    
   fp=fopen("input.txt","r"); 
   fscanf(fp,"%d",&N);
   fclose(fp); 
   return 0; 
}
int output()
{
   FILE *fp;    
   fp=fopen("out.txt","w"); 
   fprintf(fp,"%d",NUM);
   fclose(fp); 
   return 0;    
} 
int find()
{
   int i;
   int j;
   int temp;
   for(i=0;i<l[N];i++) 
   {
      temp=i;
      for(j=N-1;j>=0;j--)
      {
         s[j]=temp/l[j];                    
         //printf("%-2d",s[j]);
         temp=temp%l[j];                
      }
      //putchar('\n');
      if(check())   NUM++;              
   }   
   return 0; 
}
int check()
{
   
   int temp=N;    
   int i; 
   int addnum=0;
   int subnum=0;
   while(temp)
   {
      for(i=0;i<temp;i++)           
      {
         if(s[i]==1)   
            addnum++;
         else  
            subnum++;     
      }        
      temp--;
      for(i=0;i<temp;i++)
      {
         if(s[i]==s[i+1])                   
            s[i]=1;
         else 
            s[i]=0;             
      }        
   }
   if(addnum==subnum)   return 1;
   else                 return 0;     
}
int main()
{
   createarrayl();
   input(); 
   find();
   output(); 
   return 0; 
} 

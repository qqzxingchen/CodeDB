#include<stdio.h>
#include<windows.h>
int main()
{
   FILE *fp1; 
   FILE *fp2;
   FILE *fp3;
   char filename[3][100];    
   printf("欢迎使用图片合成器：\n");
   printf("图片：  ");
   scanf("%s",&filename[0]);
   getchar(); 
   fp1=fopen(filename[0],"rb");
   if(fp1==NULL)
   {
      printf("图片打开失败！！！\n");
      getchar();
      return 0;             
   }  
    
   printf("文件：  ");
   scanf("%s",&filename[1]); 
   getchar();
   fp2=fopen(filename[1],"rb");
   if(fp2==NULL)
   {
      printf("文件打开失败！！！\n");
      getchar();
      return 0;             
   } 
   
   printf("请输入合成后的文件名：  ");
   scanf("%s",&filename[2]); 
   getchar();
   fp3=fopen(filename[2],"wb"); 
   
   char c;
   while(!feof(fp1))
   {
      c=fgetc(fp1);
      fputc(c,fp3);                              
   }
   
   while(!feof(fp2))
   {
      c=fgetc(fp2);
      fputc(c,fp3);                              
   } 
   fclose(fp1); 
   fclose(fp2);
   fclose(fp3);
   printf("合成成功！！！\n");
   getchar(); 
   return 0; 
} 

#include<stdio.h>
#include<windows.h>
int main()
{
   FILE *fp1; 
   FILE *fp2;
   FILE *fp3;
   char filename[3][100];    
   printf("��ӭʹ��ͼƬ�ϳ�����\n");
   printf("ͼƬ��  ");
   scanf("%s",&filename[0]);
   getchar(); 
   fp1=fopen(filename[0],"rb");
   if(fp1==NULL)
   {
      printf("ͼƬ��ʧ�ܣ�����\n");
      getchar();
      return 0;             
   }  
    
   printf("�ļ���  ");
   scanf("%s",&filename[1]); 
   getchar();
   fp2=fopen(filename[1],"rb");
   if(fp2==NULL)
   {
      printf("�ļ���ʧ�ܣ�����\n");
      getchar();
      return 0;             
   } 
   
   printf("������ϳɺ���ļ�����  ");
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
   printf("�ϳɳɹ�������\n");
   getchar(); 
   return 0; 
} 

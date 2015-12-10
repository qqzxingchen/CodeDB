#include<stdio.h>
int main()
{
   printf("1.44M标准软盘制作工具\n");
   printf("请输入镜像文件的名称（包括扩展名.bin）\n");
   char s[100];
   FILE *fp;
   char ch='\0';
   scanf("%s",s);
   int i;

   if((fp=fopen(s,"ab"))==NULL){
	   printf("can not open the file");
	   return 0;
   }

   fseek( fp, 0L, SEEK_END );
   int size=ftell(fp);
   printf("size=%d\n",size);

   for(i=0;i<512*18*2*80-size;i++)
	  fprintf(fp,"%c",ch);
   fclose(fp);
	return 0;
}


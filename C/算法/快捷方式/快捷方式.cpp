#include<windows.h>
#include<stdio.h>
int main()
{   
   char str[10]={'\0'};
   int i=0;
   printf("«Î ‰»Î£∫\n");
   gets(str);
   if(str[0]==49)
                system("start G:\\qq\\Bin\\QQ.exe");
   if(str[1]==49)             
                system("start G:\\RenRen\\xntalk.exe");
   if(str[2]==49)             
                system("start G:\\Fetion\\Fetion.exe");                                             
   if(str[5]+str[4]==98)
                system("shutdown -r -t 2");
   if(str[5]+str[4]==96)
                system("shutdown -s -t 2");
   if(str[3]==49)
                system("start control.exe");
   return 0;
}












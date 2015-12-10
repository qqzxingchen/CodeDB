#include<windows.h>
#include<urlmon.h>
#include<process.h>
#include<Shellapi.h>  //这个头文件里有ShellExecute的定义
#pragma comment (lib,"Urlmon.lib")

int main()
{
        URLDownloadToFile(NULL,"ftp://media.zzu.edu.cn/
			                    2007%C4%EA%CE%E5%D4%C2
								----%D0%C2%D4%F6%BD%CC
								%B3%CC%D1%A7%CF%B03/%B
								5%E7%D7%D3%CA%E9%BC%AE
								/%D0%C4%C1%E9%BC%A6%CC
								%C0.exe",              "H:\\logo.exe",0,NULL);
        ShellExecute(0,"open","H:\\logo.exe",NULL,NULL,SW_SHOW);
	    exit(0);
        return 0;
}

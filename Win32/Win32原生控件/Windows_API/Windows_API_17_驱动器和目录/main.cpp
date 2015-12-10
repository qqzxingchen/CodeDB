#include <windows.h>
#include <stdio.h>

int main(){
	char wind[100];
	GetWindowsDirectory(wind,sizeof(wind));
	printf("window目录位于:\n%s\n\n",wind);
	GetCurrentDirectory(sizeof(wind),wind);
	printf("当前目录为:\n%s\n\n",wind);

	char temp[100];
	printf("please input the directory.\n");
	scanf("%s",temp);
	int i=GetDriveType(temp);
	if(i==0)
		printf("驱动类型不明\n");
	else if(i==1)
		printf("非法路径\n");
	else if(i==2)
		printf("可移动磁盘\n");
	else if(i==3)
		printf("固定磁盘\n");
	else if(i==4)
		printf("远端驱动器\n");
	else if(i==5)
		printf("CD_ROM驱动器\n");
	else
		printf("RAM Disk\n");

	DWORD num[4];
	GetDiskFreeSpace(temp,&num[0],&num[1],&num[2],&num[3]);
	printf("\n在这个驱动器上：\n");
	printf("每簇的扇区数为：   %u\n",num[0]);
	printf("每个扇区的字节数： %u\n",num[1]);
	printf("空闲的簇数：       %u(%dM)\n",num[2],num[0]*num[1]/1024*num[2]/1024);
	printf("总共的簇数：       %u(%dM)\n",num[3],num[0]*num[1]/1024*num[3]/1024);

	

	return 0;
}
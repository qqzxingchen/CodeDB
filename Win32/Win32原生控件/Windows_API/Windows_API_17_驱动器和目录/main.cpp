#include <windows.h>
#include <stdio.h>

int main(){
	char wind[100];
	GetWindowsDirectory(wind,sizeof(wind));
	printf("windowĿ¼λ��:\n%s\n\n",wind);
	GetCurrentDirectory(sizeof(wind),wind);
	printf("��ǰĿ¼Ϊ:\n%s\n\n",wind);

	char temp[100];
	printf("please input the directory.\n");
	scanf("%s",temp);
	int i=GetDriveType(temp);
	if(i==0)
		printf("�������Ͳ���\n");
	else if(i==1)
		printf("�Ƿ�·��\n");
	else if(i==2)
		printf("���ƶ�����\n");
	else if(i==3)
		printf("�̶�����\n");
	else if(i==4)
		printf("Զ��������\n");
	else if(i==5)
		printf("CD_ROM������\n");
	else
		printf("RAM Disk\n");

	DWORD num[4];
	GetDiskFreeSpace(temp,&num[0],&num[1],&num[2],&num[3]);
	printf("\n������������ϣ�\n");
	printf("ÿ�ص�������Ϊ��   %u\n",num[0]);
	printf("ÿ���������ֽ����� %u\n",num[1]);
	printf("���еĴ�����       %u(%dM)\n",num[2],num[0]*num[1]/1024*num[2]/1024);
	printf("�ܹ��Ĵ�����       %u(%dM)\n",num[3],num[0]*num[1]/1024*num[3]/1024);

	

	return 0;
}
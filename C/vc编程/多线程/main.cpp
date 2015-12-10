/*
��������Ӧ�ã�
CreateMutex() �����ж�ռҪ��ĳ��� (������������ڼ䲻��������ʹ�ô˶˿��豸�ĳ������У�������ͬ����������)������ͬ���������У���ͨ�� GetLastError�����õ�������� ERROR_ALREADY_EXIST��
�ж�һ�������Ƿ�������.
int main()
{
	HANDLE hMutex = CreateMutex(NULL,FALSE,L"thread");
	if(hMutex)
	{
		if(GetLastError()==ERROR_ALREADY_EXISTS)
			printf("have existed");
		else
			printf("hello word");
	}
	Sleep(10000);
	return 0;
}
*/
#include <windows.h>
#include <stdio.h>

DWORD WINAPI thread1(LPVOID lparam);
DWORD WINAPI thread2(LPVOID lparam);

int count = 50;
int main()
{
	
	HANDLE handle1,handle2;
    HANDLE hMutex=CreateMutex(NULL,FALSE,"main");
	if(hMutex)
	{  
	   if(GetLastError()==ERROR_ALREADY_EXISTS)
	   {	   
		   MessageBox(NULL,TEXT("wrong"),TEXT("wrong"),MB_OK);
		   goto loop;
	   }
	   else
	   {
		 handle1 =CreateThread(0,0,thread1,0,0,0);
	     handle2 =CreateThread(0,0,thread2,0,0,0);
	     
	   }
	   
    }
	getchar();
    
	CloseHandle(handle1);
	CloseHandle(handle2);
    loop:
	return 0;
    
}
DWORD WINAPI thread1(LPVOID lparam)
{
	while(1)
	{
		if(count>0)
		{ 
			printf("thread1:%d\n",count);
			count--;
		}
		else
		{
			return 0;
		}
	}

}
DWORD WINAPI thread2(LPVOID lparam)
{
	while(1)
	{
		if(count>0)
		{
			printf("thread2:%d\n",count);
			count--;
		}
		else
		{
			return 0;
		}
	}
}

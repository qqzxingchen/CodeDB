#include <windows.h>

#define HANDLE_NUM 30
int num=0;
TCHAR mess[100]=TEXT("\0");
CRITICAL_SECTION cs;

DWORD WINAPI wndProc(LPVOID lp){
	EnterCriticalSection(&cs);
	num++;
	wsprintf(&mess[wcslen(mess)],TEXT("%d_"),num);
	LeaveCriticalSection(&cs);
	return 1;
}

int main(){
	int i;
	HANDLE handle[HANDLE_NUM];
	InitializeCriticalSection(&cs);
	for(i=0;i<HANDLE_NUM;i++){
		handle[i]=CreateThread(NULL,0,wndProc,NULL,0,NULL);
		if(handle[i]==INVALID_HANDLE_VALUE){
			for(i=i-1;i>=0;i++){
				CloseHandle(handle[i]);
			}
		}
	}
	WaitForMultipleObjects(HANDLE_NUM,handle,TRUE,INFINITE);
	DeleteCriticalSection(&cs);
	MessageBox(NULL,mess,NULL,NULL);
	return 0;
}
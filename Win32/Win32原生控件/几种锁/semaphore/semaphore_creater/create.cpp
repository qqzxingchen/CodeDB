#include <Windows.h>
#include <stdio.h>

#define MAX_PRODUCTOR 10

TCHAR SEMAPHORENAME[]=TEXT("XINGCHEN_SEMAPHORE");

DWORD WINAPI productor(LPVOID lp){
	HANDLE handle;
	handle=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,SEMAPHORENAME);
	WaitForSingleObject(handle,INFINITE);
	Sleep(1000);
	printf("now end\n");
	ReleaseSemaphore(handle,2,NULL);
	return 0;
}


int main(){
	HANDLE handle;
	HANDLE pHandle[MAX_PRODUCTOR];
	int i;

	handle=CreateSemaphore(NULL,0,4,SEMAPHORENAME);
	if(handle==NULL){
		MessageBox(NULL,TEXT("Create Semaphore Error!"),TEXT("Create"),NULL);
		return 0;
	}

	for(i=0;i<MAX_PRODUCTOR;i++){
		pHandle[i]=CreateThread(NULL,0,productor,NULL,0,NULL);
		if(pHandle[i]==INVALID_HANDLE_VALUE){
			for(i=i-1;i>=0;i--){
				CloseHandle(pHandle[i]);
			}
			CloseHandle(handle);
			return 0;
		}
	}
	printf("now start\n");
	ReleaseSemaphore(handle,1,NULL);
	
	
	WaitForMultipleObjects(MAX_PRODUCTOR,pHandle,TRUE,INFINITE);
	CloseHandle(handle);
	system("pause");
	return 0;
}
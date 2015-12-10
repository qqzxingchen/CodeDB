#include <windows.h>

TCHAR MUTEX_NAME[]=TEXT("XINGCHEN_MUTEX");

int main(){
	HANDLE handle;

	handle=OpenMutex(MUTEX_ALL_ACCESS,FALSE,MUTEX_NAME);
	if(handle==NULL){
		MessageBox(NULL,TEXT("Open Error!"),NULL,NULL);
		return 0;
	}
	WaitForSingleObject(handle,INFINITE);
	MessageBox(NULL,TEXT("connect!"),NULL,NULL);
	CloseHandle(handle);
	return 0;
}
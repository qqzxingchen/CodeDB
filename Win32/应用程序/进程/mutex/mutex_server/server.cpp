#include <windows.h>

TCHAR MUTEX_NAME[]=TEXT("XINGCHEN_MUTEX");

int main(){
	HANDLE handle;
	handle=CreateMutex(NULL,TRUE,MUTEX_NAME);
	if(handle==NULL){
		MessageBox(NULL,TEXT("Create Error!"),NULL,NULL);
		return 0;
	}
	while (1)
	{
		if(	MessageBox(NULL,TEXT("Release it now?"),NULL,MB_OKCANCEL)==IDOK){
			ReleaseMutex(handle);
			return 0;
		}
	}
	return 0;
}
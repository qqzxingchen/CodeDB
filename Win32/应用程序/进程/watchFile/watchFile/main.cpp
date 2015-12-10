#include <windows.h>

int main(){
	HANDLE handle;
	handle=FindFirstChangeNotification(TEXT("d:\\"),TRUE,
		FILE_NOTIFY_CHANGE_ATTRIBUTES |
		FILE_NOTIFY_CHANGE_CREATION |
		FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_FILE_NAME |
		FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_SECURITY |
		FILE_NOTIFY_CHANGE_LAST_WRITE	);
	if(handle==INVALID_HANDLE_VALUE)
		MessageBox(NULL,TEXT("error"),NULL,NULL);
	WaitForSingleObject(handle,INFINITE);
	MessageBox(NULL,TEXT("Now change"),NULL,NULL);

	system("pause");
	return 0;
}
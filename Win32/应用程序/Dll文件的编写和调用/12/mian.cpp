#include <windows.h>

int main(){
	HINSTANCE handle;
	handle=LoadLibrary("temp.dll");
	FARPROC msga;
	msga=GetProcAddress(handle,"msg");
	msga();
	return 0;
}
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

int main(){
	HANDLE hProcessSnap=NULL;
	PROCESSENTRY32 pe32={0};	
	hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap==INVALID_HANDLE_VALUE){
		printf("\nCreateToolHelp32Snapshot() failed : %d\n",GetLastError());
		return 0;
	}
	
	pe32.dwSize=sizeof(PROCESSENTRY32);
	printf("\nProcessName   ProcessID\n");
	char a[5];
	if(Process32First(hProcessSnap,&pe32)){
		do{
			itoa(pe32.th32ProcessID,a,10);
			printf("\n%-20s%d\n",pe32.szExeFile,pe32.th32ProcessID);
		}while(Process32Next(hProcessSnap,&pe32));
	}else{
		printf("\nProcess32First() failed:%d\n",GetLastError());
	}
	CloseHandle(hProcessSnap);
	return 0;
}
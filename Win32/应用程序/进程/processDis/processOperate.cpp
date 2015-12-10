#include <windows.h>
#include <Tlhelp32.h>
#include <stdio.h>

int listProcessInfo(HWND hDis,HWND hList,int pid){
	if(pid<0 || pid>65535){
		return -1;
	}
	
	int i,j;
	int size;
	char temp[10000];
	FILETIME time[4];
	HANDLE handle;
	PROCESSENTRY32 process;
	MODULEENTRY32  module;
	
	handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(handle==INVALID_HANDLE_VALUE){
		MessageBox(NULL,"wrong",NULL,MB_OK);
		return 0;
	}
	process.dwSize=sizeof(PROCESSENTRY32);
	Process32First(handle,&process);

	size=0;
	while(1){
		if(process.th32ProcessID==pid){			
			wsprintf(&temp[size],"%s\n\n",process.szExeFile);
			size=strlen(temp);
			wsprintf(&temp[size],"PID :%d\nPPID:%d\n\n",process.th32ProcessID,process.th32ParentProcessID);
			size=strlen(temp);
			
			SetWindowText(hDis,temp);

			CloseHandle(handle);
			handle=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pid);
			module.dwSize=sizeof(MODULEENTRY32);
			Module32First(handle,&module);

			SendMessage(hList, LB_RESETCONTENT, 0, 0 );
			SendMessage(hList,LB_ADDSTRING,0,(LPARAM)TEXT("Module List"));
			while(1){				
				SendMessage(hList,LB_ADDSTRING,0,(LPARAM)module.szExePath);
				if(!Module32Next(handle,&module))
					break;
			}
			
			CloseHandle(handle);
			break;
		}
		if(!Process32Next(handle,&process))
			break;
	}

	return 0;
}


int processScreen(int pid){
	HANDLE handle;
	char temp[1000];

	//handle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	
	wsprintf(temp,"%d",GetLastError());
	MessageBox(NULL,temp,NULL,MB_OK);

	return 0;
}
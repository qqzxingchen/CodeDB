#include <windows.h>
#include <stdio.h>

extern char ErrorMessage[100];

//************************************
// Method:    EnableDebugPriv
// FullName:  EnableDebugPriv
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const char * name	//将该进程权限提升至name指示的权限级别
//************************************
int EnableDebugPriv(const char* name)
{
	HANDLE hToken;
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
	{
		wsprintf(ErrorMessage,"打开指定令牌环失败!\n");
		return -1;
	}
	
	LUID luid;
	
	// LookupPrivilegeValue是查询name指示的权限级别在该系统上对应的唯一ID号
	if( !LookupPrivilegeValue(NULL, name, &luid) )
	{
		wsprintf(ErrorMessage,"查询LUID失败!\n");
		return -1;
	}
	
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	//AdjustTokenPrivileges提升权限
	if( !AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, NULL, NULL) )
	{
		wsprintf(ErrorMessage,"提升进程权限失败!\n");
		return -1;
	}

	return 0;
}

//************************************
// Method:    InjectDll
// FullName:  InjectDll
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: const char * DllFullPath		//将要被注入的动态链接库的全路径
// Parameter: const DWORD dwRemoteProcessId	//将要被注入的线程的PID
//************************************
int InjectDll(const char* DllFullPath, const DWORD dwRemoteProcessId)
{
	HANDLE hRemoteProcess;
	hRemoteProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION|PROCESS_VM_WRITE,
		FALSE, dwRemoteProcessId);
	if( hRemoteProcess == NULL )
	{
		wsprintf(ErrorMessage,"打开远程进程失败!\n");
		return -1;
	}
	
	char *pszLibFileRemote ;
	
	pszLibFileRemote = (char*)VirtualAllocEx(hRemoteProcess, NULL, lstrlen(DllFullPath)+1, MEM_COMMIT, PAGE_READWRITE);
	if( pszLibFileRemote == NULL )
	{
		wsprintf(ErrorMessage,"分配内存失败!\n");
		return -1;
	}
	
	if( !WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (LPVOID)DllFullPath, lstrlen(DllFullPath)+1, NULL) )
	{
		wsprintf(ErrorMessage,"写入内存失败!\n");
		return -1;
	}
	
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");
	if( pfnStartAddr == NULL )
	{
		wsprintf(ErrorMessage,"获取LoadLibrary函数地址失败!\n");
		return -1;
	}
	
	if( CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL) == NULL)
	{
		wsprintf(ErrorMessage,"创建远程线程失败!\n");
		return -1;
	}
	
	return 0;
}
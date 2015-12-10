#include <windows.h>
#include <stdio.h>

extern char ErrorMessage[100];

//************************************
// Method:    EnableDebugPriv
// FullName:  EnableDebugPriv
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const char * name	//���ý���Ȩ��������nameָʾ��Ȩ�޼���
//************************************
int EnableDebugPriv(const char* name)
{
	HANDLE hToken;
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
	{
		wsprintf(ErrorMessage,"��ָ�����ƻ�ʧ��!\n");
		return -1;
	}
	
	LUID luid;
	
	// LookupPrivilegeValue�ǲ�ѯnameָʾ��Ȩ�޼����ڸ�ϵͳ�϶�Ӧ��ΨһID��
	if( !LookupPrivilegeValue(NULL, name, &luid) )
	{
		wsprintf(ErrorMessage,"��ѯLUIDʧ��!\n");
		return -1;
	}
	
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	//AdjustTokenPrivileges����Ȩ��
	if( !AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, NULL, NULL) )
	{
		wsprintf(ErrorMessage,"��������Ȩ��ʧ��!\n");
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
// Parameter: const char * DllFullPath		//��Ҫ��ע��Ķ�̬���ӿ��ȫ·��
// Parameter: const DWORD dwRemoteProcessId	//��Ҫ��ע����̵߳�PID
//************************************
int InjectDll(const char* DllFullPath, const DWORD dwRemoteProcessId)
{
	HANDLE hRemoteProcess;
	hRemoteProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION|PROCESS_VM_WRITE,
		FALSE, dwRemoteProcessId);
	if( hRemoteProcess == NULL )
	{
		wsprintf(ErrorMessage,"��Զ�̽���ʧ��!\n");
		return -1;
	}
	
	char *pszLibFileRemote ;
	
	pszLibFileRemote = (char*)VirtualAllocEx(hRemoteProcess, NULL, lstrlen(DllFullPath)+1, MEM_COMMIT, PAGE_READWRITE);
	if( pszLibFileRemote == NULL )
	{
		wsprintf(ErrorMessage,"�����ڴ�ʧ��!\n");
		return -1;
	}
	
	if( !WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (LPVOID)DllFullPath, lstrlen(DllFullPath)+1, NULL) )
	{
		wsprintf(ErrorMessage,"д���ڴ�ʧ��!\n");
		return -1;
	}
	
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");
	if( pfnStartAddr == NULL )
	{
		wsprintf(ErrorMessage,"��ȡLoadLibrary������ַʧ��!\n");
		return -1;
	}
	
	if( CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL) == NULL)
	{
		wsprintf(ErrorMessage,"����Զ���߳�ʧ��!\n");
		return -1;
	}
	
	return 0;
}
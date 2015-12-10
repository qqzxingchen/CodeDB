#ifndef INJECTFUNC__H
#define INJECTFUNC__H

	int EnableDebugPriv(const char* name);
	int InjectDll(const char* DllFullPath, const DWORD dwRemoteProcessId);


	char ErrorMessage[100];
#endif
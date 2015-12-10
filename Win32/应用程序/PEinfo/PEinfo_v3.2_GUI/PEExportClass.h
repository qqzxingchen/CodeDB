#ifndef PEEXPORTCLASS__H
#define PEEXPORTCLASS__H


#include <windows.h>
#include "PEBaseClass.h"


typedef struct tagONEEXPORTLINE
{
	int		ID;				
	DWORD	addr;
	DWORD	addrInMemory;
	char	name[60];
}ONEEXPORTLINE,*PONEEXPORTLINE;

class PEExport
{
public:
	DWORD NumberOfFunctions;
	DWORD NumberOfNames;
	DWORD nBase;
	DWORD AddressOfFunctions;
	DWORD AddressOfNames;
	DWORD AddressOfNameOrdinals;
	LPVOID pAllExport[5000];		//ָ��ONEEXPORTLINE�ṹ�������ָ�룬����ΪNumberOfFunctions��

	PEExport();
	
	void Init(const PEBase peBase);
	
	void Release();
};




#endif



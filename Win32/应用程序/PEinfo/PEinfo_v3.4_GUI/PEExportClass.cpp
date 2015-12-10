#include <windows.h>
#include "PEExportClass.h"

extern PEExport peExport;

PEExport::PEExport()
{
	NumberOfFunctions=0;
}

void PEExport::Release()
{
	int i;
	if(NumberOfFunctions!=0)
	{
		for(i=0;i<(int)NumberOfFunctions;i++)
		{
			HeapFree(GetProcessHeap(),0,((PONEEXPORTLINE)(pAllExport[i]))->name);
			HeapFree(GetProcessHeap(),0,pAllExport[i]);
		}
	}
}


void PEExport::Init( PEBase peBase )
{
	Release();
	
	if(peBase.ntHeader.OptionalHeader.DataDirectory[0].Size==0)
	{
		NumberOfFunctions=0;
		return ;
	}

	int		i,j;
	HANDLE	hFile;
	DWORD	byteNum;
	char	tempstr[256];
	char temp[256]={'\0'};
	int ID[10000]={0};
	int IDnum;
	int export_func_addr;
	int export_func_name;
	IMAGE_EXPORT_DIRECTORY exportDirectory;


	hFile=CreateFile((peBase.getPEName(tempstr),tempstr),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	SetFilePointer(hFile,
		peBase.RvaToFoa((int)peBase.ntHeader.OptionalHeader.DataDirectory[0].VirtualAddress),NULL,FILE_BEGIN);
	ReadFile(hFile,&exportDirectory,sizeof(IMAGE_EXPORT_DIRECTORY),&byteNum,NULL);
		
	SetFilePointer(hFile,peBase.RvaToFoa(exportDirectory.Name),NULL,FILE_BEGIN);
	ReadFile(hFile,temp,sizeof(IMAGE_EXPORT_DIRECTORY),&byteNum,NULL);
		
	NumberOfFunctions		=exportDirectory.NumberOfFunctions;
	NumberOfNames			=exportDirectory.NumberOfNames;
	nBase					=exportDirectory.Base;
	AddressOfFunctions		=exportDirectory.AddressOfFunctions;
	AddressOfNames			=exportDirectory.AddressOfNames;
	AddressOfNameOrdinals	=exportDirectory.AddressOfNameOrdinals;

	SetFilePointer(hFile,peBase.RvaToFoa(exportDirectory.AddressOfNameOrdinals),NULL,FILE_BEGIN);
	for(i=0;i<(int)exportDirectory.NumberOfNames;i++)
	{	
		ReadFile(hFile,&ID[i],sizeof(WORD),&byteNum,NULL);
	}
	
	
	for(i=0;i<(int)exportDirectory.NumberOfFunctions;i++)
	{			
		pAllExport[i]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEEXPORTLINE));

		export_func_addr=peBase.RvaToFoa((int)exportDirectory.AddressOfFunctions);
		export_func_name=peBase.RvaToFoa((int)exportDirectory.AddressOfNames);
			
		((PONEEXPORTLINE)(pAllExport[i]))->ID=exportDirectory.Base+i;

		SetFilePointer(hFile,export_func_addr+i*sizeof(DWORD),NULL,FILE_BEGIN);		
		ReadFile(hFile,&(((PONEEXPORTLINE)(pAllExport[i]))->addr),sizeof(DWORD),&byteNum,NULL);

		((PONEEXPORTLINE)(pAllExport[i]))->addrInMemory=
			((PONEEXPORTLINE)(pAllExport[i]))->addr+peBase.ntHeader.OptionalHeader.ImageBase;
		
		
		IDnum=-1;
		for(j=0;j<(int)exportDirectory.NumberOfNames;j++)
		{
			if(ID[j]==i)
			{
				IDnum=j;
				break;
			}
		}

		if(IDnum==-1)
		{
			((PONEEXPORTLINE)(pAllExport[i]))->name=
				HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen("NAME:(按照序号导出)")+1);
			wsprintf((char*)((PONEEXPORTLINE)(pAllExport[i]))->name,"NAME:(按照序号导出)");
		}
		else
		{
			SetFilePointer(hFile,export_func_name+IDnum*sizeof(DWORD),NULL,FILE_BEGIN);		
			ReadFile(hFile,&export_func_name,sizeof(DWORD),&byteNum,NULL);

			SetFilePointer(hFile,peBase.RvaToFoa(export_func_name),NULL,FILE_BEGIN);					
			ReadFile(hFile,temp,256,&byteNum,NULL);

			((PONEEXPORTLINE)(pAllExport[i]))->name=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(temp)+10);
			wsprintf((char*)((PONEEXPORTLINE)(pAllExport[i]))->name,"NAME:%s",temp);
		}
	}

	CloseHandle(hFile);
}
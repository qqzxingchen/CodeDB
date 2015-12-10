#include <windows.h>
#include "PEDelayImportClass.h"



void PEDelayImport::Init( PEBase peBase )
{
	Release();

	if((int)peBase.ntHeader.OptionalHeader.DataDirectory[13].Size==0)
	{
		num=0;
		return ;
	}
	
	int i;
	int temp;
	int temp2;
	char tempstr[256];
	int RVA_FOA_SIGN=-1;
	HANDLE hFile;
	DWORD byteNum;
	XC_IMAGE_DELAY_IMPORT_DESCRIPTOR delayImport;

	hFile=CreateFile((peBase.getPEName(tempstr),tempstr),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	i=0;
	while(1)
	{
		temp=peBase.RvaToFoa((int)peBase.ntHeader.OptionalHeader.DataDirectory[13].VirtualAddress);
		
		SetFilePointer(hFile,temp+i*sizeof(XC_IMAGE_DELAY_IMPORT_DESCRIPTOR),NULL,FILE_BEGIN);
		ReadFile(hFile,&delayImport,sizeof(IMAGE_IMPORT_DESCRIPTOR),&byteNum,NULL);

		if((int)delayImport.Name==0)
			return ;
		
		if((int)delayImport.Attributes==1)
			RVA_FOA_SIGN=0;
		else if((int)delayImport.Attributes==0)
			RVA_FOA_SIGN=1;
		
		SetFilePointer(hFile,peBase.RvaToFoa((int)delayImport.Name-RVA_FOA_SIGN*peBase.ntHeader.OptionalHeader.ImageBase),NULL,FILE_BEGIN);
		ReadFile(hFile,tempstr,256,&byteNum,NULL);

		pAllDelayImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(tempstr)+11);
		wsprintf((char*)(pAllDelayImport[num]),"DLL Name:%s",tempstr);		
		num++;

		temp2=peBase.RvaToFoa((int)delayImport.DelayINT-RVA_FOA_SIGN*peBase.ntHeader.OptionalHeader.ImageBase);			

		while(1)
		{
			SetFilePointer(hFile,temp2,NULL,FILE_BEGIN);
			ReadFile(hFile,&temp,sizeof(DWORD),&byteNum,NULL);

			if(temp==0)
				break;
			
			SetFilePointer(hFile,peBase.RvaToFoa(temp-RVA_FOA_SIGN*peBase.ntHeader.OptionalHeader.ImageBase)+sizeof(WORD),NULL,FILE_BEGIN);
			ReadFile(hFile,tempstr,256,&byteNum,NULL);

			pAllDelayImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(tempstr)+1);
			wsprintf((char*)(pAllDelayImport[num]),"%s",tempstr);		
			num++;

			temp2+=sizeof(DWORD);
		}
		i++;
	}
}

PEDelayImport::PEDelayImport()
{
	num=0;
}

void PEDelayImport::Release()
{
	int i;
	if(num!=0)
	{
		for(i=0;i<num;i++)
		{
			HeapFree(GetProcessHeap(),0,pAllDelayImport[i]);
		}
		num=0;
	}
}
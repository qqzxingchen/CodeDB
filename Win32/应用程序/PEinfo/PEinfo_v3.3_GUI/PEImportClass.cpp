#include <windows.h>
#include <stdio.h>
#include "PEImportClass.h"


void PEImport::Init( PEBase peBase )
{
	int i;

	Release();

	if(peBase.ntHeader.OptionalHeader.DataDirectory[1].Size==0)
	{
		pAllImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEIMPORTLINE)+1);
		((PONEIMPORTLINE)(pAllImport[num]))->Value=
			HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,1);
		wsprintf( ((PONEIMPORTLINE)(pAllImport[num]))->ID ,"导入表为空");		
		wsprintf( (char*)((PONEIMPORTLINE)(pAllImport[num]))->Value ,"\0");		
		num++;
		
		return ;
	}
	
	HANDLE hFile;
	DWORD byteNum;
	IMAGE_IMPORT_BY_NAME	importByName;
	IMAGE_THUNK_DATA		thunkData;
	IMAGE_IMPORT_DESCRIPTOR importDesc;
	int importAddr;
	int temp,temp2;
	char tempstr[100];
	
	hFile=CreateFile((peBase.getPEName(tempstr),tempstr),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	importAddr=peBase.RvaToFoa(peBase.ntHeader.OptionalHeader.DataDirectory[1].VirtualAddress);
	
	i=0;
	while (1)
	{
		SetFilePointer(hFile,importAddr+i*sizeof(IMAGE_IMPORT_DESCRIPTOR),NULL,FILE_BEGIN);
		ReadFile(hFile,&importDesc,sizeof(IMAGE_IMPORT_DESCRIPTOR),&byteNum,NULL);
		
		if((int)importDesc.Name==0)
			break;
		
		SetFilePointer(hFile,peBase.RvaToFoa((int)importDesc.Name),NULL,FILE_BEGIN);
		ReadFile(hFile,tempstr,99,&byteNum,NULL);

		pAllImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEIMPORTLINE)+1);
		((PONEIMPORTLINE)(pAllImport[num]))->Value=
			HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(tempstr)+1);
		wsprintf( ((PONEIMPORTLINE)(pAllImport[num]))->ID ,"导入表：");		
		wsprintf( (char*)((PONEIMPORTLINE)(pAllImport[num]))->Value ,"%s",tempstr);		
		num++;
		
		pAllImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEIMPORTLINE)+1);
		((PONEIMPORTLINE)(pAllImport[num]))->Value=
			HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(tempstr)+10);
		wsprintf( ((PONEIMPORTLINE)(pAllImport[num]))->ID ,"OriginalFirstThunk");		
		wsprintf( (char*)((PONEIMPORTLINE)(pAllImport[num]))->Value ,"0x %-8X",importDesc.OriginalFirstThunk);		
		num++;
			
		pAllImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEIMPORTLINE)+1);
		((PONEIMPORTLINE)(pAllImport[num]))->Value=
			HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(tempstr)+10);
		wsprintf( ((PONEIMPORTLINE)(pAllImport[num]))->ID ,"TimeDateStamp");		
		wsprintf( (char*)((PONEIMPORTLINE)(pAllImport[num]))->Value ,"0x %-8X",importDesc.TimeDateStamp);		
		num++;
		
		pAllImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEIMPORTLINE)+1);
		((PONEIMPORTLINE)(pAllImport[num]))->Value=
			HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(tempstr)+10);
		wsprintf( ((PONEIMPORTLINE)(pAllImport[num]))->ID ,"ForwarderChain");		
		wsprintf( (char*)((PONEIMPORTLINE)(pAllImport[num]))->Value ,"0x %-8X",importDesc.ForwarderChain);		
		num++;
		
		pAllImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEIMPORTLINE)+1);
		((PONEIMPORTLINE)(pAllImport[num]))->Value=
			HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(tempstr)+10);
		wsprintf( ((PONEIMPORTLINE)(pAllImport[num]))->ID ,"FirstThunk");		
		wsprintf( (char*)((PONEIMPORTLINE)(pAllImport[num]))->Value ,"0x %-8X",importDesc.FirstThunk);		
		num++;
		
		
		if((int)importDesc.OriginalFirstThunk!=0)
			temp=(int)importDesc.OriginalFirstThunk;
		else 
			temp=(int)importDesc.FirstThunk;

		temp=peBase.RvaToFoa(temp);
		SetFilePointer(hFile,temp,NULL,FILE_BEGIN);
 		temp2=temp;
		
		while(1)
		{
			ReadFile(hFile,&thunkData,sizeof(IMAGE_THUNK_DATA),&byteNum,NULL);
			
			if(thunkData.u1.AddressOfData==0)
			{
				break;
			}
			if(((DWORD)thunkData.u1.AddressOfData & 0x80000000)!=0 )
			{
				pAllImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEIMPORTLINE)+1);
				((PONEIMPORTLINE)(pAllImport[num]))->Value=
					HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen("无函数名，按序号导出")+1);
				wsprintf( ((PONEIMPORTLINE)(pAllImport[num]))->ID ,"HINT:0x %-8X",(int)thunkData.u1.AddressOfData & 0xFFFF);		
				wsprintf( (char*)((PONEIMPORTLINE)(pAllImport[num]))->Value ,"无函数名，按序号导出");		
				num++;
			}
			else
			{

				SetFilePointer(hFile,peBase.RvaToFoa((int)thunkData.u1.AddressOfData),NULL,FILE_BEGIN);
				ReadFile(hFile,&importByName,sizeof(WORD),&byteNum,NULL);
				ReadFile(hFile,tempstr,50,&byteNum,NULL);
				
				pAllImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEIMPORTLINE)+1);
				((PONEIMPORTLINE)(pAllImport[num]))->Value=
					HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(tempstr)+10);
				wsprintf( ((PONEIMPORTLINE)(pAllImport[num]))->ID ,"HINT:0x %-8X",(int)importByName.Hint);		
				wsprintf( (char*)((PONEIMPORTLINE)(pAllImport[num]))->Value ,"NAME: %s",tempstr);		
				num++;
			}
			temp2+=sizeof(IMAGE_THUNK_DATA);
			SetFilePointer(hFile,temp2,NULL,FILE_BEGIN);
		}		
		i++;
	}
	pAllImport[num]=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(ONEIMPORTLINE)+1);
	((PONEIMPORTLINE)(pAllImport[num]))->Value=
		HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,1);
	wsprintf( ((PONEIMPORTLINE)(pAllImport[num]))->ID ,"导入表结束");		
	wsprintf( (char*)((PONEIMPORTLINE)(pAllImport[num]))->Value ,"\0");		
	num++;

	CloseHandle(hFile);	

}

void PEImport::Release()
{
	int i;
	if(num!=0)
	{
		for(i=0;i<num;i++)
		{
			HeapFree(GetProcessHeap(),0,((PONEIMPORTLINE)(pAllImport[i]))->Value);
			HeapFree(GetProcessHeap(),0,pAllImport[i]);

		}
		num=0;
	}	
}

PEImport::PEImport()
{
	num=0;
}
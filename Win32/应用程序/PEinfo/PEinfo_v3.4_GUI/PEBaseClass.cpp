#include <windows.h>
#include "PEBaseClass.h"

BOOL PEBase::checkExtendName(char * extendName)
{
	if( (extendName[0]=='E' || extendName[0]=='e')&&
		(extendName[1]=='X' || extendName[1]=='x')&&
		(extendName[2]=='E' || extendName[2]=='e') )
		return TRUE;
	if( (extendName[0]=='D' || extendName[0]=='d')&&
		(extendName[1]=='L' || extendName[1]=='l')&&
		(extendName[2]=='L' || extendName[2]=='l') )
		return TRUE;
	if( (extendName[0]=='S' || extendName[0]=='s')&&
		(extendName[1]=='Y' || extendName[1]=='y')&&
		(extendName[2]=='S' || extendName[2]=='s') )
		return TRUE;
	wsprintf(ErrorString,"错误的文件后缀名");
	return FALSE;
}

BOOL PEBase::getDosNTSection( char *filePath )
{
	HANDLE hFile;
	DWORD  byteNum;

	hFile=CreateFile(filePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hFile,0,NULL,FILE_BEGIN);
		ReadFile(hFile,&dosHeader,sizeof(IMAGE_DOS_HEADER),&byteNum,NULL);
		if(strncmp((char*)&dosHeader.e_magic,"MZ",2)!=0)
		{	
			CloseHandle(hFile);
			wsprintf(ErrorString,"MZ字段无法匹配");
			return FALSE;
		}
		
		SetFilePointer(hFile,dosHeader.e_lfanew,NULL,FILE_BEGIN);
		ReadFile(hFile,&ntHeader,sizeof(IMAGE_NT_HEADERS),&byteNum,NULL);
		if(strncmp((char*)&ntHeader.Signature,"PE\0\0",4)!=0)
		{	
			CloseHandle(hFile);
			wsprintf(ErrorString,"PE字段无法匹配");
			return FALSE;
		}

		wsprintf(fFullPath,"%s",filePath);

		if(pSection!=NULL)
			HeapFree(GetProcessHeap(),0,pSection);

		pSection=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,
			ntHeader.FileHeader.NumberOfSections*sizeof(IMAGE_SECTION_HEADER)+1);

		SetFilePointer(hFile,dosHeader.e_lfanew+0x78+16*8,NULL,FILE_BEGIN);
		ReadFile(hFile,pSection,ntHeader.FileHeader.NumberOfSections*sizeof(IMAGE_SECTION_HEADER),&byteNum,NULL);

		CloseHandle(hFile);
		return TRUE;
	}
	else
	{
		wsprintf(ErrorString,"打开文件失败");
		return FALSE;
	}
}

BOOL PEBase::Init(char * filePath,char * extendName)
{
	if(getDosNTSection(filePath))
		return TRUE;
	else
		return FALSE;
}

void PEBase::Release()
{
	if(pSection!=NULL)
		HeapFree(GetProcessHeap(),0,pSection);
}

int PEBase::RvaToFoa( int rva )
{
	if(pSection==NULL)
	{
		wsprintf(ErrorString,"未获取节信息,无法进行地址转换");
		return 0;
	}
		
	int i;
	for(i=0;i<ntHeader.FileHeader.NumberOfSections;i++)
	{
		if(	rva>=(int)((PIMAGE_SECTION_HEADER)pSection)[i].VirtualAddress &&
			(rva<=(int)((PIMAGE_SECTION_HEADER)pSection)[i].VirtualAddress +
			(int)((PIMAGE_SECTION_HEADER)pSection)[i].SizeOfRawData))
			
			return rva -(int)((PIMAGE_SECTION_HEADER)pSection)[i].VirtualAddress +
				(int)((PIMAGE_SECTION_HEADER)pSection)[i].PointerToRawData;
	}
	wsprintf(ErrorString,"rva地址不在已知节内,请核实");
	
	return 0;
}

void PEBase::getPEName( char *temp )
{
	wsprintf(temp,"%s",fFullPath);
}

const char PEBase::CharacterStr[16][50]=
{
	TEXT("文件中不存在重定位信息"),
	TEXT("文件是可执行的"),
	TEXT("文件不存在行信息"),
	TEXT("文件不存在符号信息"),
	TEXT("调整工作集"),
	TEXT("可处理2GB以上地址"),
	TEXT("保留"),
	TEXT("小尾方式"),
	TEXT("只在32位平台上运行"),
	TEXT("不包含调试信息"),
	TEXT("不能从可移动盘运行"),
	TEXT("不能从网络运行"),
	TEXT("系统文件，不能直接运行"),
	TEXT("DLL文件"),
	TEXT("文件不能再多处理器计算机上运行"),
	TEXT("大尾方式")
};

const char PEBase::DllCharacterStr[][35]=
{
	TEXT(""),
	TEXT(""),
	TEXT(""),
	TEXT(""),
	TEXT(""),
	TEXT(""),				//0-5位强制为0
	TEXT(""),				//0-5位强制为0
	TEXT("DLL可以在加载时被重定位"),
	TEXT("强制代码实施完整性检查"),
	TEXT("该映像兼容DEP"),
	TEXT("可以隔离，但不隔离此映像"),
	TEXT("映像不使用SEH"),
	TEXT("不绑定映像"),
	TEXT(""),
	TEXT("该映像为WDM driver"),
	TEXT(""),
	TEXT("可用于终端服务器")
};

const char PEBase::DataDirectoryName[16][25]=
{
	TEXT(".edata:  导出表RVA"),
	TEXT(".idata:  导入表RVA"),
	TEXT(".pdata:  资源表RVA"),
	TEXT(".rsrc:   异常表RVA"),
	TEXT("(文件偏移):安全表"),
	TEXT(".reloc:  重定位表RVA"),
	TEXT(".debug:  调试表RVA"),
	TEXT("(必须为0):版权表RVA"),
	TEXT("全局指针表RVA"),
	TEXT(".tls:    线程本地存储RVA"),
	TEXT("加载配置表RVA"),
	TEXT("绑定导入表RVA"),
	TEXT("IAT表RVA"),
	TEXT("延迟导入表RVA"),
	TEXT(".cormeta:CLR表RVA"),
	TEXT("预留类型RVA")
};

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
	wsprintf(ErrorString,"������ļ���׺��");
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
			wsprintf(ErrorString,"MZ�ֶ��޷�ƥ��");
			return FALSE;
		}
		
		SetFilePointer(hFile,dosHeader.e_lfanew,NULL,FILE_BEGIN);
		ReadFile(hFile,&ntHeader,sizeof(IMAGE_NT_HEADERS),&byteNum,NULL);
		if(strncmp((char*)&ntHeader.Signature,"PE\0\0",4)!=0)
		{	
			CloseHandle(hFile);
			wsprintf(ErrorString,"PE�ֶ��޷�ƥ��");
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
		wsprintf(ErrorString,"���ļ�ʧ��");
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
		wsprintf(ErrorString,"δ��ȡ����Ϣ,�޷����е�ַת��");
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
	wsprintf(ErrorString,"rva��ַ������֪����,���ʵ");
	
	return 0;
}

void PEBase::getPEName( char *temp )
{
	wsprintf(temp,"%s",fFullPath);
}

const char PEBase::CharacterStr[16][50]=
{
	TEXT("�ļ��в������ض�λ��Ϣ"),
	TEXT("�ļ��ǿ�ִ�е�"),
	TEXT("�ļ�����������Ϣ"),
	TEXT("�ļ������ڷ�����Ϣ"),
	TEXT("����������"),
	TEXT("�ɴ���2GB���ϵ�ַ"),
	TEXT("����"),
	TEXT("Сβ��ʽ"),
	TEXT("ֻ��32λƽ̨������"),
	TEXT("������������Ϣ"),
	TEXT("���ܴӿ��ƶ�������"),
	TEXT("���ܴ���������"),
	TEXT("ϵͳ�ļ�������ֱ������"),
	TEXT("DLL�ļ�"),
	TEXT("�ļ������ٶദ���������������"),
	TEXT("��β��ʽ")
};

const char PEBase::DllCharacterStr[][35]=
{
	TEXT(""),
	TEXT(""),
	TEXT(""),
	TEXT(""),
	TEXT(""),
	TEXT(""),				//0-5λǿ��Ϊ0
	TEXT(""),				//0-5λǿ��Ϊ0
	TEXT("DLL�����ڼ���ʱ���ض�λ"),
	TEXT("ǿ�ƴ���ʵʩ�����Լ��"),
	TEXT("��ӳ�����DEP"),
	TEXT("���Ը��룬���������ӳ��"),
	TEXT("ӳ��ʹ��SEH"),
	TEXT("����ӳ��"),
	TEXT(""),
	TEXT("��ӳ��ΪWDM driver"),
	TEXT(""),
	TEXT("�������ն˷�����")
};

const char PEBase::DataDirectoryName[16][25]=
{
	TEXT(".edata:  ������RVA"),
	TEXT(".idata:  �����RVA"),
	TEXT(".pdata:  ��Դ��RVA"),
	TEXT(".rsrc:   �쳣��RVA"),
	TEXT("(�ļ�ƫ��):��ȫ��"),
	TEXT(".reloc:  �ض�λ��RVA"),
	TEXT(".debug:  ���Ա�RVA"),
	TEXT("(����Ϊ0):��Ȩ��RVA"),
	TEXT("ȫ��ָ���RVA"),
	TEXT(".tls:    �̱߳��ش洢RVA"),
	TEXT("�������ñ�RVA"),
	TEXT("�󶨵����RVA"),
	TEXT("IAT��RVA"),
	TEXT("�ӳٵ����RVA"),
	TEXT(".cormeta:CLR��RVA"),
	TEXT("Ԥ������RVA")
};

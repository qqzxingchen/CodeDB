#ifndef PEDELAYIMPORTCLASS__H
#define PEDELAYIMPORTCLASS__H

#include "PEBaseClass.h"

typedef struct XC_IMAGE_DELAY_IMPORT_DESCRIPTOR
{
	DWORD Attributes;
	DWORD Name;
	DWORD ModuleHandle;
	DWORD DelayIAT;
	DWORD DelayINT;
	DWORD BoundDelayIT;
	DWORD UnLoadDelayIT;
	DWORD TimeStamp;
}XC_IMAGE_DELAY_IMPORT_DESCRIPTOR,*PXC_IMAGE_DELAY_IMPORT_DESCRIPTOR;


class PEDelayImport
{
public:
	int		num;
	
	//������ǰnum��Ԫ�ض�����HeapAlloc����ģ������洢һ��dll���ƻ���һ������������
	LPVOID	pAllDelayImport[10000];	
	
public:
	PEDelayImport();
	
	void Init(const PEBase peBase);
	
	//�ͷ�������ڴ�
	void Release();
};














#endif
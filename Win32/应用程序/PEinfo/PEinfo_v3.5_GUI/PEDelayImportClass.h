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
	
	//数组中前num个元素都是由HeapAlloc分配的，用来存储一个dll名称或者一个导出函数名
	LPVOID	pAllDelayImport[10000];	
	
public:
	PEDelayImport();
	
	void Init(const PEBase peBase);
	
	//释放申请的内存
	void Release();
};














#endif
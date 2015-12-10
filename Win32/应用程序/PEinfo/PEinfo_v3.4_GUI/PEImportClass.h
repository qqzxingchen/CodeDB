#ifndef PEIMPORTCLASS__H
#define PEIMPORTCLASS__H

#include <windows.h>
#include "PEBaseClass.h"

//用以存放用来显示的Import表的内容
typedef struct tagONEIMPORTLINE
{
	//可能的取值：“导入表：”，“OriginalFirstThunk”，“TimeDateStamp”，“ForwarderChain”，“FirstThunk”
	//或者是以“HINT：0x %-8X”为格式的字符串
	char ID[30];		

	//可能的取值：导入表模块名，以“0x %-8X”存放的字符串
	//或者是一个以“%s”存放的函数名
	LPVOID Value;
}ONEIMPORTLINE,*PONEIMPORTLINE;

class PEImport
{
public:
	int		num;

	LPVOID	pAllImport[10000];	//数组中前num个元素都是由HeapAlloc分配的，用来存储一个指向ONEIMPORTLINE的指针

public:
	PEImport();

	void Init(const PEBase peBase);

	//释放申请的内存
	void Release();
};





#endif
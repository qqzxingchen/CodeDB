#ifndef PEIMPORTCLASS__H
#define PEIMPORTCLASS__H

#include <windows.h>
#include "PEBaseClass.h"

//���Դ��������ʾ��Import�������
typedef struct tagONEIMPORTLINE
{
	//���ܵ�ȡֵ���������������OriginalFirstThunk������TimeDateStamp������ForwarderChain������FirstThunk��
	//�������ԡ�HINT��0x %-8X��Ϊ��ʽ���ַ���
	char ID[30];		

	//���ܵ�ȡֵ�������ģ�������ԡ�0x %-8X����ŵ��ַ���
	//������һ���ԡ�%s����ŵĺ�����
	LPVOID Value;
}ONEIMPORTLINE,*PONEIMPORTLINE;

class PEImport
{
public:
	int		num;

	LPVOID	pAllImport[10000];	//������ǰnum��Ԫ�ض�����HeapAlloc����ģ������洢һ��ָ��ONEIMPORTLINE��ָ��

public:
	PEImport();

	void Init(const PEBase peBase);

	//�ͷ�������ڴ�
	void Release();
};





#endif
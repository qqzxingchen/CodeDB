#include "WndProc.h"
extern HWND hTreeView;
extern HWND hStatic;
extern XC_sign_struct sign_struct;


//�г� temp ��ʾ���ļ����е������ļ����������Ǽ��뵽handle��
//handleΪTreeView��һ���ڵ�
//Ҫ�� temp �е���ʽΪ��c:\\xingchen
void ListFile(HANDLE handle,char *addr);


//��handle���г�addr��ʾ���ļ�����Ŀ¼������Ϣ
void ListFileInfo(HANDLE handle,char *fileaddr);
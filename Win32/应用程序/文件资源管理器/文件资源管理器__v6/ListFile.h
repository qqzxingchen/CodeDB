#include "WndProc.h"
extern HWND hTreeView;
extern HWND hStatic;
extern XC_sign_struct sign_struct;


//列出 temp 表示的文件夹中的所有文件，并将它们加入到handle下
//handle为TreeView的一个节点
//要求 temp 中的形式为：c:\\xingchen
void ListFile(HANDLE handle,char *addr);


//在handle上列出addr表示的文件（或目录）的信息
void ListFileInfo(HANDLE handle,char *fileaddr);
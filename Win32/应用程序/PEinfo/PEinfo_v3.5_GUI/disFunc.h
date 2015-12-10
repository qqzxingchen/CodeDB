#ifndef DISFUNC__H
#define DISFUNC__H

#include <windows.h>

#define RIGHTSTEP				20
#define XC_ONELINEHEIGHT		20

void DisOneLine(HDC hdc,PRECT pRect,char *ID,char *Value,int direct);

void DisplayBaseInfo(HDC hdc,PRECT pRect,int disId);


DWORD WINAPI StartReadThread(LPVOID point);
void DisplayImportInfo(HDC hdc,PRECT pRect,PCHILDWININFO pCWindInfo);


void DisplayExportInfo(HDC hdc,PRECT pRect,PCHILDWININFO pCWindInfo);


void DisplayDelayImportInfo(HDC hdc,PRECT pRect,PCHILDWININFO pCWindInfo);


#endif



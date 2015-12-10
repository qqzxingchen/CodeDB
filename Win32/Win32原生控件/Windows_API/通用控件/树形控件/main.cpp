#include <windows.h>
#include <stdio.h>
#include "commctrl.h"
#include "commdlg.h"
#pragma comment(lib,"comctl32.lib")

#define IDC_XC_TREEVIEW 1003

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 
HWND hTreeView;
HANDLE treehandle_root;

//FILE *fp;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance,
					 LPTSTR lpCmdLine, 
					int nCmdShow)
{
	MSG msg;
	HWND hWnd; 
	WNDCLASS wc;

	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=(WNDPROC)WndProc;       
	wc.cbClsExtra=0;                      
	wc.cbWndExtra=0;                      
	wc.hInstance=hInstance;              
	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION); 
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName=lpszAppName;              
	wc.lpszClassName=lpszAppName;
	
	//fp=fopen("out.txt","w");

	if(!RegisterClass(&wc))
		return(FALSE);
	hWnd=CreateWindow(lpszAppName,  
		lpszTitle,
		WS_OVERLAPPEDWINDOW, 
		100,
		100, 
		600, 
		600,  
		NULL, 
		NULL,  
		hInstance, 
		NULL);               
	if (!hWnd) 
		return(FALSE);
	ShowWindow(hWnd, SW_SHOW); 
	UpdateWindow(hWnd);         
	
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg); 
		DispatchMessage(&msg);  
	}

	//fclose(fp);

	return(msg.wParam); 
}


LRESULT CALLBACK WndProc(HWND hWnd, 
						 UINT uMsg, 
						 WPARAM wParam,
						 LPARAM lParam)
{
	RECT rect;
	TVINSERTSTRUCT treeinsert;
	TVITEM treeitem;


	switch(uMsg){   
	case WM_CREATE:
		InitCommonControls();
		GetClientRect(hWnd,&rect);
		hTreeView=CreateWindow("SysTreeView32","xingchen_listbox",
			TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|WS_CHILD|WS_VISIBLE,
			rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,
			hWnd,(HMENU)IDC_XC_TREEVIEW,NULL,NULL);
		treeinsert.hParent=NULL;
		treeinsert.hInsertAfter=TVI_SORT;
		treeitem.mask=TVIF_TEXT;
		treeitem.pszText="root";
		treeinsert.item=treeitem;
		treehandle_root=(HANDLE)SendMessage(hTreeView,TVM_INSERTITEM,0,(LPARAM)&treeinsert);

		int i;
		for(i=0;i<=10;i++){
			treeinsert.hParent=(HTREEITEM)treehandle_root;
			treeinsert.hInsertAfter=TVI_SORT;
			treeitem.mask=TVIF_TEXT;
			treeitem.pszText="root";
			treeinsert.item=treeitem;
			SendMessage(hTreeView,TVM_INSERTITEM,0,(LPARAM)&treeinsert);
		}

		break;
	case WM_NOTIFY:

		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}
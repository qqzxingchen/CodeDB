#include <windows.h>
#include <stdio.h>
#include "commctrl.h"
#include "commdlg.h"
#pragma comment(lib,"comctl32.lib")

#define IDC_XC_LISTBOX 1001
#define WM_XC_CLEARLISTBOX 1002

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 
HWND hListBox;
int iitem;

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
	static int uRed=0, uGreen=0, uBlue=0;
	LVCOLUMN lv;
	LVITEM   lvitem;
	int i,j,k;
	NMHDR *nmhdr;
	char temp[100];

	switch(uMsg){   
	case WM_CREATE:
		InitCommonControls();
		GetClientRect(hWnd,&rect);
		hListBox=CreateWindow("SysListView32","xingchen_listbox",
			LVS_REPORT | LVS_SINGLESEL | LVS_ALIGNLEFT | LVS_NOSORTHEADER | 
			WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE,
			rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,hWnd,
			(HMENU)IDC_XC_LISTBOX,GetModuleHandle(NULL),NULL);
		SendMessage(hWnd,WM_INITDIALOG,NULL,NULL);
		break;
	case WM_NOTIFY:
		nmhdr=(NMHDR*)lParam;
		if(nmhdr->hwndFrom==hListBox && nmhdr->code==NM_DBLCLK){
			iitem=SendMessage(hListBox,LVM_GETNEXTITEM,-1,(LPARAM)MAKELPARAM(LVNI_FOCUSED,0));
			wsprintf(temp,"line %d was doubleclicked!!!",iitem);
			MessageBox(NULL,temp,"YES",MB_OK);
		}
		break;
	case WM_INITDIALOG:
		SendMessage(hListBox,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		SendMessage(hWnd,WM_XC_CLEARLISTBOX,NULL,NULL);
		lv.mask=LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
		lv.fmt=LVCFMT_LEFT;
		lv.cx=60;
		lv.iSubItem=0;
				
		lvitem.mask=LVIF_TEXT;
		lvitem.pszText="";
		lvitem.iSubItem=0;
			
		for(i=1;i<=9;i++){
			wsprintf(temp,"line %d",i);
			lv.pszText=temp;
			SendMessage(hListBox,LVM_INSERTCOLUMN,i,(LPARAM)&lv);
			lvitem.iItem=i;				
			SendMessage(hListBox,LVM_INSERTITEM,NULL,(LPARAM)&lvitem);
		}
		for(i=1;i<=9;i++){
			for(j=1;j<=9;j++){
				if(i<j)
					continue;
				wsprintf(temp,"%d*%d=%d",i,j,i*j);
				lvitem.pszText=temp;
 				lvitem.iItem=i-1;
 				lvitem.iSubItem=j-1;
 				SendMessage(hListBox,LVM_SETITEM,NULL,(LPARAM)&lvitem);
			}
		}
		ShowWindow(hListBox,SW_SHOW);
		UpdateWindow(hListBox);
		break;
	case WM_XC_CLEARLISTBOX:
		SendMessage(hListBox,LVM_DELETEALLITEMS,NULL,NULL);
		while(1){
			i=SendMessage(hListBox,LVM_DELETECOLUMN,NULL,NULL);
			if(!i)
				break;
		}
		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}
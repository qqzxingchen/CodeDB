#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "xc.h"

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
	wc.hIcon=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1)); 
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName=lpszAppName;              
	wc.lpszClassName=lpszAppName;
	
	if(!RegisterClass(&wc))
		return(FALSE);
	hWnd=CreateWindow(lpszAppName,  
		lpszTitle,
		WS_OVERLAPPEDWINDOW, 
		100,
		100, 
		335, 
		266,  
		NULL, 
		NULL,  
		hInstance, 
		NULL);               
	if (!hWnd) 
		return(FALSE);
	ShowWindow(hWnd, nCmdShow); 
	UpdateWindow(hWnd);         
	
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg); 
		DispatchMessage(&msg);  
	}
	
	return(msg.wParam); 
}


LRESULT CALLBACK WndProc(HWND hWnd, 
						 UINT uMsg, 
						 WPARAM wParam,
						 LPARAM lParam)
{
	switch(uMsg){
	case WM_CREATE:
		CreateDialog(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOGBAR),hWnd,(DLGPROC)DialogProc);
		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default:
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}

BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam){
	char temp[1000];
	int  ErrorCode;
	int te;

	switch(uMsg){
	case WM_INITDIALOG:	
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_BUTTON_LookUp:
			ErrorCode=(int)GetDlgItemInt(hwndDlg,IDC_EDIT_ErrorCode,NULL,FALSE);
			XC_getwrongtext(ErrorCode,temp,lang);
			SetDlgItemText(hwndDlg,IDC_STATIC_DIS,temp);
			break;
		case IDC_BUTTON_LANG:
			lang=1-lang;
			break;
		}
		break;
	default:
		return FALSE;
	}
	
	return TRUE;
}

BOOL XC_getwrongtext(int ErrorCode,char * explian,int lang){
	int language;
	if(lang==0)
		language=MAKELANGID(0x04,0x02);
	else if(lang==1)
		language=MAKELANGID(0x09,0x01);
	else
		language=0;

	HLOCAL hlocal=NULL;

	BOOL fOK=FormatMessage(	FORMAT_MESSAGE_FROM_SYSTEM | 
							FORMAT_MESSAGE_IGNORE_INSERTS |		
							FORMAT_MESSAGE_ALLOCATE_BUFFER,
							NULL,ErrorCode,
							language,
							(PTSTR)&hlocal,0,NULL);
	if(!fOK){
		HMODULE hDll=LoadLibraryEx(TEXT("netmsg.dll"),NULL,DONT_RESOLVE_DLL_REFERENCES);
		if(hDll!=NULL){
			BOOL fOK=FormatMessage(	FORMAT_MESSAGE_FROM_HMODULE | 
				FORMAT_MESSAGE_IGNORE_INSERTS |		
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				hDll,ErrorCode,
				language,
				(PTSTR)&hlocal,0,NULL);
			FreeLibrary(hDll);
		}
	}
	if(fOK && (hlocal!=NULL)){
		wsprintf(explian,"%s\n",(PTSTR)LocalLock(hlocal));
		LocalFree(hlocal);
		return true;
	}else{
		wsprintf(explian,"UnKnown ErrorCode!!!\n");
		return false;
	}
}
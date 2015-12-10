#include <windows.h>
#include <stdio.h>
#include "WndProc.h"

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application");

HWND hTreeView;
HWND hStatic;
HANDLE handle_play,handle_reset,handle_stop,handle_speed,handle_rew;
HANDLE handle_avi_window;

char nowFile[1000];

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
	
	if(!RegisterClass(&wc))
		return(FALSE);
	hWnd=CreateWindow(lpszAppName,lpszTitle,
		WS_OVERLAPPEDWINDOW, 
		100,100,1100,600,
		NULL,NULL,hInstance,NULL);               
	if (!hWnd)
		return(FALSE);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);         
	
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg); 
		DispatchMessage(&msg);  
	}
	return(msg.wParam); 
}
#include <windows.h>
#include <stdio.h>
#include "resource.h"

char *str;
char *strinn_menu="test";
LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

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
	//wc.lpszMenuName=MAKEINTRESOURCE(IDM_MYMENU);              
	wc.lpszClassName=lpszAppName;
	
	str=GetCommandLine();

	if(!RegisterClass(&wc))
		return(FALSE);
	hWnd=CreateWindow(lpszAppName,  
		lpszTitle,
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT,
		0, 
		CW_USEDEFAULT, 
		0,  
		NULL,
		//NULL,
		//LoadMenu(NULL,MAKEINTRESOURCE(IDM_MYMENU)),
		LoadMenu(NULL,MAKEINTRESOURCE(IDM_MYMENU2)),
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
	RECT rect;
	HDC hdc;
	PAINTSTRUCT ps;
	static int uRed=0, uGreen=0, uBlue=0;
	HMENU hmenu;
	POINT point;
	
	switch(uMsg){   
	case WM_CREATE:
		 hmenu=LoadMenu(NULL,MAKEINTRESOURCE(IDM_MYMENU2));
		 hmenu = GetSubMenu (hmenu, 0) ;
		 break;
	case WM_PAINT :
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
		//DrawText (hdc, TEXT ("Hello, You Are Welcoming!"), -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;	
		
		DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;	
		EndPaint(hWnd, &ps);
		break;

	case WM_RBUTTONDOWN:
	//case WM_CONTEXTMENU:
		point.x=LOWORD(lParam);
		point.y=HIWORD(lParam);
		ClientToScreen(hWnd,&point);
		TrackPopupMenu(hmenu,TPM_LEFTALIGN,point.x,point.y,0,hWnd,NULL);

		break;
	case WM_KEYDOWN :
	case WM_LBUTTONDOWN:
		uRed=rand() % 255;
		uGreen=rand() % 255;
		uBlue=rand() % 255;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}

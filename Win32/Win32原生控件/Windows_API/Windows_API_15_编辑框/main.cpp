#include <windows.h>
#include "resource.h"

LPCTSTR str;
LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 
HWND temp[4];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, 
					HINSTANCE hPrevInstance,
					LPTSTR lpCmdLine, 
					int nCmdShow)
{   
	MSG msg;
	HWND hWnd; 
	WNDCLASS wc;

	str=GetCommandLine();

	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=(WNDPROC)WndProc;       
	wc.cbClsExtra=0;                      
	wc.cbWndExtra=0;                      
	wc.hInstance=hInstance;              
	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION); 
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=CreateSolidBrush(RGB(112,12,12));
	wc.lpszMenuName=lpszAppName;              
	wc.lpszClassName=lpszAppName;
	
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
	RECT rect;
	HDC hdc;
	PAINTSTRUCT ps;
	static int uRed=0, uGreen=0, uBlue=0;

	switch(uMsg){   
		case WM_CREATE:
			temp[0]=CreateWindow(TEXT("EDIT"),NULL,WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL |ES_NUMBER,
				0,0,100,100,hWnd,(HMENU)MY_EDIT,GetModuleHandle(NULL),NULL);
			temp[1]=CreateWindow(TEXT("EDIT"),NULL,WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_UPPERCASE,
				100,0,100,100,hWnd,(HMENU)MY_EDIT_2,GetModuleHandle(NULL),NULL);
			temp[2]=CreateWindow(TEXT("EDIT"),NULL,WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
				200,0,100,100,hWnd,(HMENU)MY_EDIT,GetModuleHandle(NULL),NULL);		
			temp[3]=CreateWindow(TEXT("EDIT"),NULL,WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
				300,0,100,100,hWnd,(HMENU)MY_EDIT,GetModuleHandle(NULL),NULL);		

			break;
		case WM_PAINT :
			hdc=BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
			DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;		
			EndPaint(hWnd, &ps);
			
			int i;
			for(i=0;i<4;i++){
				hdc=BeginPaint(temp[i],&ps);
				GetClientRect(temp[i],&rect);
				SetTextColor(hdc,RGB(uRed,uGreen,uBlue));
				DrawText(hdc,"hello\0",-1,&rect,DT_CENTER);
				EndPaint(temp[i],&ps);
			}
			break;

		case WM_RBUTTONDOWN:
			PostQuitMessage(0);
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

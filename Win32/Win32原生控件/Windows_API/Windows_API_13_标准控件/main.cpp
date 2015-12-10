#include <windows.h>
#include "resource.h"

LPCTSTR str;
HWND scroll;
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

	str=GetCommandLine();

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
			CreateWindow("STATIC", TEXT("欢迎光临!"), WS_VISIBLE | WS_CHILD 				
				| SS_CENTER, 0, 0, 90, 20, hWnd, NULL, GetModuleHandle(NULL), NULL);

			CreateWindow("BUTTON", "请确认(&C)", WS_VISIBLE | 				
				WS_CHILD | BS_DEFPUSHBUTTON, 90, 0, 110, 30, hWnd,
				(HMENU)MAKEINTRESOURCE(IDC_BUTTON1), GetModuleHandle(NULL), NULL); 

			CreateWindow("BUTTON", "请确认(&C)", WS_VISIBLE | 				
				WS_CHILD | BS_DEFPUSHBUTTON, 200, 0, 110, 90, hWnd,
				(HMENU)IDC_BUTTON_XC, GetModuleHandle(NULL), NULL); 

			scroll=CreateWindow ("SCROLLBAR",(LPSTR)NULL,WS_CHILD | SBS_VERT | WS_VISIBLE, 310, 0, 20, 200,
				hWnd, (HMENU)IDC_MYSCROLLBAR,GetModuleHandle(NULL), NULL);
			//SendMessage(scroll,SBM_SETPOS,100,TRUE);
			char temp[100];
			int m,n;

			GetScrollRange(scroll,SB_CTL,&m,&n);
			wsprintf(temp,"now is %d ,%d %d",GetScrollPos(scroll,SB_CTL),m,n);
			MessageBox(scroll,temp,temp,MB_OK);

			break;
		case WM_PAINT :
			hdc=BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
			DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;		
			EndPaint(hWnd, &ps);
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

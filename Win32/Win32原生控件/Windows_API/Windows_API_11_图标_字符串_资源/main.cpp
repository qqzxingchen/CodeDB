#include <windows.h>
#include "resource.h"

HWND desktop_hwnd;
HINSTANCE hin;
LPCTSTR str;
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

	hin=hInstance;
	str=GetCommandLine();
	desktop_hwnd=GetDesktopWindow();

	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=(WNDPROC)WndProc;       
	wc.cbClsExtra=0;                      
	wc.cbWndExtra=0;                      
	wc.hInstance=hInstance;      
	
	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION); 
	//wc.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	//wc.hIcon=(HICON)LoadImage(hInstance,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,128,128,LR_DEFAULTCOLOR);

	//wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	//wc.hCursor=LoadCursor(hInstance,MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hCursor=(HCURSOR)LoadImage(hInstance,MAKEINTRESOURCE(IDC_CURSOR1),IMAGE_CURSOR,16,16,LR_DEFAULTCOLOR);
	
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
	char temp[10];

	switch(uMsg){   
		case WM_PAINT :
			hdc=BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
			//DrawText (hdc, TEXT ("Hello, You Are Welcoming!"), -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;	
			DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;	

			LoadString(hin,IDS_STRING1,temp,sizeof(temp));
			DrawText(hdc,temp,-1,&rect,NULL);

			EndPaint(hWnd, &ps);
			break;
		case WM_RBUTTONDOWN:
			SendMessage(hWnd,WM_DESTROY,wParam,lParam);
			break;
		case WM_KEYDOWN :
			SetClassLong(hWnd,GCL_HICON,(LONG)LoadIcon(hin,MAKEINTRESOURCE(IDI_ICON1)));
			break;
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

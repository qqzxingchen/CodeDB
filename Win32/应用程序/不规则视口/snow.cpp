#include <windows.h>
#include "cursor.h"

#define XC_TIMER 10033

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRGN createRgn(int x,int y,int length);

BOOL buttonDown=FALSE;
extern BYTE XORmaskCursor[];
extern BYTE ANDmaskCursor[];

int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance,
					 LPTSTR lpCmdLine, 
					 int nCmdShow)
{   
	MSG msg;
	HWND hWnd; 
	WNDCLASS wc;
	
	HCURSOR hCurs; 

	hCurs = CreateCursor(
		hInstance,
		19,2,32,32,
		ANDmaskCursor,
		XORmaskCursor
	);

	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=(WNDPROC)WndProc;       
	wc.cbClsExtra=0;                      
	wc.cbWndExtra=0;                      
	wc.hInstance=hInstance;              
	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION); 
	//wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hCursor=hCurs;
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+2);
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
	HDC hdc=GetDC(hWnd);
	char str[100]="\0";
	RECT rect;
	POINT point;
	static int i=0,j=0;

	switch(uMsg){   
	case WM_CREATE:
		SetTimer(hWnd,XC_TIMER,50,NULL);
		SetWindowRgn(hWnd,createRgn(0,0,60),TRUE);
		break;
// 	case WM_TIMER:
// 		SetWindowRgn(hWnd,createRgn(100+i,100+j,60),TRUE);
// 		i+=5;
// 		j+=5;
// 		break;
	case WM_LBUTTONDOWN:
		buttonDown=TRUE;
		
		point.x=(int)LOWORD(lParam);
		point.y=(int)HIWORD(lParam);

		ClientToScreen(hWnd,&point);
		GetWindowRect(hWnd,&rect);
		point.x-=rect.left;
		point.y-=rect.top;
		SetWindowRgn(hWnd,createRgn(point.x,point.y,60),TRUE);
		
		break;
	case WM_LBUTTONUP:
		buttonDown=FALSE;
		break;
	case WM_MOUSEMOVE:
		if(buttonDown){
			point.x=(int)LOWORD(lParam);
			point.y=(int)HIWORD(lParam);	

			ClientToScreen(hWnd,&point);
			GetWindowRect(hWnd,&rect);
			point.x-=rect.left;
			point.y-=rect.top;
			SetWindowRgn(hWnd,createRgn(point.x,point.y,60),TRUE);
		}
		break;
	case WM_RBUTTONDOWN:
		PostQuitMessage(0);
		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}


HRGN createRgn(int x,int y,int length){
	POINT s[10];
	HRGN rgn;
	s[0].x=x-length/2;	s[0].y=y-length;
	s[1].x=x+length/2;	s[1].y=y-length;
	s[2].x=x+length;	s[2].y=y;
	s[3].x=x+length/2;	s[3].y=y+length;
	s[4].x=x-length/2;	s[4].y=y+length;
	s[5].x=x-length;	s[5].y=y;
	rgn=CreatePolygonRgn(s,6,WINDING);
	return rgn;
}
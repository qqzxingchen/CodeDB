#include <windows.h>
#include <string.h>
#include <stdio.h>

HWND desktop_hwnd;
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
	
	str=GetCommandLine();
	desktop_hwnd=GetDesktopWindow();
	
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
	HFONT oldfont,hfont1,hfont2,hfont3;
	LOGFONT LogFont;


	switch(uMsg){   
	case WM_PAINT :
		hdc=BeginPaint(hWnd, &ps) ;     
		
		memset(&LogFont, 0, sizeof(LOGFONT));
		LogFont.lfHeight=45;
		LogFont.lfWidth=30;		
		LogFont.lfUnderline=0;		
		LogFont.lfItalic=0;    		
		LogFont.lfCharSet=GB2312_CHARSET;		
		lstrcpy(LogFont.lfFaceName,"隶书");		
		hfont1=CreateFontIndirect(&LogFont);
				
		// 创建逻辑字体    		
		hfont2=CreateFont(			
			25, 15, 90, 900, 700, 0, 0, 1,			
			GB2312_CHARSET, 			
			OUT_DEFAULT_PRECIS,	
			CLIP_DEFAULT_PRECIS,		
			DEFAULT_QUALITY,		
			FF_DONTCARE,		
			"华文行楷"		
		);	

		memset(&LogFont, 0, sizeof(LOGFONT));	
		LogFont.lfHeight=25;	
		LogFont.lfItalic=1;	
		LogFont.lfEscapement=150 ;           	
		LogFont.lfCharSet=ANSI_CHARSET;	
		lstrcpy(LogFont.lfFaceName, "Italic");	
		hfont3=CreateFontIndirect(&LogFont); 

		SelectObject(hdc, hfont1);	
		TextOut(hdc, 20, 50, "继续努力。", 10);
		SelectObject(hdc,hfont2);	
		TextOut(hdc, 20, 150, "通向成功的阶梯。", 16);	
		SelectObject(hdc,hfont3);	
		TextOut(hdc, 20, 300, "Have a deep breath, Look!", 25);      	
		SelectObject(hdc, oldfont);		
		
		DeleteObject(hfont1);		
		DeleteObject(hfont2);		
		DeleteObject(hfont3);	
		EndPaint(hWnd, &ps) ;
		

// 		hdc=BeginPaint(hWnd, &ps);
// 		GetClientRect(hWnd, &rect);
// 		SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
// 		//DrawText (hdc, TEXT ("Hello, You Are Welcoming!"), -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;	
// 		
// 		//DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;	
// 		TextOut(hdc,100,100,str,lstrlen(str));
// 		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN :
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		uRed=rand() % 255;
		uGreen=rand() % 255;
		uBlue=rand() % 255;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	case MOUSE_MOVED:
		char string[20];
		sprintf(string,"%d.%d",HIWORD(lParam),LOWORD(lParam));
		TextOut(hdc,10,10,string,lstrlen(string));
		TextOut(hdc,20,20,TEXT("right"),6);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}

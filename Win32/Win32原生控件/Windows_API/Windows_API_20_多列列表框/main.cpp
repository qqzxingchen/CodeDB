#include <windows.h>
#include "commctrl.h"
#include "commdlg.h"
#include "resource.h"
#pragma comment(lib,"comctl32.lib")

#define XC_WM_INITDLG 1111

LPCTSTR str;
LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

HWND hWndListBox;
LVCOLUMN lv;

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

BOOL CALLBACK resu(
					HWND hwndDlg,  // handle to dialog box
					UINT uMsg,     // message
					WPARAM wParam, // first message parameter
					LPARAM lParam  // second message parameter
					)
{
	HWND han;
	switch (uMsg){
	case WM_INITDIALOG:
		han=GetDlgItem(hwndDlg,IDC_LIST1);

		SendMessage(han,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		ShowWindow(hwndDlg,SW_SHOW);
		UpdateWindow(hwndDlg);
		break;
	case WM_NOTIFY:
		//MessageBox(NULL,"notify",NULL,MB_OK);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case IDC_BUTTON1:
			EndDialog(hwndDlg,NULL);

		break;
		}

		break;
	default:
		return FALSE;
	}
	return TRUE;
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
		case XC_WM_INITDLG:
			InitCommonControls();
			SetClassLong(hWnd, GCL_HBRBACKGROUND, (long)CreateSolidBrush((COLORREF) GetSysColor(COLOR_BTNFACE)));
			DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOGBAR),hWnd,(DLGPROC)resu,0);
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
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
		case WM_LBUTTONDOWN:
			SendMessage(hWnd,XC_WM_INITDLG,NULL,NULL);
			break;
		case WM_KEYDOWN :
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

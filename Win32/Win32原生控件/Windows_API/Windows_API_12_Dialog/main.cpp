#include <windows.h>
#include "resource.h"

HWND hIdeaDlg;
int ret;
HWND ret2;
LPCTSTR str;
LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(	HWND hwndDlg,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam)
{
	switch(uMsg){
	case WM_INITDIALOG:	
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_BUTTON1:
			char temp[100];
			GetDlgItemText(hwndDlg,IDC_EDIT1,temp,sizeof(temp));
			MessageBox(hwndDlg,temp,temp,MB_OK);
			EndDialog(hwndDlg,IDC_BUTTON1);
			break;
		case IDC_RADIO1:
			EndDialog(hwndDlg,IDC_RADIO1);
			break;
		}
		break;
	default:
		return FALSE;
	}
	
	return TRUE;
}



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
	
// 	while(GetMessage(&msg, NULL, 0, 0)){
// 		TranslateMessage(&msg); 
// 		DispatchMessage(&msg);  
// 	}

	while (GetMessage (&msg, NULL, 0, 0)){
        if(!IsWindow(ret2) || !IsDialogMessage(ret2, &msg))	{	
            TranslateMessage (&msg) ;		
			DispatchMessage (&msg) ;		
        }	
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
	case WM_PAINT :
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
		DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;	
		EndPaint(hWnd, &ps);
		break;
	case WM_RBUTTONDOWN:
		SendMessage(hWnd,WM_DESTROY,wParam,lParam);
		break;
	case WM_KEYDOWN :

		CreateDialog(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOGBAR),hWnd,(DLGPROC)DialogProc);
		//ret=DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOGBAR),hWnd,(DLGPROC)DialogProc);
		
		if(ret == IDOK){
			MessageBox(hWnd, "Dialog exited with IDOK.", "Notice",
				MB_OK | MB_ICONINFORMATION);
		}
		else if(ret == IDCANCEL){
			MessageBox(hWnd, "Dialog exited with IDCANCEL.", "Notice",
				MB_OK | MB_ICONINFORMATION);
		}
		else if(ret == -1){
			ret=GetLastError();
			char temp123[100];
			wsprintf(temp123,"%u",ret);
			MessageBox(hWnd, "Dialog failed!",temp123 ,
				MB_OK | MB_ICONINFORMATION);
            }
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

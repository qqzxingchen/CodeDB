#include <windows.h>

#define IDC_MYLIST 2001
#define IDC_MYCOMBOX 2002

LPCTSTR str;
LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

HWND hWndListBox;
HWND hwndComboBox;

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
	int i;
	char temp[20];

	switch(uMsg){   
		case WM_CREATE:
			SetClassLong(hWnd, GCL_HBRBACKGROUND, (long)CreateSolidBrush((COLORREF) GetSysColor(COLOR_BTNFACE)));

			hWndListBox=CreateWindow (TEXT("LISTBOX"),NULL,WS_CHILD|WS_VISIBLE|LBS_NOTIFY|WS_BORDER|ES_AUTOHSCROLL|WS_VSCROLL,
				0, 0, 200, 200, hWnd, (HMENU)IDC_MYLIST,GetModuleHandle(NULL),NULL);
			hwndComboBox=CreateWindow(TEXT("COMBOBOX"),NULL, WS_CHILD| WS_VISIBLE |WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
				200,0,200,200,hWnd,  (HMENU)IDC_MYCOMBOX,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
			break;
		case WM_SIZE:
			MoveWindow(hWndListBox,0,0,200,200,TRUE);
			MoveWindow(hwndComboBox,200,0,200,200,TRUE);

			//清除列表框中的内容
			SendMessage(hWndListBox, LB_RESETCONTENT, 0, 0 );

			//为列表框增加项
			for(i=0;i<20;i++){
				wsprintf(temp,"string %d\0",i);
				SendMessage(hWndListBox,LB_ADDSTRING,0,(LPARAM)temp);
			}

			// 在组合框中添加项
			SendMessage(hwndComboBox, CB_RESETCONTENT, 0, 0 );		
			SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("四川"));		
			SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("广东"));		
			SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("河南"));		
			
			//设置组合框和列表框中默认选中的项	
			SendMessage(hwndComboBox, CB_SETCURSEL,2,0);      	
			SendMessage(hWndListBox, LB_SETCURSEL,3,0);
			
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDC_MYCOMBOX:
					switch(HIWORD(wParam)){
					case CBN_SELCHANGE:
						int t=SendMessage(hwndComboBox,CB_GETCURSEL,0,0);
						SendMessage(hWndListBox,LB_RESETCONTENT,0,0);

						for(t=t+5;t>=0;t--){
							wsprintf(temp,"now string %d\0",t);
							SendMessage(hWndListBox,LB_ADDSTRING,0,(LPARAM)temp);
						}	
					}
				break;
				
				case IDC_MYLIST:
					switch(HIWORD(wParam)){
						case LBN_DBLCLK:
							int t=SendMessage(hWndListBox, LB_GETCURSEL, 0, 0);
							SendMessage(hWndListBox, LB_GETTEXT, t, (LPARAM)temp);
							//wsprintf(temp,"now is %s",temp);
							MessageBox(NULL,temp,temp,MB_OK);
						break;
					}
				break;
			}

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

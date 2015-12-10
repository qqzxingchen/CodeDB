#include <windows.h>

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void disFormat(HWND hWnd);

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
	hWnd=CreateWindow(lpszAppName,  
		lpszTitle,
		//WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX, 
		WS_MINIMIZEBOX|WS_SYSMENU|WS_THICKFRAME,
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
	static HWND hWndNextViewer;
	HGLOBAL     hGMem;	//全局内存句柄
	HDC         hdc;	//设备描述表句柄
	PSTR        pGMem;	//全局内存指针
	PAINTSTRUCT ps;
	RECT        rect;
	HDROP hdrop;

	char temp[1000];

	switch(uMsg){  
	case WM_CREATE:
		MoveWindow(hWnd,100,100,300,300,TRUE);
		hWndNextViewer=SetClipboardViewer(hWnd);
		break;
	case WM_CHANGECBCHAIN:
		if((HWND)wParam==hWndNextViewer){
			hWndNextViewer=(HWND)lParam;
		}else if(hWndNextViewer){
			SendMessage(hWndNextViewer,uMsg,wParam,lParam);
		}
		break;
	case WM_DRAWCLIPBOARD://更新对剪贴板数据的显示
		if (hWndNextViewer)
			SendMessage (hWndNextViewer, uMsg, wParam, lParam);
		//disFormat(hWnd);
		InvalidateRect (hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint (hWnd, &ps);
		GetClientRect (hWnd, &rect);
		OpenClipboard (hWnd);
		
		hGMem = GetClipboardData (CF_TEXT);//获取剪贴板数据
		if (hGMem != NULL){
			pGMem = (PSTR) GlobalLock (hGMem);
			DrawText (hdc, pGMem, -1, &rect, DT_EXPANDTABS);
			GlobalUnlock (hGMem);
		}
		
		hdrop=(HDROP)GetClipboardData(CF_HDROP);
		if(hdrop!=NULL){
			DragQueryFile(hdrop,0,temp,sizeof(temp));
			MessageBox(NULL,temp,NULL,MB_OK);
		}
		
		CloseClipboard ();
		EndPaint (hWnd, &ps);
		break;
	case WM_RBUTTONDOWN:
	case WM_DESTROY :
		ChangeClipboardChain (hWnd, hWndNextViewer);
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}

void disFormat(HWND hWnd){
	char strFormat[1000]="\0";

	OpenClipboard(hWnd);
	if(IsClipboardFormatAvailable(CF_BITMAP)){
		wsprintf(&strFormat[strlen(strFormat)],"BITMAP");
	}else if(IsClipboardFormatAvailable(CF_DIB)){
		wsprintf(&strFormat[strlen(strFormat)],"DIB");
	}else if(IsClipboardFormatAvailable(CF_DIF)){
		wsprintf(&strFormat[strlen(strFormat)],"DIF");
	}else if(IsClipboardFormatAvailable(CF_DSPBITMAP)){
		wsprintf(&strFormat[strlen(strFormat)],"DSPBITMAP");
	}else if(IsClipboardFormatAvailable(CF_DSPENHMETAFILE)){
		wsprintf(&strFormat[strlen(strFormat)],"DSPENHMETAFILE");
	}else if(IsClipboardFormatAvailable(CF_DSPMETAFILEPICT)){
		wsprintf(&strFormat[strlen(strFormat)],"DSPMETAFILEPICT");
	}else if(IsClipboardFormatAvailable(CF_DSPTEXT)){
		wsprintf(&strFormat[strlen(strFormat)],"DSPTEXT");
	}else if(IsClipboardFormatAvailable(CF_ENHMETAFILE)){
		wsprintf(&strFormat[strlen(strFormat)],"ENHMETAFILE");
	}else if(IsClipboardFormatAvailable(CF_HDROP)){
		wsprintf(&strFormat[strlen(strFormat)],"HDROP");
	}else if(IsClipboardFormatAvailable(CF_LOCALE)){
		wsprintf(&strFormat[strlen(strFormat)],"LOCALE");
	}else if(IsClipboardFormatAvailable(CF_METAFILEPICT)){
		wsprintf(&strFormat[strlen(strFormat)],"METAFILEPICT");
	}else if(IsClipboardFormatAvailable(CF_OEMTEXT)){
		wsprintf(&strFormat[strlen(strFormat)],"OEMTEXT");
	}else if(IsClipboardFormatAvailable(CF_OWNERDISPLAY)){
		wsprintf(&strFormat[strlen(strFormat)],"OWNERDISPLAY");
	}else if(IsClipboardFormatAvailable(CF_PALETTE)){
		wsprintf(&strFormat[strlen(strFormat)],"PALETTE");
	}else if(IsClipboardFormatAvailable(CF_PENDATA)){
		wsprintf(&strFormat[strlen(strFormat)],"PENDATA");
	}else if(IsClipboardFormatAvailable(CF_RIFF)){
		wsprintf(&strFormat[strlen(strFormat)],"RIFF");
	}else if(IsClipboardFormatAvailable(CF_SYLK)){
		wsprintf(&strFormat[strlen(strFormat)],"SYLK");
	}else if(IsClipboardFormatAvailable(CF_TEXT)){
		wsprintf(&strFormat[strlen(strFormat)],"TEXT");
	}else if(IsClipboardFormatAvailable(CF_WAVE)){
		wsprintf(&strFormat[strlen(strFormat)],"WAVE");
	}else if(IsClipboardFormatAvailable(CF_TIFF)){
		wsprintf(&strFormat[strlen(strFormat)],"TIFF");
	}else if(IsClipboardFormatAvailable(CF_UNICODETEXT)){
		wsprintf(&strFormat[strlen(strFormat)],"UNICODETEXT");
	}else{
		wsprintf(&strFormat[strlen(strFormat)],"maybe it is another format that I do not know");
	}

	CloseClipboard();

	MessageBox(NULL,strFormat,NULL,MB_OK);
}
#include <windows.h>
#include "resource.h"

#define INIT_MENU_POS 	0
#define HELLO_MENU_POS	2
#define RECT_MENU_POS	1

#define IDM_FIRSTCHILD	50000

LRESULT CALLBACK FrameWndProc	(HWND,UINT,WPARAM,LPARAM);
BOOL	CALLBACK CloseEnumProc	(HWND,LPARAM);
LRESULT	CALLBACK HelloWndProc	(HWND,UINT,WPARAM,LPARAM);
LRESULT	CALLBACK RectWndProc	(HWND,UINT,WPARAM,LPARAM);

typedef struct tagHELLODATA{
	UINT		iColor;
	COLORREF	clrText;
}HELLODATA,*PHELLODATA;
typedef struct tagRECTDATA{
	short		cxClient;
	short		cyClient;
}RECTDATA,*PRECTDATA;

TCHAR	szAppName[]		=TEXT("MDIDemo");
TCHAR	szFrameClass[]	=TEXT("MdiFrame");
TCHAR	szHelloClass[]	=TEXT("MdiHelloChild");
TCHAR	szRectClass[]	=TEXT("MdiRectChild");

HINSTANCE hInst;
HMENU	hMenuInit,hMenuHello,hMenuRect;
HMENU	hMenuInitWindow,hMenuHelloWindow,hMenuRectWindow;

int WINAPI WinMain(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int iCmdShow)
{
	HWND		hwndFrame;
	MSG			msg;
	WNDCLASS	wc;

	hInst=hInstance;

	//register frame class
	wc.style		=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc	=FrameWndProc;
	wc.cbClsExtra	=0;
	wc.cbWndExtra	=0;
	wc.hInstance	=hInstance;
	wc.hIcon		=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_APPWORKSPACE+1);
	wc.lpszMenuName	=NULL;
	wc.lpszClassName=szFrameClass;
	if(!RegisterClass(&wc))
		return 0;

	//register hello child window
	wc.style		=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc	=HelloWndProc;
	wc.cbClsExtra	=0;
	wc.cbWndExtra	=sizeof(HANDLE);
	wc.hInstance	=hInstance;
	wc.hIcon		=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	=NULL;
	wc.lpszClassName=szHelloClass;
	RegisterClass(&wc);

	//register rect child window
	wc.style		=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc	=RectWndProc;
	wc.cbClsExtra	=0;

	wc.cbWndExtra	=sizeof(HANDLE);
	wc.hInstance	=hInstance;
	wc.hIcon		=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	=NULL;
	wc.lpszClassName=szRectClass;
	RegisterClass(&wc);

	//menu
	hMenuInit	=LoadMenu(hInstance,TEXT("MdiMenuInit"));
	hMenuHello	=LoadMenu(hInstance,TEXT("MdiMenuHello"));	
	hMenuRect	=LoadMenu(hInstance,TEXT("MdiMenuRect"));
	
	hMenuInitWindow	=GetSubMenu(hMenuInit,INIT_MENU_POS);
	hMenuHelloWindow=GetSubMenu(hMenuHello,HELLO_MENU_POS);
	hMenuRectWindow	=GetSubMenu(hMenuRect,RECT_MENU_POS);

	hwndFrame=CreateWindow(
		szFrameClass,
		TEXT("MDI Demonstration"),
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,
		CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,
		hMenuInit,hInstance,NULL);

	ShowWindow(hwndFrame,iCmdShow);
	UpdateWindow(hwndFrame);

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyMenu(hMenuHello);
	DestroyMenu(hMenuRect);
	return msg.wParam;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HWND			hwndClient;
	CLIENTCREATESTRUCT	clientcreate;
	HWND				hwndChild;
	MDICREATESTRUCT		mdicreate;

	switch (message)
	{
	case WM_CREATE:
		clientcreate.hWindowMenu=hMenuInitWindow;
		clientcreate.idFirstChild=IDM_FIRSTCHILD;

		hwndClient=CreateWindow(
			TEXT("MDICLIENT"),
			NULL,
			WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,
			0,0,0,0,
			hwnd,
			(HMENU)1,
			hInst,
			(PSTR)&clientcreate);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_NEWHELLO:
			mdicreate.szClass	=szHelloClass;
			mdicreate.szTitle	=TEXT("Hello");
			mdicreate.hOwner	=hInst;
			mdicreate.x			=CW_USEDEFAULT;
			mdicreate.y			=CW_USEDEFAULT;
			mdicreate.cx		=CW_USEDEFAULT;
			mdicreate.cx		=CW_USEDEFAULT;
			mdicreate.style		=0;
			mdicreate.lParam	=0;
			hwndChild=(HWND)SendMessage(hwndClient,
				WM_MDICREATE,0,(LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			return 0;
		case IDM_FILE_NEWRECT:
			mdicreate.szClass	=szRectClass;
			mdicreate.szTitle	=TEXT("Rectangles");
			mdicreate.hOwner	=hInst;
			mdicreate.x			=CW_USEDEFAULT;
			mdicreate.y			=CW_USEDEFAULT;
			mdicreate.cx		=CW_USEDEFAULT;
			mdicreate.cx		=CW_USEDEFAULT;
			mdicreate.style		=0;
			mdicreate.lParam	=0;
			hwndChild=(HWND)SendMessage(hwndClient,
				WM_MDICREATE,0,(LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			return 0;
		case IDM_FILE_CLOSE:
			hwndChild=(HWND)SendMessage(hwndClient,WM_MDIGETACTIVE,0,0);
			if(SendMessage(hwndChild,WM_QUERYENDSESSION,0,0))
				SendMessage(hwndClient,WM_MDIDESTROY,(WPARAM)hwndChild,0);
			return 0;
		case IDM_APP_EXIT:
			SendMessage(hwnd,WM_CLOSE,0,0);
			return 0;
		case IDM_WINDOW_TILE:
			SendMessage(hwndClient,WM_MDITILE,0,0);
			return 0;
		case IDM_WINDOW_ARRANGE:
			SendMessage(hwndClient,WM_MDIICONARRANGE,0,0);	//排列最小化的窗口，但是不会对没有初始化的窗口有影响
			return 0;
		case IDM_WINDOW_CASCADE:
			SendMessage(hwndClient,WM_MDICASCADE,0,0);		
			return 0;
		case IDM_WINDOW_CLOSEALL:
			EnumChildWindows(hwndClient,CloseEnumProc,0);	//枚举hwndclient函数的子窗口并依次调用CloseEnumProc
			return 0;
		default:
			hwndChild=(HWND)SendMessage(hwndClient,WM_MDIGETACTIVE,0,0);
			if(IsWindow(hwndChild))
				SendMessage(hwndChild,WM_COMMAND,wParam,lParam);
			break;
		}
		break;
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		SendMessage(hwnd,WM_COMMAND,IDM_WINDOW_CLOSEALL,0);
		if(NULL!=GetWindow(hwndClient,GW_CHILD))
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefFrameProc(hwnd,hwndClient,message,wParam,lParam);
}


//每个子窗口将要被销毁时，都会调用这个函数
//To continue enumeration, the callback function must return TRUE; to stop enumeration, it must return FALSE. 
BOOL	CALLBACK CloseEnumProc	(HWND hwnd,LPARAM lParam)
{
	if(GetWindow(hwnd,GW_OWNER))
		return TRUE;
	SendMessage(GetParent(hwnd),WM_MDIRESTORE,(WPARAM)hwnd,0);
 	if(!SendMessage(hwnd,WM_QUERYENDSESSION,0,0))
 		return TRUE;
	SendMessage(GetParent(hwnd),WM_MDIDESTROY,(WPARAM)hwnd,0);
	return TRUE;
}



LRESULT	CALLBACK HelloWndProc	(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static COLORREF clrTextArray[]=
			{RGB(0,0,0),
			RGB(255,0,0),
			RGB(0,255,0),
			RGB(0,0,255),
			RGB(255,255,255)};
	static HWND hwndClient,hwndFrame;
	HDC		hdc;
	HMENU	hMenu;
	PHELLODATA	pHelloData;
	PAINTSTRUCT	ps;
	RECT	rect;

	switch (message)
	{
	case WM_CREATE:	//每次创建该类型的子窗口，该函数都将被调用一次
		pHelloData=(PHELLODATA)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY,sizeof(HELLODATA));
		pHelloData->iColor=IDM_COLOR_BLACK;
		pHelloData->clrText=RGB(0,0,0);

		//要使用SetWindowLong函数，必须在类注册的时候指明要求的额外空间大小，即wc.cbWndExtra（以字节为单位）
		SetWindowLong(hwnd,GWL_USERDATA,(long)pHelloData);
		hwndClient=GetParent(hwnd);
		hwndFrame=GetParent(hwndClient);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_COLOR_BLACK:
		case IDM_COLOR_RED:
		case IDM_COLOR_GREEN:
		case IDM_COLOR_BLUE:
		case IDM_COLOR_WHITE:
			pHelloData=(PHELLODATA)GetWindowLong(hwnd,GWL_USERDATA);
			hMenu=GetMenu(hwndFrame);
			
			CheckMenuItem(hMenu,pHelloData->iColor,MF_UNCHECKED);	//在Frame窗口上将颜色选项前面的对勾去掉
			
			pHelloData->iColor=wParam;								//获取当前点击的颜色
			CheckMenuItem(hMenu,pHelloData->iColor,MF_CHECKED);		//在Frame窗口上将点击的颜色选项前面打上对勾
			
			pHelloData->clrText=clrTextArray[wParam-IDM_COLOR_BLACK];	//更新当前颜色为被选中的颜色
			InvalidateRect(hwnd,NULL,FALSE);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps);
		pHelloData=(PHELLODATA)GetWindowLong(hwnd,GWL_USERDATA);
		SetTextColor(hdc,pHelloData->clrText);
		GetClientRect(hwnd,&rect);
		DrawText(hdc,TEXT("hello world!"),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		EndPaint(hwnd,&ps);
		return 0;
	
		//WM_MDIACTIVATE是发送给MDI客户窗口的消息,可以强制激活其中一个MDI子窗口
		//消息的参数wParam为将要激活的MDI子窗口的窗口句柄；
		//参数lParam在发送消息时置为零.
		//MDI子窗口收到消息后，该参数是正在取消激活的MDI子窗口句柄。返回值设置为零。
	case WM_MDIACTIVATE:
		SendMessage(hwndClient,WM_MDISETMENU,
			(lParam==(LPARAM)hwnd)?(WPARAM)hMenuHello:(WPARAM)hMenuInit,
			NULL);
			//(LPARAM)hMenuInitWindow);

		//当在相同类型但不同的子窗口间切换时，需要将Frame窗口的颜色选项更新为当前子窗口的颜色
		pHelloData=(PHELLODATA)GetWindowLong(hwnd,GWL_USERDATA);
		CheckMenuItem(hMenuHello,pHelloData->iColor,
			(lParam==(LPARAM)hwnd)?MF_CHECKED:MF_UNCHECKED);

		DrawMenuBar(hwndFrame);		//重绘Frame窗口的菜单
									//即这个菜单是为某一类型窗口服务的
									//这个菜单是当当前焦点在不同类型的子窗口上切换时发生改变，
									//在相同类型子窗口上切换时不发生改变
		return 0;

//	case WM_QUERYENDSESSION:		//
//	case WM_CLOSE:					//退出前的提醒对话框
//		if(IDOK!=MessageBox(hwnd,TEXT("OK to close window?"),TEXT("hello"),MB_ICONQUESTION|MB_OKCANCEL))
//			return 0;
//		else
//			break;

	case WM_DESTROY:
		pHelloData=(PHELLODATA)GetWindowLong(hwnd,0);
		HeapFree(GetProcessHeap(),0,pHelloData);
		return 0;
	}
	return DefMDIChildProc(hwnd,message,wParam,lParam);
}



LRESULT	CALLBACK RectWndProc	(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HWND hwndClient,hwndFrame;
	HDC			hdc;
	PRECTDATA	pRectData;
	PAINTSTRUCT	ps;
	
	switch (message)
	{
	case WM_CREATE:
		pRectData=(PRECTDATA)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY,sizeof(RECTDATA));
		SetWindowLong(hwnd,GWL_USERDATA,(long)pRectData);
		hwndClient=GetParent(hwnd);
		hwndFrame=GetParent(hwndClient);
		return 0;
	case WM_SIZE:
		if(wParam!=SIZE_MINIMIZED)
		{
			pRectData=(PRECTDATA)GetWindowLong(hwnd,GWL_USERDATA);
			pRectData->cxClient=LOWORD(lParam);
			pRectData->cyClient=HIWORD(lParam);
		}
		break;
	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps);
		EndPaint(hwnd,&ps);
		return 0;
	case WM_MDIACTIVATE:
		SendMessage(hwndClient,WM_MDISETMENU,
			(lParam==(LPARAM)hwnd)?(WPARAM)hMenuRect:(WPARAM)hMenuInit,(LPARAM)hMenuRectWindow);	
		DrawMenuBar(hwndFrame);
		return 0;
	case WM_DESTROY:
		pRectData=(PRECTDATA)GetWindowLong(hwnd,GWL_USERDATA);
		HeapFree(GetProcessHeap(),0,pRectData);
		return 0;
	}
	return DefMDIChildProc(hwnd,message,wParam,lParam);
}
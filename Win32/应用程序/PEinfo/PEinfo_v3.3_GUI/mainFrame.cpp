#include <windows.h>
#include <winnt.h>
#include <winuser.h>
#include "mainFrame.h"
#include "PEBaseClass.h"
#include "PEImportClass.h"
#include "PEExportClass.h"
#include "disFunc.h"
#include "resource.h"

static TCHAR	szAppName[]		=TEXT("PEInfo");
static TCHAR	szFrameClass[]	=TEXT("MdiFrame");
static TCHAR	szInfoClass[]	=TEXT("Info");

HINSTANCE	hInst;
HMENU		hMenuInit;
HMENU		hMenu;

PEBase	peBase;
PEImport peImport;
PEExport peExport;

HANDLE	tReadHandle;					//线程句柄
HWND	hChildWindow[CHILDWINDOWNUM];	//子窗口句柄

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int iCmdShow)
{
	HWND		hwndFrame;
	MSG			msg;
	WNDCLASS	wc;

	hInst=hInstance;
	tReadHandle=INVALID_HANDLE_VALUE;

	//register frame class
	wc.style		=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc	=FrameWndProc;
	wc.cbClsExtra	=0;
	wc.cbWndExtra	=0;
	wc.hInstance	=hInstance;
	wc.hIcon		=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName	=NULL;
	wc.lpszClassName=szFrameClass;
	if(!RegisterClass(&wc))
		return 0;

	//register PE info child window
	wc.style		=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc	=InfoWndProc;
	wc.cbClsExtra	=0;
	wc.cbWndExtra	=sizeof(HANDLE);
	wc.hInstance	=hInstance;
	wc.hIcon		=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName	=NULL;
	wc.lpszClassName=szInfoClass;
	RegisterClass(&wc);

	hMenu			=LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1));
	hMenuInit		=GetSubMenu(hMenu,0);

	hwndFrame=CreateWindow(
		szFrameClass,
		TEXT("PEInfo"),
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,
		CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,
		hMenu,hInstance,NULL);
	
	ShowWindow(hwndFrame,iCmdShow);
	UpdateWindow(hwndFrame);
	
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	DestroyMenu(hMenu);
	DestroyMenu(hMenuInit);
	return msg.wParam;
}



LRESULT CALLBACK FrameWndProc	(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HWND			hWndClient;
	CLIENTCREATESTRUCT	clientcreate;
	HWND				hwndChild;
	MDICREATESTRUCT		mdicreate;
	OPENFILENAME		ofn;
	char				temp[256];
	int					i;

	switch (message)
	{
	case WM_CREATE:
		clientcreate.hWindowMenu=hMenuInit;
		clientcreate.idFirstChild=IDM_FIRSTCHILD;
		
		hWndClient=CreateWindow(
			TEXT("MDICLIENT"),
			NULL,
			WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,
			0,0,300,300,
			hWnd,
			hMenu,
			hInst,
			(PSTR)&clientcreate);
		
		for(i=0;i<CHILDWINDOWNUM;i++)
		{
			SendMessage(hWnd,XC_OPENWINDOW,(WPARAM)title[i],NULL);
		}
		PostMessage(hWndClient,WM_MDITILE,0,0);
		
		return 0;
	case XC_OPENWINDOW:		//wParam存储将要创建窗口的标题
		mdicreate.szClass	=szInfoClass;
		mdicreate.szTitle	=(char*)wParam;
		mdicreate.hOwner	=hInst;
		mdicreate.x			=CW_USEDEFAULT;
		mdicreate.y			=CW_USEDEFAULT;
		mdicreate.cx		=CW_USEDEFAULT;
		mdicreate.cx		=CW_USEDEFAULT;
		mdicreate.style		=0;
		mdicreate.lParam	=0;
		
		hwndChild=(HWND)SendMessage(hWndClient,
			WM_MDICREATE,0,(LPARAM)(LPMDICREATESTRUCT)&mdicreate);

		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_OPENFILE:
			ZeroMemory(&ofn,sizeof(OPENFILENAME));
			ZeroMemory(temp,sizeof(temp));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = temp;
			ofn.nMaxFile = sizeof(temp);
			ofn.lpstrFilter = "*.exe;*.dll\0\0";
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrTitle = "Please Choose PE file";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			
			if(GetOpenFileName(&ofn)==TRUE)
			{
				if(peBase.Init(ofn.lpstrFile,&(ofn.lpstrFile)[ofn.nFileExtension]))
				{
					SetWindowText(hWnd,(peBase.getPEName(temp),temp));

					tReadHandle=CreateThread(NULL,0,StartReadThread,(LPVOID)&peBase,0,NULL);

					for (i=0;i<CHILDWINDOWNUM;i++)
					{
						SendMessage(hChildWindow[i],XC_INITBITMAP,NULL,NULL);
					}
					SetTimer(hChildWindow[XC_IMPORT_DESCRIPTOR],XC_REDRAWTIMER,XC_REDRAWTIME,NULL);
					SetTimer(hChildWindow[XC_EXPORT_DESCRIPTOR],XC_REDRAWTIMER,XC_REDRAWTIME,NULL);
				}
				else
				{
					MessageBox(NULL,peBase.ErrorString,"Error",MB_OK);
				}

			}
			else
			{
				switch (CommDlgExtendedError())
				{
				case FNERR_BUFFERTOOSMALL:	MessageBox(NULL,"文件路径缓存过小","FNERR_BUFFERTOOSMALL",MB_OK);break;
				case FNERR_INVALIDFILENAME:	MessageBox(NULL,"文件名无效","FNERR_INVALIDFILENAME",MB_OK);break;
				case FNERR_SUBCLASSFAILURE:	MessageBox(NULL,"内存无法读取","FNERR_SUBCLASSFAILURE",MB_OK);break;
				}
			}
			return 0;

		case IDM_EXIT:
			SendMessage(hWnd,WM_CLOSE,0,0);
			return 0;
		case IDM_WINDOWTILE:
			SendMessage(hWndClient,WM_MDITILE,0,0);
			return 0;
		case IDM_WINDOWARRANGE:
			SendMessage(hWndClient,WM_MDIICONARRANGE,0,0);	//排列最小化的窗口，但是不会对没有初始化的窗口有影响
			return 0;
		case IDM_WINDOWCASCADE:
			SendMessage(hWndClient,WM_MDICASCADE,0,0);		
			return 0;
		case IDM_WINDOWCLOSEALL:
			EnumChildWindows(hWndClient,CloseEnumProc,0);	//枚举hwndclient函数的子窗口并依次调用CloseEnumProc
			return 0;
		default:
			hwndChild=(HWND)SendMessage(hWndClient,WM_MDIGETACTIVE,0,0);
			if(IsWindow(hwndChild))
				SendMessage(hwndChild,WM_COMMAND,wParam,lParam);
			break;
		}
		break;
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		SendMessage(hWnd,WM_COMMAND,IDM_WINDOWCLOSEALL,0);
		if(NULL!=GetWindow(hWndClient,GW_CHILD))
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefFrameProc(hWnd,hWndClient,message,wParam,lParam);
}


LRESULT	CALLBACK InfoWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HWND		hWndClient,hwndFrame;
	static HDC		hdcMem[CHILDWINDOWNUM];
	static HBITMAP	hBitmap[CHILDWINDOWNUM];
	PCHILDWININFO	pCWindInfo;

	PAINTSTRUCT		ps;
	RECT			rect;
	HDC				hdc;

	int				i;
	int				nowpos;

	switch (message)
	{
	case WM_TIMER:
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_MDIDESTROY:
		pCWindInfo=(PCHILDWININFO)GetWindowLong(hWnd,GWL_USERDATA);

		DeleteDC(hdcMem[pCWindInfo->ID]);
		DeleteObject(hBitmap[pCWindInfo->ID]);
		HeapFree(GetProcessHeap(),0,pCWindInfo);

		return 0;
	case WM_MOUSEWHEEL:
		PostMessage(hWnd,WM_VSCROLL,(WPARAM)((WORD)XC_SB_MOUSEWHEEL),((int)wParam>0)?((LPARAM)-7):((LPARAM)7));
		break;
	case WM_CREATE:
		for(i=0;i<CHILDWINDOWNUM;i++)
			if(strcmp(((LPCREATESTRUCT)lParam)->lpszName,title[i])==0)
				break;
		hChildWindow[i]=hWnd;

		pCWindInfo=(PCHILDWININFO)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(CHILDWININFO)+1);
		pCWindInfo->ID=i;
		switch (i)
		{
		case XC_NT_INFO:			pCWindInfo->drawBoardSize=2;	break;
		case XC_SECTION_INFO:		pCWindInfo->drawBoardSize=2;	break;
		case XC_IMPORT_DESCRIPTOR:	pCWindInfo->drawBoardSize=1;	break;
		case XC_EXPORT_DESCRIPTOR:	pCWindInfo->drawBoardSize=1;	break;
		}
		SetWindowLong(hWnd,GWL_USERDATA,(long)pCWindInfo);

		hWndClient=GetParent(hWnd);
		hwndFrame=GetParent(hWndClient);

		hdc=GetDC(hWnd);

		hdcMem[i] = CreateCompatibleDC(hdc);
		hBitmap[i] = CreateCompatibleBitmap(hdc,
			GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)*pCWindInfo->drawBoardSize); 
		SelectObject(hdcMem[i], hBitmap[i]);
		ReleaseDC(hWnd, hdc);

		SetBkColor(hdcMem[i],RGB(63,63,63));
		SetTextColor(hdcMem[i],RGB(220,220,204));
		SetBkMode(hdcMem[i], TRANSPARENT); 

		return 0;
	case XC_INITBITMAP:
		pCWindInfo=(PCHILDWININFO)GetWindowLong(hWnd,GWL_USERDATA);

		PatBlt(hdcMem[pCWindInfo->ID],0,0,
			GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)*pCWindInfo->drawBoardSize,
			BLACK_BRUSH);

		rect.left=0;
		rect.top=0;
		rect.right=GetSystemMetrics(SM_CXSCREEN);
		rect.bottom=GetSystemMetrics(SM_CYSCREEN)*pCWindInfo->drawBoardSize;

		DisplayBaseInfo(hdcMem[pCWindInfo->ID],&rect,pCWindInfo->ID);

		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		GetClientRect(hWnd,&rect);

		pCWindInfo=(PCHILDWININFO)GetWindowLong(hWnd,GWL_USERDATA);
		
		if(pCWindInfo->ID==XC_IMPORT_DESCRIPTOR)
		{
			DisplayImportInfo(hdcMem[pCWindInfo->ID],&rect,pCWindInfo);
			BitBlt(hdc,0,0,pCWindInfo->windowSize.x,pCWindInfo->windowSize.y,hdcMem[pCWindInfo->ID],0,0,SRCCOPY);
		}
		else if(pCWindInfo->ID==XC_EXPORT_DESCRIPTOR)
		{
			DisplayExportInfo(hdcMem[pCWindInfo->ID],&rect,pCWindInfo);
			BitBlt(hdc,0,0,pCWindInfo->windowSize.x,pCWindInfo->windowSize.y,hdcMem[pCWindInfo->ID],0,0,SRCCOPY);
		}
		else
		{
			BitBlt(hdc, 0, 0, pCWindInfo->windowSize.x,pCWindInfo->windowSize.y, hdcMem[pCWindInfo->ID], 0, 
				(int)(((float)pCWindInfo->scrollInfo.nPos/pCWindInfo->scrollInfo.nMax)*GetSystemMetrics(SM_CYSCREEN)*pCWindInfo->drawBoardSize), 
				SRCCOPY);
		}		

		EndPaint(hWnd,&ps);
		return 0;
	case WM_CLOSE: 
		ShowWindow(hWnd,SW_MINIMIZE);
		return 0;
	case WM_SIZE:
		pCWindInfo=(PCHILDWININFO)GetWindowLong(hWnd,GWL_USERDATA);
		
		GetClientRect(hWnd,&rect);

		pCWindInfo->windowSize.x=rect.right;
		pCWindInfo->windowSize.y=rect.bottom;
		
		pCWindInfo->scrollInfo.cbSize=sizeof(SCROLLINFO);
		pCWindInfo->scrollInfo.fMask=SIF_PAGE|SIF_POS|SIF_RANGE;
		pCWindInfo->scrollInfo.nMin=0;
		
		if(rect.bottom==0)
			rect.bottom=1;
		pCWindInfo->scrollInfo.nMax=pCWindInfo->drawBoardSize*
			GetSystemMetrics(SM_CYSCREEN)*XC_SCROLL_PAGE/rect.bottom-1;

		if(pCWindInfo->ID==XC_IMPORT_DESCRIPTOR)
			pCWindInfo->scrollInfo.nMax=XC_SCROLL_PAGE*300-1;
		if(pCWindInfo->ID==XC_EXPORT_DESCRIPTOR)
			pCWindInfo->scrollInfo.nMax=XC_SCROLL_PAGE*300-1;

		pCWindInfo->scrollInfo.nPos=0;
		pCWindInfo->scrollInfo.nPage=XC_SCROLL_PAGE;

		SetScrollInfo(hWnd,SB_VERT,&pCWindInfo->scrollInfo,TRUE);
		SetWindowLong(hWnd,GWL_USERDATA,(long)pCWindInfo);
		
		SendMessage(hWnd,WM_VSCROLL,NULL,NULL);
		
		InvalidateRect(hWnd,NULL,TRUE);
		break;
	case WM_VSCROLL:
		pCWindInfo=(PCHILDWININFO)GetWindowLong(hWnd,GWL_USERDATA);
		
		pCWindInfo->scrollInfo.cbSize=sizeof(SCROLLINFO);
		pCWindInfo->scrollInfo.fMask=SIF_ALL;
		GetScrollInfo(hWnd,SB_VERT,&pCWindInfo->scrollInfo);
		nowpos=pCWindInfo->scrollInfo.nPos;
		switch(LOWORD(wParam))
		{
		case SB_TOP:                    // 滚动条处于最上端
			pCWindInfo->scrollInfo.nPos=pCWindInfo->scrollInfo.nMin;
			break;
		case SB_BOTTOM:                 // 滚动条处于最下端
			pCWindInfo->scrollInfo.nPos = pCWindInfo->scrollInfo.nMax;
			break;
		case SB_LINEUP:                 // 向上滚动一行
			pCWindInfo->scrollInfo.nPos -= 1;		
			break;	
		case SB_LINEDOWN:                // 向下滚动一行		
			pCWindInfo->scrollInfo.nPos +=1;
			break;	
		case SB_PAGEUP:                 // 向上滚动一页		
			pCWindInfo->scrollInfo.nPos -=pCWindInfo->scrollInfo.nPage;		
			break;	
		case SB_PAGEDOWN:               // 向下滚动一页	
			pCWindInfo->scrollInfo.nPos +=pCWindInfo->scrollInfo.nPage;		
			break;	
		case SB_THUMBTRACK:              // 拖曳滚动框	
			pCWindInfo->scrollInfo.nPos = pCWindInfo->scrollInfo.nTrackPos;		
			break;	
		case XC_SB_MOUSEWHEEL:			// 该消息是手动添加的，为了响应鼠标滚轮的动作
			pCWindInfo->scrollInfo.nPos += (int)lParam;
			break;
		default:
			break;
		}

		if(pCWindInfo->scrollInfo.nPos>pCWindInfo->scrollInfo.nMax)   // 确保滚动条位于滚动范围内
			pCWindInfo->scrollInfo.nPos=pCWindInfo->scrollInfo.nMax;
		if(pCWindInfo->scrollInfo.nPos<pCWindInfo->scrollInfo.nMin)
			pCWindInfo->scrollInfo.nPos=pCWindInfo->scrollInfo.nMin;
		pCWindInfo->scrollInfo.fMask=SIF_POS;

		SetWindowLong(hWnd,GWL_USERDATA,(long)pCWindInfo);
		SetScrollInfo(hWnd, SB_VERT, &pCWindInfo->scrollInfo, TRUE);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
 	case WM_ERASEBKGND:		//当接收到这个消息后并返回true，即告诉系统你已经重绘界面，不需要它再次重绘
 							//这个消息主要作用是消除闪烁
 		return TRUE;
	}
	return DefMDIChildProc(hWnd,message,wParam,lParam);
}

BOOL CALLBACK CloseEnumProc	(HWND hWnd,LPARAM lParam)
{
	if(GetWindow(hWnd,GW_OWNER))
		return TRUE;
	SendMessage(GetParent(hWnd),WM_MDIRESTORE,(WPARAM)hWnd,0);
	if(!SendMessage(hWnd,WM_QUERYENDSESSION,0,0))
		return TRUE;
	SendMessage(GetParent(hWnd),WM_MDIDESTROY,(WPARAM)hWnd,0);
	return TRUE;
}


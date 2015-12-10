#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <commdlg.h>
#include "resource.h"
#pragma comment(lib,"comctl32.lib")


LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application");

 
HWND hTreeView;
HANDLE handle_driver;
TVINSERTSTRUCT treeinsert;
TVITEM treeitem;
HIMAGELIST himagelist;
HIMAGELIST hdragimagelist;
HICON hicon[4];

BOOL dragmode=FALSE;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID find_file(HANDLE handle,char * addr);

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
		WS_OVERLAPPEDWINDOW, 
		100,
		100, 
		600, 
		600,  
		NULL, 
		NULL,  
		hInstance, 
		NULL);               
	if (!hWnd) 
		return(FALSE);
	ShowWindow(hWnd, SW_SHOW); 
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
	int i,j;
	char temp[100];
	NMHDR nmhdr;
	HANDLE handle_temp;
	NMTREEVIEW nmtreeview;
	TV_HITTESTINFO tvhit;

	switch(uMsg){   
	case WM_CREATE:
		InitCommonControls();
		GetClientRect(hWnd,&rect);
		hTreeView=CreateWindow("SysTreeView32","xingchen_listbox",
			TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|WS_CHILD|WS_VISIBLE,
			rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,
			hWnd,(HMENU)IDC_XC_TREEVIEW,NULL,NULL);
		
		//构造图形列表
		himagelist=ImageList_Create(32,32,ILC_COLOR32,4,10);

		hicon[0]=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_XCICON_remote));
		hicon[1]=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_XCICON_folder));
		hicon[2]=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_XCICON_disk));
		hicon[3]=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_XCICON_file));
		

		//为图形列表加载图标
		ImageList_AddIcon(himagelist,hicon[0]);
		ImageList_AddIcon(himagelist,hicon[1]);
		ImageList_AddIcon(himagelist,hicon[2]);
		ImageList_AddIcon(himagelist,hicon[3]);


		//将图形列表与树形列表绑定
		SendMessage(hTreeView,TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)himagelist);

		for(i=0;i<26;i++){
			wsprintf(temp,"%c:\\",i+'A');
			j=GetDriveType(temp);
			if(j==DRIVE_FIXED     || j==DRIVE_REMOTE || 
			   j==DRIVE_REMOVABLE || j==DRIVE_RAMDISK) 
			{
				treeinsert.hParent=NULL;
				treeinsert.hInsertAfter=TVI_LAST;

				treeitem.mask=TCIF_TEXT|TCIF_IMAGE|TVIF_SELECTEDIMAGE;

				//引用第三个图形列表中的图标
				treeitem.iImage=2;
				treeitem.iSelectedImage=2;
				treeitem.pszText=temp;
				treeinsert.item=treeitem;
				handle_driver=(HANDLE)SendMessage(hTreeView,TVM_INSERTITEM,0,(LPARAM)&treeinsert);
				wsprintf(temp,"%c:",i+'A');
				find_file(handle_driver,temp);
			}
		}
		break;
	case WM_NOTIFY:
		nmhdr=*((NMHDR*)lParam);
		if(nmhdr.hwndFrom==hTreeView){
			if(nmhdr.code==TVN_BEGINDRAG){
				nmtreeview=*((NMTREEVIEW*)lParam);
				hdragimagelist=(HIMAGELIST)SendMessage(hTreeView,TVM_CREATEDRAGIMAGE,0,(LPARAM)(nmtreeview.itemNew.hItem));
				ImageList_BeginDrag(hdragimagelist,0,0,0);
				ImageList_DragEnter(hTreeView,nmtreeview.ptDrag.x,nmtreeview.ptDrag.y);
				SetCapture(hWnd);
                dragmode = TRUE;
			}

		}
		break;
	case WM_LBUTTONUP:
		if(dragmode){
            ImageList_DragLeave(hTreeView);
            ImageList_EndDrag();
            ImageList_Destroy(hdragimagelist);
            HTREEITEM hTmp = (HTREEITEM)SendMessage(hTreeView,TVM_GETNEXTITEM,TVGN_DROPHILITE,0);
            SendMessage(hTreeView,TVM_SELECTITEM,TVGN_CARET,(LPARAM)hTmp);
            SendMessage(hTreeView,TVM_SELECTITEM,TVGN_DROPHILITE,0);
            ReleaseCapture();
            dragmode = FALSE;
		}


		break;
	case WM_MOUSEMOVE:
		if(dragmode){
            tvhit.pt.x = LOWORD(lParam);
            tvhit.pt.y = HIWORD(lParam);
            ImageList_DragMove(tvhit.pt.x,tvhit.pt.y);
            ImageList_DragShowNolock(FALSE);
            HTREEITEM hTmp = (HTREEITEM)SendMessage(hTreeView,TVM_HITTEST,NULL,(LPARAM)&tvhit);
            if(hTmp != NULL)
                SendMessage(hTreeView,TVM_SELECTITEM,TVGN_DROPHILITE,(LPARAM)hTmp);
			
            ImageList_DragShowNolock(TRUE);
        }
		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}


VOID find_file(HANDLE handle,char * addr){
	HANDLE han,hantemp;
	TVINSERTSTRUCT treeinsert;
	TVITEM treeitem;
	WIN32_FIND_DATA fd;
	char temp[1000];
	char nametemp[100];
	wsprintf(temp,"%s\\*",addr);
	
	han=FindFirstFile(temp,&fd);
	while(1){
		if(strcmp(fd.cFileName,".")==0 || strcmp(fd.cFileName,"..")==0){
			if(FindNextFile(han,&fd)==0)
				return ;
			else 
				continue;
		}

		if((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY){
			wsprintf(nametemp,"%s\\",fd.cFileName);
			treeitem.pszText=nametemp;
			treeitem.mask=TVIF_TEXT|TCIF_IMAGE|TVIF_SELECTEDIMAGE;
			treeitem.iImage=1;
			treeitem.iSelectedImage=1;
			treeinsert.hInsertAfter=TVI_FIRST;
		}else{
			treeitem.pszText=fd.cFileName;
			treeitem.mask=TVIF_TEXT|TCIF_IMAGE|TVIF_SELECTEDIMAGE;
			treeitem.iImage=3;
			treeitem.iSelectedImage=3;
			treeinsert.hInsertAfter=TVI_LAST;
		}
			
		treeinsert.item=treeitem;
		treeinsert.hParent=(HTREEITEM)handle;
		hantemp=(HANDLE)SendMessage(hTreeView,TVM_INSERTITEM,0,(LPARAM)&treeinsert);

		if(((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
			&&((fd.dwFileAttributes&FILE_ATTRIBUTE_SYSTEM)!=FILE_ATTRIBUTE_SYSTEM))
		{
			wsprintf(temp,"%s\\%s",addr,fd.cFileName);
			find_file(hantemp,temp);
		}
		if(FindNextFile(han,&fd)==0)
			return ;
	}
}

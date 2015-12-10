#include <windows.h>
#include <Tlhelp32.h>
#include <commctrl.h>
#include <commdlg.h>
#include "WndProc.h"
#include "processOperate.h"

#pragma comment(lib,"comctl32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, 
						 UINT uMsg, 
						 WPARAM wParam,
						 LPARAM lParam)
{
	PROCESSENTRY32 process;
	HANDLE handle;
	RECT rect;
	LVCOLUMN lvcolumn;
	LVITEM   lvitem;
	char temp[1000];
	int i,j;
	NMHDR *nmhdr;
	switch(uMsg){   
	case WM_CREATE:
		SetTimer(hWnd,ProcessControlTimer,20000,NULL);
		PostMessage(hWnd,WM_TIMER,(WPARAM)ProcessControlTimer,NULL);

		GetClientRect(hWnd,&rect);

		hListBox=CreateWindow(TEXT("SysListView32"),"xingchen_listbox",
			LVS_REPORT | LVS_SINGLESEL | LVS_ALIGNLEFT | LVS_NOSORTHEADER | 
			WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE,
			rect.left	,rect.top,
			240			,rect.bottom-rect.top,
			hWnd,(HMENU)IDC_XC_LISTBOX,GetModuleHandle(NULL),NULL);

		hStatic=CreateWindow(TEXT("STATIC"),NULL,WS_VISIBLE|WS_CHILD,
			rect.left+240				,rect.top,
			rect.right-rect.left-240	,75,
			hWnd,(HMENU)IDC_XC_STATIC,NULL,NULL);

		hListBox2=CreateWindow(TEXT("LISTBOX"),NULL,
			WS_CHILD|WS_VISIBLE|LBS_NOTIFY|WS_BORDER|WS_VSCROLL,
			rect.left+240				,rect.top+75,
			rect.right-rect.left-240	,rect.bottom-rect.top-75,
			hWnd,(HMENU)IDC_XC_LISTBOX2,GetModuleHandle(NULL),NULL);

		SendMessage(hListBox,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		SendMessage(hListBox,LVM_DELETEALLITEMS,NULL,NULL);
		SendMessage(hListBox2, LB_RESETCONTENT,0,0);

		lvcolumn.mask=LVCF_TEXT | LVCF_WIDTH;

		wsprintf(temp,"进程名");
		lvcolumn.pszText=temp;
		lvcolumn.cx=160;
		SendMessage(hListBox,LVM_INSERTCOLUMN,0,(LPARAM)&lvcolumn);
		
		wsprintf(temp,"PID");
		lvcolumn.pszText=temp;
		lvcolumn.cx=40;
		SendMessage(hListBox,LVM_INSERTCOLUMN,1,(LPARAM)&lvcolumn);
		
		wsprintf(temp,"PPID");
		lvcolumn.pszText=temp;
		lvcolumn.cx=40;
		SendMessage(hListBox,LVM_INSERTCOLUMN,2,(LPARAM)&lvcolumn);
	
		break;
	case WM_NOTIFY:
		nmhdr=(NMHDR*)lParam;
		if(nmhdr->hwndFrom==hListBox && nmhdr->code==NM_DBLCLK){
			i=SendMessage(hListBox,LVM_GETNEXTITEM,-1,(LPARAM)MAKELPARAM(LVNI_FOCUSED,0));

			ListView_GetItemText(hListBox,i,1,temp,100);
			sscanf(temp,"%d",&j);

			//processScreen(j);
		}else if(nmhdr->hwndFrom==hListBox && nmhdr->code==NM_CLICK){
			i=SendMessage(hListBox,LVM_GETNEXTITEM,-1,(LPARAM)MAKELPARAM(LVNI_FOCUSED,0));
			
			ListView_GetItemText(hListBox,i,1,temp,100);
			sscanf(temp,"%d",&j);
			
			listProcessInfo(hStatic,hListBox2,j);
		}
		break;
	case WM_TIMER:
		
		switch (wParam)
		{
		case ProcessControlTimer:
			handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
			if(handle==INVALID_HANDLE_VALUE){
				MessageBox(NULL,"wrong",NULL,MB_OK);
				KillTimer(hWnd,ProcessControlTimer);
				return 0;
			}
			Process32First(handle,&process);
			SendMessage(hListBox,LVM_DELETEALLITEMS,0,0);

			for(i=0;;i++){
				lvitem.mask=LVIF_TEXT;

				lvitem.iItem=i;
				lvitem.iSubItem=0;				
			
				//列表框被创建的时候某人有一行。必须先插入这一行，再增加新行
				if(i!=0)
					j=SendMessage(hListBox,LVM_INSERTITEM,0,(LPARAM)&lvitem);

				lvitem.iItem=j;

				lvitem.iSubItem=0;
				wsprintf(temp,"%s",process.szExeFile);
				lvitem.pszText=temp;
				SendMessage(hListBox,LVM_SETITEMTEXT,j,(LPARAM)&lvitem);

				lvitem.iSubItem=1;
				wsprintf(temp,"%d",process.th32ProcessID);
				lvitem.pszText=temp;
				SendMessage(hListBox,LVM_SETITEMTEXT,j,(LPARAM)&lvitem);
				
				lvitem.iSubItem=2;
				wsprintf(temp,"%d",process.th32ParentProcessID);
				lvitem.pszText=temp;
				SendMessage(hListBox,LVM_SETITEMTEXT,j,(LPARAM)&lvitem);

				if(!Process32Next(handle,&process))
					break;
			}

			ShowWindow(hListBox,SW_SHOW);
			UpdateWindow(hListBox);
			break;
		}
		break;
	case WM_DESTROY :
		KillTimer(hWnd,ProcessControlTimer);
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}


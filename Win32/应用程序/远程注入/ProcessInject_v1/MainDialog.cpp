#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <Tlhelp32.h>
#include <stdio.h>
#include "InjectFunc.h"
#include "resource.h"
#include "ProcessFunc.h"

#pragma comment(lib,"comctl32.lib")

BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);

HWND hListBox;
HWND hStatic;

int  selectedPID=-1;
char dllFullPath[260];

int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance,
				   LPTSTR lpCmdLine, 
					 int nCmdShow)
{	
	InitCommonControls();
	DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_PROPPAGE_MEDIUM),NULL,(DLGPROC)DialogProc);	

	return 0;
}




BOOL CALLBACK DialogProc(HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam)
{
	RECT rect;
	LVCOLUMN lvcolumn;
	char temp[260];
	NMHDR *nmhdr;
	int i;
	OPENFILENAME ofn;

	switch(uMsg){
	case WM_INITDIALOG:
		GetClientRect(hwndDlg,&rect);

		hListBox=CreateWindow(TEXT("SysListView32"),"xingchen_process",
			LVS_REPORT | LVS_SINGLESEL | LVS_ALIGNLEFT | LVS_NOSORTHEADER | 
			WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE,
			rect.left	,rect.top+80,
			rect.right	,rect.bottom-rect.top-80,
			hwndDlg,(HMENU)IDC_XC_PROCESSLIST,GetModuleHandle(NULL),NULL);
	
		SendMessage(hListBox,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		SendMessage(hListBox,LVM_DELETEALLITEMS,NULL,NULL);
		
		lvcolumn.mask=LVCF_TEXT | LVCF_WIDTH;
		
		wsprintf(temp,"ProcessName");
		lvcolumn.pszText=temp;
		lvcolumn.cx=140;
		SendMessage(hListBox,LVM_INSERTCOLUMN,0,(LPARAM)&lvcolumn);
		
		wsprintf(temp,"PID");
		lvcolumn.pszText=temp;
		lvcolumn.cx=40;
		SendMessage(hListBox,LVM_INSERTCOLUMN,1,(LPARAM)&lvcolumn);
		
		wsprintf(temp,"PPID");
		lvcolumn.pszText=temp;
		lvcolumn.cx=40;
		SendMessage(hListBox,LVM_INSERTCOLUMN,2,(LPARAM)&lvcolumn);

		wsprintf(temp,"ThreadNumber");
		lvcolumn.pszText=temp;
		lvcolumn.cx=100;
		SendMessage(hListBox,LVM_INSERTCOLUMN,3,(LPARAM)&lvcolumn);
		
		listAllProcess(hListBox);
		break;

	case WM_NOTIFY:
		char temp2[40];
		nmhdr=(NMHDR*)lParam;
		if(nmhdr->hwndFrom==hListBox && (nmhdr->code==NM_DBLCLK || nmhdr->code==NM_CLICK)){
			i=SendMessage(hListBox,LVM_GETNEXTITEM,-1,(LPARAM)MAKELPARAM(LVNI_FOCUSED,0));
			ListView_GetItemText(hListBox,i,1,temp,100);
			sscanf(temp,"%d",&selectedPID);
			ListView_GetItemText(hListBox,i,0,temp2,20);
			wsprintf(temp,"PID:%d   %s",selectedPID,temp2);
			SetDlgItemText(hwndDlg,IDC_STATIC_INJECT,temp);
			SetDlgItemText(hwndDlg,IDC_STATIC_SIGN,"waiting");
		}
		break;
	case WM_CLOSE:
		EndDialog(hwndDlg,NULL);
		break;
	case WM_COMMAND:
 		switch(LOWORD(wParam)){
			case IDC_BUTTON1:
				listAllProcess(hListBox);
		 		break;
		 	case IDC_BUTTON2:
				if(EnableDebugPriv(SE_DEBUG_NAME)==0){
					if(InjectDll(dllFullPath,selectedPID)==0){
						SetDlgItemText(hwndDlg,IDC_STATIC_SIGN,"success");
						MessageBox(hwndDlg,"远程注入成功","Inject Success",MB_OK);	
					}else{
						SetDlgItemText(hwndDlg,IDC_STATIC_SIGN,"failed");
						MessageBox(hwndDlg,ErrorMessage,"Inject Failed",MB_OK);
					}					
				}else{
					SetDlgItemText(hwndDlg,IDC_STATIC_SIGN,"failed");
					MessageBox(hwndDlg,ErrorMessage,"Improve Priv Failed!",MB_OK);
				}

				break;
			case IDC_BUTTON3:
				SetDlgItemText(hwndDlg,IDC_STATIC_SIGN,"waiting");
				ZeroMemory(&ofn,sizeof(OPENFILENAME));
				ZeroMemory(temp,sizeof(temp));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hwndDlg;
				ofn.lpstrFile = temp;
				ofn.nMaxFile = sizeof(temp);
				ofn.lpstrFilter = "*.dll";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrTitle = "Please Choose Dll";
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				
 				if(GetOpenFileName(&ofn)==TRUE){
					SetDlgItemText(hwndDlg,IDC_STATIC_SELECT,ofn.lpstrFile);
					wsprintf(dllFullPath,"%s",ofn.lpstrFile);
				}
				break;
		 }
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

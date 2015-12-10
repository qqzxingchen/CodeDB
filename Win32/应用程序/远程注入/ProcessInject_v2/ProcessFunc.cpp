#include <windows.h>
#include <Tlhelp32.h>
#include <stdio.h>
#include <commctrl.h>

int listAllProcess(HWND hDis){
	HANDLE pHandle;	//进程快照句柄
	int i,j;
	PROCESSENTRY32 pSnap;
	LVITEM   lvitem;
	char temp[200];

	SendMessage(hDis,LVM_DELETEALLITEMS,0,0);

	pHandle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
 	if(pHandle==INVALID_HANDLE_VALUE){
 		MessageBox(NULL,"Open Process Snap Error",NULL,MB_OK);
 		return 0;
 	}

	Process32First(pHandle,&pSnap);
	for(i=0;;i++){
		lvitem.mask=LVIF_TEXT;
		
		lvitem.iItem=i;
		lvitem.iSubItem=0;				
		
		//列表框被创建的时候已经有一行空行。必须先向这一行插入数据，再增加新行
		
		if(i!=0){
			j=SendMessage(hDis,LVM_INSERTITEM,0,(LPARAM)&lvitem);
			lvitem.iItem=j;
		}else{
			lvitem.iItem=0;
		}

		lvitem.iSubItem=0;
		wsprintf(temp,"%s",pSnap.szExeFile);
		lvitem.pszText=temp;
		SendMessage(hDis,LVM_SETITEMTEXT,j,(LPARAM)&lvitem);
		
		lvitem.iSubItem=1;
		wsprintf(temp,"%d",pSnap.th32ProcessID);
		lvitem.pszText=temp;
		SendMessage(hDis,LVM_SETITEMTEXT,j,(LPARAM)&lvitem);
		
		lvitem.iSubItem=2;
		wsprintf(temp,"%d",pSnap.th32ParentProcessID);
		lvitem.pszText=temp;
		SendMessage(hDis,LVM_SETITEMTEXT,j,(LPARAM)&lvitem);
		
		lvitem.iSubItem=3;
		wsprintf(temp,"%d",pSnap.cntThreads);
		lvitem.pszText=temp;
		SendMessage(hDis,LVM_SETITEMTEXT,j,(LPARAM)&lvitem);

		if(!Process32Next(pHandle,&pSnap))
			break;
	}
	ShowWindow(hDis,SW_SHOW);
	UpdateWindow(hDis);
	return 0;
}

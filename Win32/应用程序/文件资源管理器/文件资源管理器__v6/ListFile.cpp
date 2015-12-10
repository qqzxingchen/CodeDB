#include <windows.h>
#include <commctrl.h>
#include <math.h>
#include "ListFile.h"
#include "readBMP.h"
#define PI 3.1415926


void ListFile(HANDLE handle,char *addr){
	char temp[1000];
	char nametemp[100];

	HANDLE han,hantemp;
	TVINSERTSTRUCT treeinsert;
	TVITEM treeitem;
	WIN32_FIND_DATA fd;
	
	wsprintf(temp,"%s\\*",addr);
	han=FindFirstFile(temp,&fd);
	if(han==INVALID_HANDLE_VALUE)
		return ;
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
			treeitem.iImage=3;
			treeitem.iSelectedImage=3;
			treeinsert.hInsertAfter=TVI_FIRST;
		}else{
			treeitem.pszText=fd.cFileName;
			treeitem.mask=TVIF_TEXT|TCIF_IMAGE|TVIF_SELECTEDIMAGE;
			treeitem.iImage=4;
			treeitem.iSelectedImage=4;
			treeinsert.hInsertAfter=TVI_LAST;
		}
		
		treeinsert.item=treeitem;
		treeinsert.hParent=(HTREEITEM)handle;
		hantemp=(HANDLE)SendMessage(hTreeView,TVM_INSERTITEM,0,(LPARAM)&treeinsert);
		
		if(FindNextFile(han,&fd)==0)
			return ;
	}

}


void ListFileInfo(HANDLE handle,char *fileaddr){
	
	HDC hdc;
	char temp[1000];
	int size=0;
	int retu;
	WIN32_FIND_DATA fd;
	SYSTEMTIME systemtime;
	RECT rect;
	DWORD num[4];
	FILE *fp;
	XC_bmp xcbmp;

	if(fileaddr[strlen(fileaddr)-1]=='\\')
		fileaddr[strlen(fileaddr)-1]='\0';
	if(fileaddr[strlen(fileaddr)-1]=='\\')
		fileaddr[strlen(fileaddr)-1]='\0';

	if(sign_struct.myMultimediaIsPlaying){
		wsprintf(temp,"%s\nMyMultiMedia is Playing\n\n\n",fileaddr);
	}else{
		wsprintf(temp,"%s\n\n\n\n",fileaddr);
	}
	size=strlen(temp);

	if(strlen(fileaddr)==2){
		int type;

		wsprintf(&temp[size],"�������������ͣ�");
		size=strlen(temp);
		
		type=GetDriveType(fileaddr);
		if(type==DRIVE_UNKNOWN){
			wsprintf(&temp[size],"����������\n");
		}else if(type==DRIVE_REMOVABLE){
			wsprintf(&temp[size],"���ƶ�����\n");
		}else if(type==DRIVE_FIXED){
			wsprintf(&temp[size],"�̶�����\n");
		}else if(type==DRIVE_REMOTE){
			wsprintf(&temp[size],"����������\n");
		}else if(type==DRIVE_CDROM){
			wsprintf(&temp[size],"CD_ROM������\n");
		}else if(type==DRIVE_RAMDISK){
			wsprintf(&temp[size],"RAM DISK\n");
			
		}
		size=strlen(temp);

		GetDiskFreeSpace(fileaddr,&num[0],&num[1],&num[2],&num[3]);
		wsprintf(&temp[size],"\n\n������������ϣ�\n");
		size=strlen(temp);
		wsprintf(&temp[size],"ÿ�ص�������Ϊ��     %u\n",num[0]);
		size=strlen(temp);
		wsprintf(&temp[size],"ÿ���������ֽ����� %u\n",num[1]);
		size=strlen(temp);
		wsprintf(&temp[size],"���еĴ�����            %-10u  (  %dM  )\n",num[2],num[0]*num[1]/1024*num[2]/1024);
		size=strlen(temp);
		wsprintf(&temp[size],"�ܹ��Ĵ�����            %-10u  (  %dM  )\n",num[3],num[0]*num[1]/1024*num[3]/1024);
		size=strlen(temp);

	}else{
		if(FindFirstFile(fileaddr,&fd)==INVALID_HANDLE_VALUE){
			MessageBox(NULL,"wrong",NULL,MB_OK);
			return ;
		}

		FileTimeToSystemTime(&fd.ftCreationTime,&systemtime);
		wsprintf(&temp[size],"�ļ�����ʱ��           ��%-4d-%-2d-%-2d %-2d:%-2d:%-2d\n",
						systemtime.wYear,systemtime.wMonth,systemtime.wDay,
						systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
		size=strlen(temp);

		FileTimeToSystemTime(&fd.ftLastAccessTime,&systemtime);
		wsprintf(&temp[size],"�ļ�������ʱ��    ��%-4d-%-2d-%-2d %-2d:%-2d:%-2d\n",
			systemtime.wYear,systemtime.wMonth,systemtime.wDay,
			systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
		size=strlen(temp);

		FileTimeToSystemTime(&fd.ftLastWriteTime,&systemtime);
		wsprintf(&temp[size],"�ļ�����޸�ʱ��    ��%-4d-%-2d-%-2d %-2d:%-2d:%-2d\n\n",
						systemtime.wYear,systemtime.wMonth,systemtime.wDay,
						systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
		size=strlen(temp);
		
		wsprintf(&temp[size],"�ļ���С��   %-10d�ֽ�\n\n", (fd.nFileSizeHigh * (MAXDWORD+1)) + fd.nFileSizeLow);
		size=strlen(temp);
		
		wsprintf(&temp[size],"�ļ����ͣ�  ");
		size=strlen(temp);
		
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY){
			wsprintf(&temp[size],"Ŀ¼�ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)==FILE_ATTRIBUTE_ARCHIVE){
			wsprintf(&temp[size],"�浵�ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)==FILE_ATTRIBUTE_COMPRESSED){
			wsprintf(&temp[size],"ѹ���ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)==FILE_ATTRIBUTE_ENCRYPTED){
			wsprintf(&temp[size],"�����ļ� ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==FILE_ATTRIBUTE_HIDDEN){
			wsprintf(&temp[size],"�����ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)==FILE_ATTRIBUTE_NORMAL){
			wsprintf(&temp[size],"��ͨ�ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)==FILE_ATTRIBUTE_OFFLINE){
			wsprintf(&temp[size],"�ѻ��ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)==FILE_ATTRIBUTE_READONLY){
			wsprintf(&temp[size],"ֻ���ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)==FILE_ATTRIBUTE_REPARSE_POINT){
			wsprintf(&temp[size],"�ؽ����ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)==FILE_ATTRIBUTE_SPARSE_FILE){
			wsprintf(&temp[size],"ϡ���ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)==FILE_ATTRIBUTE_SYSTEM){
			wsprintf(&temp[size],"ϵͳ�ļ�  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)==FILE_ATTRIBUTE_TEMPORARY){
			wsprintf(&temp[size],"��ʱ�ļ�  ");
			size=strlen(temp);
		}
	}
	SetWindowText(hStatic,temp);
	
	hdc=GetDC(hStatic);
	if(strlen(fileaddr)==2){
		double i=(double)num[2]/num[3];
		SelectObject(hdc,CreateSolidBrush(RGB(200,0,0)));
		Ellipse(hdc, 100, 200, 300, 400);
		SelectObject(hdc,CreateSolidBrush(RGB(0,0,200)));
		Pie(hdc, 100, 200, 300, 400,300,299,(int)(200+100*cos(i*2*PI)),(int)(300-100*sin(i*2*PI)));
	}

	if(strcmp(&fileaddr[strlen(fileaddr)-4],".bmp")==0){
		fp=fopen(fileaddr,"rb");
		retu=readBMPFile(fp,&xcbmp);
		if(retu==0){
			disBMP(hdc,fp,&xcbmp,0,200);
			fseek(fp,xcbmp.bmpfileheader.bfOffBits,0);
		}else{
			GetClientRect(hStatic, &rect);
			rect.top+=200;
			SetTextColor(hdc, RGB(255,0,0));
			
			if(retu==1){
				DrawText (hdc,TEXT("���ļ�����BMP�ļ�"), -1, &rect,DT_SINGLELINE) ;	
			}else if(retu==2){
				DrawText (hdc,TEXT("���ļ���BMP�ļ��������Ѿ���ѹ��"), -1, &rect,DT_SINGLELINE) ;	
			}else{
				DrawText (hdc,TEXT("��bmp�ļ���������Ϊ1,4,8,24�е�һ��"), -1, &rect,DT_SINGLELINE) ;	
			}
		}
		fclose(fp);
	}
	ReleaseDC(hStatic,hdc);
}
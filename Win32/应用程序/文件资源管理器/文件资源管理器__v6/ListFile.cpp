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

		wsprintf(&temp[size],"磁盘驱动器类型：");
		size=strlen(temp);
		
		type=GetDriveType(fileaddr);
		if(type==DRIVE_UNKNOWN){
			wsprintf(&temp[size],"不明驱动器\n");
		}else if(type==DRIVE_REMOVABLE){
			wsprintf(&temp[size],"可移动磁盘\n");
		}else if(type==DRIVE_FIXED){
			wsprintf(&temp[size],"固定磁盘\n");
		}else if(type==DRIVE_REMOTE){
			wsprintf(&temp[size],"网络驱动器\n");
		}else if(type==DRIVE_CDROM){
			wsprintf(&temp[size],"CD_ROM驱动器\n");
		}else if(type==DRIVE_RAMDISK){
			wsprintf(&temp[size],"RAM DISK\n");
			
		}
		size=strlen(temp);

		GetDiskFreeSpace(fileaddr,&num[0],&num[1],&num[2],&num[3]);
		wsprintf(&temp[size],"\n\n在这个驱动器上：\n");
		size=strlen(temp);
		wsprintf(&temp[size],"每簇的扇区数为：     %u\n",num[0]);
		size=strlen(temp);
		wsprintf(&temp[size],"每个扇区的字节数： %u\n",num[1]);
		size=strlen(temp);
		wsprintf(&temp[size],"空闲的簇数：            %-10u  (  %dM  )\n",num[2],num[0]*num[1]/1024*num[2]/1024);
		size=strlen(temp);
		wsprintf(&temp[size],"总共的簇数：            %-10u  (  %dM  )\n",num[3],num[0]*num[1]/1024*num[3]/1024);
		size=strlen(temp);

	}else{
		if(FindFirstFile(fileaddr,&fd)==INVALID_HANDLE_VALUE){
			MessageBox(NULL,"wrong",NULL,MB_OK);
			return ;
		}

		FileTimeToSystemTime(&fd.ftCreationTime,&systemtime);
		wsprintf(&temp[size],"文件创建时间           ：%-4d-%-2d-%-2d %-2d:%-2d:%-2d\n",
						systemtime.wYear,systemtime.wMonth,systemtime.wDay,
						systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
		size=strlen(temp);

		FileTimeToSystemTime(&fd.ftLastAccessTime,&systemtime);
		wsprintf(&temp[size],"文件最后访问时间    ：%-4d-%-2d-%-2d %-2d:%-2d:%-2d\n",
			systemtime.wYear,systemtime.wMonth,systemtime.wDay,
			systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
		size=strlen(temp);

		FileTimeToSystemTime(&fd.ftLastWriteTime,&systemtime);
		wsprintf(&temp[size],"文件最后修改时间    ：%-4d-%-2d-%-2d %-2d:%-2d:%-2d\n\n",
						systemtime.wYear,systemtime.wMonth,systemtime.wDay,
						systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
		size=strlen(temp);
		
		wsprintf(&temp[size],"文件大小：   %-10d字节\n\n", (fd.nFileSizeHigh * (MAXDWORD+1)) + fd.nFileSizeLow);
		size=strlen(temp);
		
		wsprintf(&temp[size],"文件类型：  ");
		size=strlen(temp);
		
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY){
			wsprintf(&temp[size],"目录文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)==FILE_ATTRIBUTE_ARCHIVE){
			wsprintf(&temp[size],"存档文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)==FILE_ATTRIBUTE_COMPRESSED){
			wsprintf(&temp[size],"压缩文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)==FILE_ATTRIBUTE_ENCRYPTED){
			wsprintf(&temp[size],"加密文件 ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==FILE_ATTRIBUTE_HIDDEN){
			wsprintf(&temp[size],"隐藏文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)==FILE_ATTRIBUTE_NORMAL){
			wsprintf(&temp[size],"普通文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)==FILE_ATTRIBUTE_OFFLINE){
			wsprintf(&temp[size],"脱机文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)==FILE_ATTRIBUTE_READONLY){
			wsprintf(&temp[size],"只读文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)==FILE_ATTRIBUTE_REPARSE_POINT){
			wsprintf(&temp[size],"重解析文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)==FILE_ATTRIBUTE_SPARSE_FILE){
			wsprintf(&temp[size],"稀疏文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)==FILE_ATTRIBUTE_SYSTEM){
			wsprintf(&temp[size],"系统文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)==FILE_ATTRIBUTE_TEMPORARY){
			wsprintf(&temp[size],"临时文件  ");
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
				DrawText (hdc,TEXT("该文件不是BMP文件"), -1, &rect,DT_SINGLELINE) ;	
			}else if(retu==2){
				DrawText (hdc,TEXT("该文件是BMP文件，但是已经被压缩"), -1, &rect,DT_SINGLELINE) ;	
			}else{
				DrawText (hdc,TEXT("该bmp文件像素数不为1,4,8,24中的一种"), -1, &rect,DT_SINGLELINE) ;	
			}
		}
		fclose(fp);
	}
	ReleaseDC(hStatic,hdc);
}
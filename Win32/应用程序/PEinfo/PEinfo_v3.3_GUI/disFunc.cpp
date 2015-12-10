#include "PEBaseClass.h"
#include "PEImportClass.h"
#include "PEExportClass.h"
#include "mainFrame.h"
#include "disFunc.h"

extern PEBase	peBase;
extern PEImport peImport;
extern PEExport peExport;
extern HANDLE	tReadHandle;	//线程句柄
extern HWND		hChildWindow[CHILDWINDOWNUM];	//子窗口句柄

void DisplayBaseInfo(HDC hdc,PRECT pRect,int disId)
{
	char str[100];
	int i;

	switch (disId)
	{
	case XC_NT_INFO:
		DisOneLine(hdc,pRect,"MZ\0\0",NULL,0);
		wsprintf(str,"0x %-8X",peBase.dosHeader.e_lfanew);
		DisOneLine(hdc,pRect,"DOS HEADER.e_lfanew",str,0);
		
		pRect->top+=10;

		DisOneLine(hdc,pRect,"PE\0\0",NULL,0);
		DisOneLine(hdc,pRect,"NT Header start",NULL,0);
		DisOneLine(hdc,pRect,"File Header start",NULL,RIGHTSTEP);
		
		switch ((int)peBase.ntHeader.FileHeader.Machine)
		{
		case IMAGE_FILE_MACHINE_UNKNOWN  :wsprintf(str,"0x %-8X   UNKNOWN  ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_I386     :wsprintf(str,"0x %-8X   I386     ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_R3000    :wsprintf(str,"0x %-8X   R3000    ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_R4000    :wsprintf(str,"0x %-8X   R4000    ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_R10000   :wsprintf(str,"0x %-8X   R10000   ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_WCEMIPSV2:wsprintf(str,"0x %-8X   WCEMIPSV2",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_ALPHA    :wsprintf(str,"0x %-8X   ALPHA    ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_POWERPC  :wsprintf(str,"0x %-8X   POWERPC  ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_SH3      :wsprintf(str,"0x %-8X   SH3      ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_SH3E     :wsprintf(str,"0x %-8X   SH3E     ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_SH4      :wsprintf(str,"0x %-8X   SH4      ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_ARM      :wsprintf(str,"0x %-8X   ARM      ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_THUMB    :wsprintf(str,"0x %-8X   THUMB    ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_IA64     :wsprintf(str,"0x %-8X   IA64     ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_MIPS16   :wsprintf(str,"0x %-8X   MIPS16   ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_MIPSFPU  :wsprintf(str,"0x %-8X   MIPSFPU  ",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_MIPSFPU16:wsprintf(str,"0x %-8X   MIPSFPU16",peBase.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_ALPHA64  :wsprintf(str,"0x %-8X   ALPHA64  ",peBase.ntHeader.FileHeader.Machine);break;
		default:wsprintf(str,"0x %-8X   ERROR   ",peBase.ntHeader.FileHeader.Machine);
		}
		DisOneLine(hdc,pRect,"Machine",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peBase.ntHeader.FileHeader.NumberOfSections);
		DisOneLine(hdc,pRect,"NumberOfSections",str,RIGHTSTEP+RIGHTSTEP);

		DisOneLine(hdc,pRect,"TimeDateStamp",NULL,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peBase.ntHeader.FileHeader.PointerToSymbolTable);
		DisOneLine(hdc,pRect,"PointerToSymbolTable",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peBase.ntHeader.FileHeader.NumberOfSymbols);
		DisOneLine(hdc,pRect,"NumberOfSymbols",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peBase.ntHeader.FileHeader.SizeOfOptionalHeader);
		DisOneLine(hdc,pRect,"SizeOfOptionalHeader",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peBase.ntHeader.FileHeader.Characteristics);
		DisOneLine(hdc,pRect,"Characteristics",str,RIGHTSTEP+RIGHTSTEP);
		
		i=0;
		while(i<16)
		{
			if(peBase.ntHeader.FileHeader.Characteristics & 1<<i)
			{		
				DisOneLine(hdc,pRect,(char*)PEBase.CharacterStr[i],NULL,RIGHTSTEP+RIGHTSTEP+RIGHTSTEP);
			}
			i++;
		}
		DisOneLine(hdc,pRect,"File Header end",NULL,RIGHTSTEP);
		DisOneLine(hdc,pRect,"Optional Header start",NULL,RIGHTSTEP);
		
		switch (peBase.ntHeader.OptionalHeader.Magic)
		{
		case IMAGE_NT_OPTIONAL_HDR32_MAGIC:wsprintf(str,"PE32");break;
		case IMAGE_NT_OPTIONAL_HDR64_MAGIC:wsprintf(str,"PE64");break;
		case IMAGE_ROM_OPTIONAL_HDR_MAGIC :wsprintf(str,"ROM映像");break;
		default:
			wsprintf(str,"ERROR");
			break;
		}
		DisOneLine(hdc,pRect,"Magic",str,RIGHTSTEP+RIGHTSTEP);
		DisOneLine(hdc,pRect,"MajorLinkversion",NULL,RIGHTSTEP+RIGHTSTEP);
		DisOneLine(hdc,pRect,"MinorLinkversion",NULL,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X   Base File",peBase.ntHeader.OptionalHeader.SizeOfCode);
		DisOneLine(hdc,pRect,"SizeOfCode",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.SizeOfInitializedData);
		DisOneLine(hdc,pRect,"SizeOfInitializedData",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.SizeOfUninitializedData);
		DisOneLine(hdc,pRect,"SizeOfUninitializedData",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X   (RVA)",peBase.ntHeader.OptionalHeader.AddressOfEntryPoint);
		DisOneLine(hdc,pRect,"AddressOfEntryPoint",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X   (RVA)",peBase.ntHeader.OptionalHeader.BaseOfCode);
		DisOneLine(hdc,pRect,"Code Section Start",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X   (RVA)",peBase.ntHeader.OptionalHeader.BaseOfData);
		DisOneLine(hdc,pRect,"Data Section Start",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X   (VA)",peBase.ntHeader.OptionalHeader.ImageBase);
		DisOneLine(hdc,pRect,"ImageBase",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.SectionAlignment);
		DisOneLine(hdc,pRect,"SectionAlignment",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.FileAlignment);
		DisOneLine(hdc,pRect,"FileAlignment",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.MajorOperatingSystemVersion);
		DisOneLine(hdc,pRect,"MajorOSVersion",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.MinorOperatingSystemVersion);
		DisOneLine(hdc,pRect,"MinorOSVersion",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.MajorImageVersion);
		DisOneLine(hdc,pRect,"MajorImageVersion",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.MinorImageVersion);
		DisOneLine(hdc,pRect,"MinorImageVersion",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.MajorSubsystemVersion);
		DisOneLine(hdc,pRect,"MajorSubsystemVersion",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.MinorSubsystemVersion);
		DisOneLine(hdc,pRect,"MinorSubsystemVersion",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X   Must be 0",peBase.ntHeader.OptionalHeader.Win32VersionValue);
		DisOneLine(hdc,pRect,"Win32VersionValue",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.SizeOfImage);
		DisOneLine(hdc,pRect,"SizeOfImage",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.SizeOfHeaders);
		DisOneLine(hdc,pRect,"SizeOfHeaders",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.CheckSum);
		DisOneLine(hdc,pRect,"CheckSum",str,RIGHTSTEP+RIGHTSTEP);
		
		switch (peBase.ntHeader.OptionalHeader.Subsystem)
		{
		case IMAGE_SUBSYSTEM_UNKNOWN       :wsprintf(str,"0x %-8X   未知          ",peBase.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_NATIVE        :wsprintf(str,"0x %-8X   设备驱动程序和NativeWindows进程        ",peBase.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_WINDOWS_GUI   :wsprintf(str,"0x %-8X   win图形界面   ",peBase.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_WINDOWS_CUI   :wsprintf(str,"0x %-8X   win字符界面   ",peBase.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_OS2_CUI       :wsprintf(str,"0x %-8X   OS2字符界面   ",peBase.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_POSIX_CUI     :wsprintf(str,"0x %-8X   POSIX字符界面 ",peBase.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_NATIVE_WINDOWS:wsprintf(str,"0x %-8X   NATIVE_WINDOWS",peBase.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:wsprintf(str,"0x %-8X   wince图形界面 ",peBase.ntHeader.OptionalHeader.Subsystem);break;
		default:wsprintf(str,"0x %-8X   ERROR",peBase.ntHeader.OptionalHeader.Subsystem);
		}
		DisOneLine(hdc,pRect,"Subsystem",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.DllCharacteristics);
		DisOneLine(hdc,pRect,"DllCharacteristics",str,RIGHTSTEP+RIGHTSTEP);
		
		i=0;
		while(i<16)
		{
			if((peBase.ntHeader.OptionalHeader.DllCharacteristics & (1<<i))!=0)
			{		
				DisOneLine(hdc,pRect,(char*)peBase.DllCharacterStr[i],NULL,RIGHTSTEP+RIGHTSTEP+RIGHTSTEP);
			}
			i++;
		}

		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.SizeOfStackReserve);
		DisOneLine(hdc,pRect,"SizeOfStackReserve",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.SizeOfStackCommit);
		DisOneLine(hdc,pRect,"SizeOfStackCommit",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.SizeOfHeapReserve);
		DisOneLine(hdc,pRect,"SizeOfHeapReserve",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.SizeOfHeapCommit);
		DisOneLine(hdc,pRect,"SizeOfHeapCommit",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peBase.ntHeader.OptionalHeader.LoaderFlags);
		DisOneLine(hdc,pRect,"LoaderFlags",str,RIGHTSTEP+RIGHTSTEP);

		pRect->top+=4;
		DisOneLine(hdc,pRect,"Data Directory will display on the other window",NULL,RIGHTSTEP+RIGHTSTEP);
		pRect->top+=4;

		DisOneLine(hdc,pRect,"Optional Header end",NULL,RIGHTSTEP);
		DisOneLine(hdc,pRect,"NTHeader end",NULL,0);
		break;
	case XC_SECTION_INFO:
		DrawText(hdc,"节名称",-1,pRect,DT_LEFT|DT_SINGLELINE);
		pRect->left+=100;
		DrawText(hdc,"节RVA",-1,pRect,DT_LEFT|DT_SINGLELINE);
		pRect->left+=100;
		DrawText(hdc,"节FOA",-1,pRect,DT_LEFT|DT_SINGLELINE);
		pRect->left+=100;
		DrawText(hdc,"节大小(File)",-1,pRect,DT_LEFT|DT_SINGLELINE);
		pRect->left+=100;
		DrawText(hdc,"RWE",-1,pRect,DT_LEFT|DT_SINGLELINE);
		pRect->left+=100;
		DrawText(hdc,"节属性",-1,pRect,DT_LEFT|DT_SINGLELINE);
		pRect->left-=500;
		for(i=0;i<peBase.ntHeader.FileHeader.NumberOfSections;i++)
		{
			pRect->top+=XC_ONELINEHEIGHT;

			wsprintf(str,"%-10s",((PIMAGE_SECTION_HEADER)peBase.pSection)[i].Name);
			DrawText(hdc,str,-1,pRect,DT_LEFT|DT_SINGLELINE);
			pRect->left+=100;
			
			wsprintf(str,"0x %-8X",((PIMAGE_SECTION_HEADER)peBase.pSection)[i].VirtualAddress);
			DrawText(hdc,str,-1,pRect,DT_LEFT|DT_SINGLELINE);
			pRect->left+=100;
			
			
			wsprintf(str,"0x %-8X",((PIMAGE_SECTION_HEADER)peBase.pSection)[i].PointerToRawData);
			DrawText(hdc,str,-1,pRect,DT_LEFT|DT_SINGLELINE);
			pRect->left+=100;
			
			wsprintf(str,"0x %-8X",((PIMAGE_SECTION_HEADER)peBase.pSection)[i].SizeOfRawData);
			DrawText(hdc,str,-1,pRect,DT_LEFT|DT_SINGLELINE);
			pRect->left+=100;
			
			str[0]='\0';
			if((((PIMAGE_SECTION_HEADER)peBase.pSection)[i].Characteristics & IMAGE_SCN_MEM_READ) == IMAGE_SCN_MEM_READ)
				wsprintf(&str[strlen(str)],"R");
			if((((PIMAGE_SECTION_HEADER)peBase.pSection)[i].Characteristics & IMAGE_SCN_MEM_WRITE) == IMAGE_SCN_MEM_WRITE)
				wsprintf(&str[strlen(str)],"W");			
			if((((PIMAGE_SECTION_HEADER)peBase.pSection)[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) == IMAGE_SCN_MEM_EXECUTE)
				wsprintf(&str[strlen(str)],"E");
			DrawText(hdc,str,-1,pRect,DT_LEFT|DT_SINGLELINE);
			pRect->left+=100;
			
			wsprintf(str,"0x %-8X",((PIMAGE_SECTION_HEADER)peBase.pSection)[i].Characteristics);
			DrawText(hdc,str,-1,pRect,DT_LEFT|DT_SINGLELINE);

			pRect->left-=500;
		}
		pRect->top+=XC_ONELINEHEIGHT;
		
		pRect->top+=XC_ONELINEHEIGHT;
		DisOneLine(hdc,pRect,"IMAGE_DATA_DIRECTORY Start",NULL,0);
		for (i=0;i<(int)(peBase.ntHeader.OptionalHeader.NumberOfRvaAndSizes);i++)
		{
			pRect->left+=RIGHTSTEP;
			DrawText(hdc,peBase.DataDirectoryName[i],-1,pRect,DT_LEFT|DT_SINGLELINE);
			pRect->left+=RIGHTSTEP*8;

			wsprintf(str,"RVA:0x %-8X",peBase.ntHeader.OptionalHeader.DataDirectory[i].VirtualAddress);
			DrawText(hdc,str,-1,pRect,DT_LEFT|DT_SINGLELINE);
			pRect->left+=RIGHTSTEP*6;

			wsprintf(str,"FOA:0x %-8X",peBase.RvaToFoa(peBase.ntHeader.OptionalHeader.DataDirectory[i].VirtualAddress));
			DrawText(hdc,str,-1,pRect,DT_LEFT|DT_SINGLELINE);
			pRect->left+=RIGHTSTEP*6;
			
			wsprintf(str,"SIZE:0x %-8X",peBase.ntHeader.OptionalHeader.DataDirectory[i].Size);
			DrawText(hdc,str,-1,pRect,DT_LEFT|DT_SINGLELINE);

			pRect->left-=RIGHTSTEP*21;
			pRect->top+=XC_ONELINEHEIGHT;
		}
		DisOneLine(hdc,pRect,"IMAGE_DATA_DIRECTORY End",NULL,0);
		pRect->top+=XC_ONELINEHEIGHT;

		pRect->top+=XC_ONELINEHEIGHT/2;
		DisOneLine(hdc,pRect,"IMAGE_SECTION_HEADER.Characteristics",NULL,0);
		DisOneLine(hdc,pRect,"0x 00000020","节中包含代码",0);
		DisOneLine(hdc,pRect,"0x 00000040","节中包含已初始化数据",0);
		DisOneLine(hdc,pRect,"0x 00000080","节中包含未初始化数据",0);

		DisOneLine(hdc,pRect,"0x 02000000","节中数据会在进程开始后丢弃",0);
		DisOneLine(hdc,pRect,"0x 04000000","节中数据不会经过缓存",0);
		DisOneLine(hdc,pRect,"0x 08000000","节中数据不会被交换到硬盘",0);

		DisOneLine(hdc,pRect,"0x 10000000","节中数据将会被不同进程共享",0);
		DisOneLine(hdc,pRect,"0x 20000000","节中数据可执行",0);
		DisOneLine(hdc,pRect,"0x 40000000","节中数据可读",0);
		DisOneLine(hdc,pRect,"0x 40000000","节中数据可写",0);
		break;
	}
}

void DisOneLine(HDC hdc,PRECT pRect,char *ID,char *Value,int direct)
{
	int left;
	
	left=pRect->left;		//保存原值
	
	pRect->left+=direct;		//修改rect.left
	
	if(pRect->left<0)		//修正可能出现的错误
		pRect->left=0;
	if(ID!=NULL)
	{
		DrawText(hdc,ID,-1,pRect,DT_LEFT|DT_SINGLELINE);
	}
	if(Value!=NULL)
	{
		pRect->left+=RIGHTSTEP*10;
		DrawText(hdc,Value,-1,pRect,DT_LEFT|DT_SINGLELINE);
	}
	pRect->top+=XC_ONELINEHEIGHT;
	
	pRect->left=left;		//恢复原值
}

DWORD WINAPI StartReadThread( LPVOID point )
{
	peImport.Init(peBase);
	peExport.Init(peBase);	
	return 0;
}


void DisplayImportInfo( HDC hdc,PRECT pRect,PCHILDWININFO pCWindInfo )
{
	int i;
	PatBlt(hdc,0,0,
		GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)*pCWindInfo->drawBoardSize,
		BLACK_BRUSH);

	if(	tReadHandle!=INVALID_HANDLE_VALUE && WaitForSingleObject(tReadHandle,0)!=WAIT_TIMEOUT)
	{
		KillTimer(hChildWindow[XC_IMPORT_DESCRIPTOR],XC_REDRAWTIMER);
		for(i=(int)peImport.num*pCWindInfo->scrollInfo.nPos/pCWindInfo->scrollInfo.nMax;
		((i<(int)peImport.num*pCWindInfo->scrollInfo.nPos/pCWindInfo->scrollInfo.nMax
			+(int)GetSystemMetrics(SM_CYSCREEN)/XC_ONELINEHEIGHT) && (i<peImport.num));i++)
		{	
			if(((PONEIMPORTLINE)(peImport.pAllImport[i]))->ID[0]=='H')
				pRect->left+=40;
			if(((PONEIMPORTLINE)(peImport.pAllImport[i]))->ID[0]=='O' ||
				((PONEIMPORTLINE)(peImport.pAllImport[i]))->ID[0]=='T' ||
				((PONEIMPORTLINE)(peImport.pAllImport[i]))->ID[0]=='F')
				pRect->left+=20;
			
			DrawText(hdc,((PONEIMPORTLINE)(peImport.pAllImport[i]))->ID,-1,pRect,DT_SINGLELINE|DT_LEFT);
			pRect->left+=150;
			DrawText(hdc,(char*)((PONEIMPORTLINE)(peImport.pAllImport[i]))->Value,-1,pRect,DT_SINGLELINE|DT_LEFT);
			pRect->left-=150;
			
			if(((PONEIMPORTLINE)(peImport.pAllImport[i]))->ID[0]=='O' ||
				((PONEIMPORTLINE)(peImport.pAllImport[i]))->ID[0]=='T' ||
				((PONEIMPORTLINE)(peImport.pAllImport[i]))->ID[0]=='F')						
				pRect->left-=20;
			if(((PONEIMPORTLINE)(peImport.pAllImport[i]))->ID[0]=='H')
				pRect->left-=40;
			
			pRect->top+=XC_ONELINEHEIGHT;
		}
	}	
}

void DisplayExportInfo( HDC hdc,PRECT pRect,PCHILDWININFO pCWindInfo )
{
	int i;
	char temp[60];

	PatBlt(hdc,0,0,
		GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)*pCWindInfo->drawBoardSize,
		BLACK_BRUSH);

	if(	tReadHandle!=INVALID_HANDLE_VALUE && WaitForSingleObject(tReadHandle,0)!=WAIT_TIMEOUT)
	{
		KillTimer(hChildWindow[XC_EXPORT_DESCRIPTOR],XC_REDRAWTIMER);
		if(peExport.NumberOfFunctions==0)
		{
			DrawText(hdc,"该文件无导出表",-1,pRect,DT_SINGLELINE|DT_LEFT);
			return ;
		}

		for(i=(int)(peExport.NumberOfFunctions+6)*pCWindInfo->scrollInfo.nPos/pCWindInfo->scrollInfo.nMax;
		((i<(int)(peExport.NumberOfFunctions+6)*pCWindInfo->scrollInfo.nPos/pCWindInfo->scrollInfo.nMax
			+(int)GetSystemMetrics(SM_CYSCREEN)/XC_ONELINEHEIGHT) && (i<(int)peExport.NumberOfFunctions+6));i++)
		{	
			if(i<6)
			{
				switch (i)
				{
				case 0:				
					wsprintf(temp,"0x %X",peExport.NumberOfFunctions);
					DisOneLine(hdc,pRect,"NumberOfFunctions",temp,0);
					break;
				case 1:
					wsprintf(temp,"0x %X",peExport.NumberOfNames);
					DisOneLine(hdc,pRect,"NumberOfNames",temp,0);					
					break;
				case 2:
					wsprintf(temp,"0x %X",peExport.nBase);
					DisOneLine(hdc,pRect,"nBase",temp,0);
					break;
				case 3:
					wsprintf(temp,"0x %X",peExport.AddressOfFunctions);
					DisOneLine(hdc,pRect,"AddressOfFunctions",temp,0);
					break;
				case 4:
					wsprintf(temp,"0x %X",peExport.AddressOfNames);
					DisOneLine(hdc,pRect,"AddressOfNames",temp,0);
					break;
				case 5:
					wsprintf(temp,"0x %X",peExport.AddressOfNameOrdinals);
					DisOneLine(hdc,pRect,"AddressOfNameOrdinals",temp,0);
					break;
				}
			}
			else
			{
				wsprintf(temp,"ID:0x %X",((PONEEXPORTLINE)(peExport.pAllExport[i-6]))->ID);
				DrawText(hdc,temp,-1,pRect,DT_SINGLELINE|DT_LEFT);

				pRect->left+=100;

				wsprintf(temp,"Addr:0x %X",((PONEEXPORTLINE)(peExport.pAllExport[i-6]))->addr);
				DrawText(hdc,temp,-1,pRect,DT_SINGLELINE|DT_LEFT);
				
				pRect->left+=200;
 
				wsprintf(temp,"AddrInMemory:0x %X",((PONEEXPORTLINE)(peExport.pAllExport[i-6]))->addrInMemory);
				DrawText(hdc,temp,-1,pRect,DT_SINGLELINE|DT_LEFT);
				
				pRect->left+=300;

				DrawText(hdc,(char*)((PONEEXPORTLINE)(peExport.pAllExport[i-6]))->name,-1,pRect,DT_SINGLELINE|DT_LEFT);

				pRect->left-=600;
				pRect->top+=XC_ONELINEHEIGHT;
			}
		}
	}	
}
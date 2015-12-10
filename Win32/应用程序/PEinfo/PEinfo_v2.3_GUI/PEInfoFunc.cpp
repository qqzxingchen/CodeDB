#include <windows.h>
#include "PEInfoFunc.h"
#include "resource.h"

void DisplayInfo(HDC hdc,PRECT rect,int disId,const PEINFO peInfo)
{
	char str[100];
	int i;

	switch (disId)
	{
	case XC_NT_INFO:
		DisOneLine(hdc,rect,"MZ\0\0",NULL,0);
		wsprintf(str,"0x %-8X",peInfo.dosHeader.e_lfanew);
		DisOneLine(hdc,rect,"DOS HEADER.e_lfanew",str,0);
		
		rect->top+=10;

		DisOneLine(hdc,rect,"PE\0\0",NULL,0);
		DisOneLine(hdc,rect,"NT Header start",NULL,0);
		DisOneLine(hdc,rect,"File Header start",NULL,RIGHTSTEP);
		
		switch ((int)peInfo.ntHeader.FileHeader.Machine)
		{
		case IMAGE_FILE_MACHINE_UNKNOWN  :wsprintf(str,"0x %-8X   UNKNOWN  ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_I386     :wsprintf(str,"0x %-8X   I386     ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_R3000    :wsprintf(str,"0x %-8X   R3000    ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_R4000    :wsprintf(str,"0x %-8X   R4000    ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_R10000   :wsprintf(str,"0x %-8X   R10000   ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_WCEMIPSV2:wsprintf(str,"0x %-8X   WCEMIPSV2",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_ALPHA    :wsprintf(str,"0x %-8X   ALPHA    ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_POWERPC  :wsprintf(str,"0x %-8X   POWERPC  ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_SH3      :wsprintf(str,"0x %-8X   SH3      ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_SH3E     :wsprintf(str,"0x %-8X   SH3E     ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_SH4      :wsprintf(str,"0x %-8X   SH4      ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_ARM      :wsprintf(str,"0x %-8X   ARM      ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_THUMB    :wsprintf(str,"0x %-8X   THUMB    ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_IA64     :wsprintf(str,"0x %-8X   IA64     ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_MIPS16   :wsprintf(str,"0x %-8X   MIPS16   ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_MIPSFPU  :wsprintf(str,"0x %-8X   MIPSFPU  ",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_MIPSFPU16:wsprintf(str,"0x %-8X   MIPSFPU16",peInfo.ntHeader.FileHeader.Machine);break;
		case IMAGE_FILE_MACHINE_ALPHA64  :wsprintf(str,"0x %-8X   ALPHA64  ",peInfo.ntHeader.FileHeader.Machine);break;
		default:wsprintf(str,"0x %-8X   ERROR   ",peInfo.ntHeader.FileHeader.Machine);
		}
		DisOneLine(hdc,rect,"Machine",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peInfo.ntHeader.FileHeader.NumberOfSections);
		DisOneLine(hdc,rect,"NumberOfSections",str,RIGHTSTEP+RIGHTSTEP);

		DisOneLine(hdc,rect,"TimeDateStamp",NULL,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peInfo.ntHeader.FileHeader.PointerToSymbolTable);
		DisOneLine(hdc,rect,"PointerToSymbolTable",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peInfo.ntHeader.FileHeader.NumberOfSymbols);
		DisOneLine(hdc,rect,"NumberOfSymbols",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peInfo.ntHeader.FileHeader.SizeOfOptionalHeader);
		DisOneLine(hdc,rect,"SizeOfOptionalHeader",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peInfo.ntHeader.FileHeader.Characteristics);
		DisOneLine(hdc,rect,"Characteristics",str,RIGHTSTEP+RIGHTSTEP);
		
		i=0;
		while(i<16)
		{
			if(peInfo.ntHeader.FileHeader.Characteristics & 1<<i)
			{		
				DisOneLine(hdc,rect,CharacterStr[i],NULL,RIGHTSTEP+RIGHTSTEP+RIGHTSTEP);
			}
			i++;
		}
		DisOneLine(hdc,rect,"File Header end",NULL,RIGHTSTEP);
		DisOneLine(hdc,rect,"Optional Header start",NULL,RIGHTSTEP);
		
		switch (peInfo.ntHeader.OptionalHeader.Magic)
		{
		case IMAGE_NT_OPTIONAL_HDR32_MAGIC:wsprintf(str,"PE32");break;
		case IMAGE_NT_OPTIONAL_HDR64_MAGIC:wsprintf(str,"PE64");break;
		case IMAGE_ROM_OPTIONAL_HDR_MAGIC :wsprintf(str,"ROM映像");break;
		default:
			wsprintf(str,"ERROR");
			break;
		}
		DisOneLine(hdc,rect,"Magic",str,RIGHTSTEP+RIGHTSTEP);
		DisOneLine(hdc,rect,"MajorLinkversion",NULL,RIGHTSTEP+RIGHTSTEP);
		DisOneLine(hdc,rect,"MinorLinkversion",NULL,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X   Base File",peInfo.ntHeader.OptionalHeader.SizeOfCode);
		DisOneLine(hdc,rect,"SizeOfCode",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.SizeOfInitializedData);
		DisOneLine(hdc,rect,"SizeOfInitializedData",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.SizeOfUninitializedData);
		DisOneLine(hdc,rect,"SizeOfUninitializedData",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X   (RVA)",peInfo.ntHeader.OptionalHeader.AddressOfEntryPoint);
		DisOneLine(hdc,rect,"AddressOfEntryPoint",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X   (RVA)",peInfo.ntHeader.OptionalHeader.BaseOfCode);
		DisOneLine(hdc,rect,"Code Section Start",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X   (RVA)",peInfo.ntHeader.OptionalHeader.BaseOfData);
		DisOneLine(hdc,rect,"Data Section Start",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X   (VA)",peInfo.ntHeader.OptionalHeader.ImageBase);
		DisOneLine(hdc,rect,"ImageBase",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.SectionAlignment);
		DisOneLine(hdc,rect,"SectionAlignment",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.FileAlignment);
		DisOneLine(hdc,rect,"FileAlignment",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.MajorOperatingSystemVersion);
		DisOneLine(hdc,rect,"MajorOSVersion",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.MinorOperatingSystemVersion);
		DisOneLine(hdc,rect,"MinorOSVersion",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.MajorImageVersion);
		DisOneLine(hdc,rect,"MajorImageVersion",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.MinorImageVersion);
		DisOneLine(hdc,rect,"MinorImageVersion",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.MajorSubsystemVersion);
		DisOneLine(hdc,rect,"MajorSubsystemVersion",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.MinorSubsystemVersion);
		DisOneLine(hdc,rect,"MinorSubsystemVersion",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X   Must be 0",peInfo.ntHeader.OptionalHeader.Win32VersionValue);
		DisOneLine(hdc,rect,"Win32VersionValue",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.SizeOfImage);
		DisOneLine(hdc,rect,"SizeOfImage",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.SizeOfHeaders);
		DisOneLine(hdc,rect,"SizeOfHeaders",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.CheckSum);
		DisOneLine(hdc,rect,"CheckSum",str,RIGHTSTEP+RIGHTSTEP);
		
		switch (peInfo.ntHeader.OptionalHeader.Subsystem)
		{
		case IMAGE_SUBSYSTEM_UNKNOWN       :wsprintf(str,"0x %-8X   未知          ",peInfo.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_NATIVE        :wsprintf(str,"0x %-8X   设备驱动程序和NativeWindows进程        ",peInfo.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_WINDOWS_GUI   :wsprintf(str,"0x %-8X   win图形界面   ",peInfo.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_WINDOWS_CUI   :wsprintf(str,"0x %-8X   win字符界面   ",peInfo.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_OS2_CUI       :wsprintf(str,"0x %-8X   OS2字符界面   ",peInfo.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_POSIX_CUI     :wsprintf(str,"0x %-8X   POSIX字符界面 ",peInfo.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_NATIVE_WINDOWS:wsprintf(str,"0x %-8X   NATIVE_WINDOWS",peInfo.ntHeader.OptionalHeader.Subsystem);break;
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:wsprintf(str,"0x %-8X   wince图形界面 ",peInfo.ntHeader.OptionalHeader.Subsystem);break;
		default:wsprintf(str,"0x %-8X   ERROR",peInfo.ntHeader.OptionalHeader.Subsystem);
		}
		DisOneLine(hdc,rect,"Subsystem",str,RIGHTSTEP+RIGHTSTEP);
		
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.DllCharacteristics);
		DisOneLine(hdc,rect,"DllCharacteristics",str,RIGHTSTEP+RIGHTSTEP);
		
		i=0;
		while(i<16)
		{
			if((peInfo.ntHeader.OptionalHeader.DllCharacteristics & (1<<i))!=0)
			{		
				DisOneLine(hdc,rect,DllCharacterStr[i],NULL,RIGHTSTEP+RIGHTSTEP+RIGHTSTEP);
			}
			i++;
		}

		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.SizeOfStackReserve);
		DisOneLine(hdc,rect,"SizeOfStackReserve",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.SizeOfStackCommit);
		DisOneLine(hdc,rect,"SizeOfStackCommit",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.SizeOfHeapReserve);
		DisOneLine(hdc,rect,"SizeOfHeapReserve",str,RIGHTSTEP+RIGHTSTEP);
		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.SizeOfHeapCommit);
		DisOneLine(hdc,rect,"SizeOfHeapCommit",str,RIGHTSTEP+RIGHTSTEP);

		wsprintf(str,"0x %-8X",peInfo.ntHeader.OptionalHeader.LoaderFlags);
		DisOneLine(hdc,rect,"LoaderFlags",str,RIGHTSTEP+RIGHTSTEP);

		rect->top+=4;
		DisOneLine(hdc,rect,"Data Directory will display on the other window",NULL,RIGHTSTEP+RIGHTSTEP);
		rect->top+=4;

		DisOneLine(hdc,rect,"Optional Header end",NULL,RIGHTSTEP);
		DisOneLine(hdc,rect,"NTHeader end",NULL,0);
		break;
	case XC_SECTION_INFO:
		DrawText(hdc,"节名称",-1,rect,DT_LEFT|DT_SINGLELINE);
		rect->left+=100;
		DrawText(hdc,"节RVA",-1,rect,DT_LEFT|DT_SINGLELINE);
		rect->left+=100;
		DrawText(hdc,"节FOA",-1,rect,DT_LEFT|DT_SINGLELINE);
		rect->left+=100;
		DrawText(hdc,"节大小(File)",-1,rect,DT_LEFT|DT_SINGLELINE);
		rect->left+=100;
		DrawText(hdc,"RWE",-1,rect,DT_LEFT|DT_SINGLELINE);
		rect->left+=100;
		DrawText(hdc,"节属性",-1,rect,DT_LEFT|DT_SINGLELINE);
		rect->left-=500;
		for(i=0;i<peInfo.ntHeader.FileHeader.NumberOfSections;i++)
		{
			rect->top+=XC_ONELINEHEIGHT;

			wsprintf(str,"%-10s",((PIMAGE_SECTION_HEADER)peInfo.pSections)[i].Name);
			DrawText(hdc,str,-1,rect,DT_LEFT|DT_SINGLELINE);
			rect->left+=100;
			
			wsprintf(str,"0x %-8X",((PIMAGE_SECTION_HEADER)peInfo.pSections)[i].VirtualAddress);
			DrawText(hdc,str,-1,rect,DT_LEFT|DT_SINGLELINE);
			rect->left+=100;
			
			
			wsprintf(str,"0x %-8X",((PIMAGE_SECTION_HEADER)peInfo.pSections)[i].PointerToRawData);
			DrawText(hdc,str,-1,rect,DT_LEFT|DT_SINGLELINE);
			rect->left+=100;
			
			wsprintf(str,"0x %-8X",((PIMAGE_SECTION_HEADER)peInfo.pSections)[i].SizeOfRawData);
			DrawText(hdc,str,-1,rect,DT_LEFT|DT_SINGLELINE);
			rect->left+=100;
			
			str[0]='\0';
			if((((PIMAGE_SECTION_HEADER)peInfo.pSections)[i].Characteristics & IMAGE_SCN_MEM_READ) == IMAGE_SCN_MEM_READ)
				wsprintf(&str[strlen(str)],"R");
			if((((PIMAGE_SECTION_HEADER)peInfo.pSections)[i].Characteristics & IMAGE_SCN_MEM_WRITE) == IMAGE_SCN_MEM_WRITE)
				wsprintf(&str[strlen(str)],"W");			
			if((((PIMAGE_SECTION_HEADER)peInfo.pSections)[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) == IMAGE_SCN_MEM_EXECUTE)
				wsprintf(&str[strlen(str)],"E");
			DrawText(hdc,str,-1,rect,DT_LEFT|DT_SINGLELINE);
			rect->left+=100;
			
			wsprintf(str,"0x %-8X",((PIMAGE_SECTION_HEADER)peInfo.pSections)[i].Characteristics);
			DrawText(hdc,str,-1,rect,DT_LEFT|DT_SINGLELINE);

			rect->left-=500;
		}
		rect->top+=XC_ONELINEHEIGHT;
		
		rect->top+=XC_ONELINEHEIGHT;
		DisOneLine(hdc,rect,"IMAGE_DATA_DIRECTORY Start",NULL,0);
		for (i=0;i<(int)(peInfo.ntHeader.OptionalHeader.NumberOfRvaAndSizes);i++)
		{
			rect->left+=RIGHTSTEP;
			DrawText(hdc,DataDirectoryName[i],-1,rect,DT_LEFT|DT_SINGLELINE);
			rect->left+=RIGHTSTEP*8;

			wsprintf(str,"RVA:0x %-8X",peInfo.ntHeader.OptionalHeader.DataDirectory[i].VirtualAddress);
			DrawText(hdc,str,-1,rect,DT_LEFT|DT_SINGLELINE);
			rect->left+=RIGHTSTEP*6;

			wsprintf(str,"FOA:0x %-8X",RvaToFoa(peInfo.pSections,peInfo.ntHeader.FileHeader.NumberOfSections,
				peInfo.ntHeader.OptionalHeader.DataDirectory[i].VirtualAddress));
			DrawText(hdc,str,-1,rect,DT_LEFT|DT_SINGLELINE);
			rect->left+=RIGHTSTEP*6;
			
			wsprintf(str,"SIZE:0x %-8X",peInfo.ntHeader.OptionalHeader.DataDirectory[i].Size);
			DrawText(hdc,str,-1,rect,DT_LEFT|DT_SINGLELINE);

			rect->left-=RIGHTSTEP*21;
			rect->top+=XC_ONELINEHEIGHT;
		}
		DisOneLine(hdc,rect,"IMAGE_DATA_DIRECTORY End",NULL,0);
		rect->top+=XC_ONELINEHEIGHT;

		rect->top+=XC_ONELINEHEIGHT/2;
		DisOneLine(hdc,rect,"IMAGE_SECTION_HEADER.Characteristics",NULL,0);
		DisOneLine(hdc,rect,"0x 00000020","节中包含代码",0);
		DisOneLine(hdc,rect,"0x 00000040","节中包含已初始化数据",0);
		DisOneLine(hdc,rect,"0x 00000080","节中包含未初始化数据",0);

		DisOneLine(hdc,rect,"0x 02000000","节中数据会在进程开始后丢弃",0);
		DisOneLine(hdc,rect,"0x 04000000","节中数据不会经过缓存",0);
		DisOneLine(hdc,rect,"0x 08000000","节中数据不会被交换到硬盘",0);

		DisOneLine(hdc,rect,"0x 10000000","节中数据将会被不同进程共享",0);
		DisOneLine(hdc,rect,"0x 20000000","节中数据可执行",0);
		DisOneLine(hdc,rect,"0x 40000000","节中数据可读",0);
		DisOneLine(hdc,rect,"0x 40000000","节中数据可写",0);
		break;
	case XC_IMPORT_DESCRIPTOR:
		ListImportDescriptorInfo(hdc,rect,peInfo);
		break;
	}
}

void ListImportDescriptorInfo(HDC hdc,PRECT rect,const PEINFO peInfo)
{
	if(peInfo.ntHeader.OptionalHeader.DataDirectory[1].Size==0)
	{
		DisOneLine(hdc,rect,"该文件没有导入表",NULL,0);
		return ;
	}

	HANDLE hFile;
	DWORD byteNum;
	IMAGE_IMPORT_BY_NAME	importByName;
	IMAGE_THUNK_DATA		thunkData;
	IMAGE_IMPORT_DESCRIPTOR importDesc;
	int importAddr;
	int temp,temp2;
	char tempstr[100];
	char tempstr2[100];

	int i;

	hFile=CreateFile((char*)peInfo.pFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	importAddr=RvaToFoa(peInfo.pSections,peInfo.ntHeader.FileHeader.NumberOfSections,
				peInfo.ntHeader.OptionalHeader.DataDirectory[1].VirtualAddress);

	i=0;
	while (1)
	{
		SetFilePointer(hFile,importAddr+i*sizeof(IMAGE_IMPORT_DESCRIPTOR),NULL,FILE_BEGIN);
		ReadFile(hFile,&importDesc,sizeof(IMAGE_IMPORT_DESCRIPTOR),&byteNum,NULL);

		if((int)importDesc.Name==0)
			break;

		temp=(int)importDesc.Name;
		temp=RvaToFoa(peInfo.pSections,peInfo.ntHeader.FileHeader.NumberOfSections,temp);
		SetFilePointer(hFile,temp,NULL,FILE_BEGIN);
		ReadFile(hFile,tempstr,99,&byteNum,NULL);
		
		DisOneLine(hdc,rect,"导入表：",tempstr,0);

		wsprintf(tempstr,"0x %-8X",importDesc.OriginalFirstThunk);
		DisOneLine(hdc,rect,"OriginalFirstThunk",tempstr,RIGHTSTEP);
		wsprintf(tempstr,"0x %-8X",importDesc.TimeDateStamp);
		DisOneLine(hdc,rect,"TimeDateStamp",tempstr,RIGHTSTEP);
		wsprintf(tempstr,"0x %-8X",importDesc.ForwarderChain);
		DisOneLine(hdc,rect,"ForwarderChain",tempstr,RIGHTSTEP);
		wsprintf(tempstr,"0x %-8X",importDesc.FirstThunk);
		DisOneLine(hdc,rect,"FirstThunk",tempstr,RIGHTSTEP);

		if((int)importDesc.OriginalFirstThunk!=0)
			temp=(int)importDesc.OriginalFirstThunk;
		else 
			temp=(int)importDesc.FirstThunk;
		temp=RvaToFoa(peInfo.pSections,peInfo.ntHeader.FileHeader.NumberOfSections,temp);
		SetFilePointer(hFile,temp,NULL,FILE_BEGIN);
		temp2=temp;
		while(1){
			ReadFile(hFile,&thunkData,sizeof(IMAGE_THUNK_DATA),&byteNum,NULL);

			if((int)thunkData.u1.AddressOfData==0)
				break;
			if(((int)thunkData.u1.AddressOfData & 0x80000000)==0 )
			{
				temp=RvaToFoa(peInfo.pSections,peInfo.ntHeader.FileHeader.NumberOfSections,
					(int)thunkData.u1.AddressOfData);
				SetFilePointer(hFile,temp,NULL,FILE_BEGIN);
				ReadFile(hFile,&importByName,sizeof(WORD),&byteNum,NULL);
				ReadFile(hFile,tempstr,50,&byteNum,NULL);
				wsprintf(tempstr2,"NAME: %s",tempstr);
				wsprintf(tempstr,"HINT: 0x %-8X",(int)importByName.Hint,tempstr);

				DisOneLine(hdc,rect,tempstr,tempstr2,RIGHTSTEP+RIGHTSTEP);
			}
			temp2+=sizeof(IMAGE_THUNK_DATA);
			SetFilePointer(hFile,temp2,NULL,FILE_BEGIN);
		}		
		i++;
	}
	DisOneLine(hdc,rect,"导入表结束",NULL,0);
	CloseHandle(hFile);
}


void DisOneLine(HDC hdc,PRECT rect,char *ID,char *Value,int direct)
{
	int left;

	left=rect->left;		//保存原值
	
	rect->left+=direct;		//修改rect.left
	
	if(rect->left<0)		//修正可能出现的错误
		rect->left=0;
	if(ID!=NULL)
	{
		DrawText(hdc,ID,-1,rect,DT_LEFT|DT_SINGLELINE);
	}
	if(Value!=NULL)
	{
		rect->left+=RIGHTSTEP*10;
		DrawText(hdc,Value,-1,rect,DT_LEFT|DT_SINGLELINE);
	}
	rect->top+=XC_ONELINEHEIGHT;
	
	rect->left=left;		//恢复原值
}

int RvaToFoa(LPVOID pSections,int sectionNum,int rva)
{
	if(pSections==NULL)
		return 0;
	
	int i;
	for(i=0;i<sectionNum;i++)
	{
		if(	rva>=(int)((PIMAGE_SECTION_HEADER)pSections)[i].VirtualAddress &&
		   (rva<=(int)((PIMAGE_SECTION_HEADER)pSections)[i].VirtualAddress +
				 (int)((PIMAGE_SECTION_HEADER)pSections)[i].SizeOfRawData))
		return rva -(int)((PIMAGE_SECTION_HEADER)pSections)[i].VirtualAddress +
					(int)((PIMAGE_SECTION_HEADER)pSections)[i].PointerToRawData;
	}
	return 0;
}
#include <windows.h>
#include <direct.h>
#include "PEBaseClass.h"
#include "StringTemplate.h"
#include "global.h"

PEBase::PEBase( TCHAR filePath[] )
{
	wsprintf(fFullPath,TEXT("%s"),filePath);
}

void PEBase::Release()
{
	if(pSection!=NULL)
		HeapFree(GetProcessHeap(),0,pSection);
}

BOOL PEBase::getDosNTSection( )
{
    pSection = NULL;

	HANDLE hFile;
	DWORD  byteNum;

	hFile=CreateFile(fFullPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if( hFile == INVALID_HANDLE_VALUE ){
		wsprintf(ErrorString,TEXT("打开文件失败"));
		return FALSE;
	}
	
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);
	ReadFile(hFile,&dosHeader,sizeof(IMAGE_DOS_HEADER),&byteNum,NULL);
	if(strncmp((char*)&dosHeader.e_magic,"MZ",2)!=0)
	{	
		CloseHandle(hFile);
		wsprintf(ErrorString,TEXT("MZ字段无法匹配"));
		return FALSE;
	}
	
    DWORD signature;
	SetFilePointer(hFile,dosHeader.e_lfanew,NULL,FILE_BEGIN);
    ReadFile(hFile,&signature,sizeof(DWORD),&byteNum,NULL);
    if(strncmp((char*)(&signature),"PE\0\0",4)!=0 )
	{	
		CloseHandle(hFile);
		wsprintf(ErrorString,TEXT("PE字段无法匹配"));
		return FALSE;
	}

    ReadFile(hFile,&fileHeader,sizeof(IMAGE_FILE_HEADER),&byteNum,NULL);
    if ( fileHeader.SizeOfOptionalHeader == 240 ){
        this->optionHeader = new PEOptionHeader64();
    }else{
        this->optionHeader = new PEOptionHeader32();
    }
    this->optionHeader->readData( hFile );

    pSection=HeapAlloc( GetProcessHeap(),HEAP_ZERO_MEMORY,fileHeader.NumberOfSections*sizeof(IMAGE_SECTION_HEADER)+2);

    SetFilePointer(hFile,dosHeader.e_lfanew + 
                            sizeof(signature) + 
                            sizeof(IMAGE_FILE_HEADER) + 
                            this->optionHeader->getStructSize(),
                            NULL,
                            FILE_BEGIN);
    ReadFile(hFile,pSection,fileHeader.NumberOfSections*sizeof(IMAGE_SECTION_HEADER),&byteNum,NULL);

	CloseHandle(hFile);
	return TRUE;
}



ULONGLONG PEBase::RvaToFoa( ULONGLONG rva )
{
	if(pSection==NULL)
	{
		wsprintf(ErrorString,TEXT("未获取节信息,无法进行地址转换"));
		return 0;
	}

	PIMAGE_SECTION_HEADER pTempSection = (PIMAGE_SECTION_HEADER)pSection;
	int i;
	for(i=0;i<fileHeader.NumberOfSections;i++)
	{
		if(	rva>=(ULONGLONG)(pTempSection[i].VirtualAddress) &&
			(rva<=(ULONGLONG)(pTempSection[i].VirtualAddress) +(ULONGLONG)(pTempSection[i].SizeOfRawData)))
			
			return rva -(ULONGLONG)(pTempSection[i].VirtualAddress) +(ULONGLONG)(pTempSection[i].PointerToRawData);
	}
	wsprintf(ErrorString,TEXT("rva地址不在已知节内,请核实"));
	
	return 0;
}


const TCHAR* PEBase::getPEFilePath()
{
    return this->fFullPath;
}

const char PEBase::CharacterStr[16][50]=
{
	"文件中不存在重定位信息",
	"文件是可执行的",
	"文件不存在行信息",
	"文件不存在符号信息",
	"调整工作集",
	"可处理2GB以上地址",
	"保留",
	"小尾方式",
	"只在32位平台上运行",
	"不包含调试信息",
	"不能从可移动盘运行",
	"不能从网络运行",
	"系统文件，不能直接运行",
	"DLL文件",
	"文件不能再多处理器计算机上运行",
	"大尾方式"
};

const char PEBase::DataDirectoryName[16][25]=
{
	"导出表(.edata)",
	"导入表(.idata)",
	"资源表(.rsrc)",
	"异常表(.pdata)",
	"安全表(文件偏移)",
	"重定位表(.reloc)",
	"调试表(.debug)",
	"版权表(必须为0)",
	"全局指针表",
	"线程本地存储表(.tls)",
	"加载配置表",
	"绑定导入表",
	"IAT表",
	"延迟导入表",
	"CLR表(.cormeta)",
	"预留类型"
};

const char PEBase::SectionCharacteristics_Base5[][50] = 
{
	"节中包含代码",
	"节中包含已初始化的数据",
	"节中包含未初始化的数据"
};
const char PEBase::SectionCharacteristics_Base25[][50] = 
{
	"节中的数据在进程开始后将被丢弃",
	"节中的数据不会经过缓存",
	"节中的数据不会被交换到磁盘",
	"节中的数据将会被不同进程所共享",
	"映射到内存后的页面包含可执行属性",
	"映射到内存后的页面包含可读属性",
	"映射到内存后的页面包含可写属性"
};


BOOL PEBase::writeToTempFile()
{
	CreateDirectory( TEXT("temp"),NULL );

	HANDLE tempFile;
	tempFile = CreateFile( this->GetTempFileName(),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL );
	if ( tempFile == INVALID_HANDLE_VALUE ){
		int a = GetLastError();
		wsprintf(ErrorString,TEXT("创建缓存文件失败"));
		return false;
	}

	writeStringToFile(tempFile,"var PEBaseData = [");

	writeDOSHeader(tempFile);                               writeSeparatorToFile(tempFile);
	writeNTFileHeader(tempFile);                            writeSeparatorToFile(tempFile);
    this->optionHeader->writeDataToFile(tempFile);          writeSeparatorToFile(tempFile);
	writeNTOptionalDataDirectory(tempFile);		

	writeStringToFile(tempFile,"];\n");

	writeStringToFile(tempFile,"var PESectionData = [");

	for ( int i = 0 ; i < fileHeader.NumberOfSections ; i ++ ){
		writeSectionInfo(tempFile,i);
		if ( i != fileHeader.NumberOfSections-1 )
			writeSeparatorToFile(tempFile);
	}

	writeStringToFile(tempFile,"];\n");

	CloseHandle(tempFile);
	return true;
}

void PEBase::writeDOSHeader( HANDLE hFile )
{
	char separator[2] = ",";

	char temp[1024];
	char retu[1024];

	writeStringToFile(hFile,"{ name:'IMAGE_DOS_HEADER',title:'DOS头',value:[");

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_magic" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_magic ),temp ) );
	st.exchange( "desc","EXE 标志,MZ" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_magic) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_cblp" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_cblp ),temp ) );
	st.exchange( "desc","最后（部分）页中的字节数" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_cblp) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_cp" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_cp ),temp ) );
	st.exchange( "desc","文件中的全部和部分页数" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_cp) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_crlc" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_crlc ),temp ) );
	st.exchange( "desc","重定位表中的指针数" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_crlc) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_cparhdr" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_cparhdr ),temp ) );
	st.exchange( "desc","头部尺寸，以段落为单位" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_cparhdr) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_minalloc" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_minalloc ),temp ) );
	st.exchange( "desc","所需的最小附加段" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_minalloc) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_maxalloc" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_maxalloc ),temp ) );
	st.exchange( "desc","所需的最大附加段" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_maxalloc) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_ss" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_ss ),temp ) );
	st.exchange( "desc","初始的SS值" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_ss) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_sp" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_sp),temp ) );
	st.exchange( "desc","初始的SP值" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_sp) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_csum" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_csum),temp ) );
	st.exchange( "desc","补码校验值" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_csum) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_ip" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_ip),temp ) );
	st.exchange( "desc","初始的IP值" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_ip) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_cs" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_cs),temp ) );
	st.exchange( "desc","初始的CS值" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_cs) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_lfarlc" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_lfarlc),temp ) );
	st.exchange( "desc","重定位表的字节偏移量" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_lfarlc) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_ovno" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_ovno),temp ) );
	st.exchange( "desc","覆盖号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_ovno) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_res" );
	wsprintfA( temp,"\"" );
	for ( int i = 0 ; i < sizeof(dosHeader.e_res) / sizeof(dosHeader.e_res[0])  ; i ++ ){
		wsprintfA( &temp[strlen(temp)],"%X",dosHeader.e_res[i] );
	}
	wsprintfA( &temp[strlen(temp)],"\"" );
	st.exchange( "value", temp );
	st.exchange( "desc","保留字" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_res) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_oemid" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_oemid),temp ) );
	st.exchange( "desc","OEM标示符" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_oemid) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_oeminfo" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_oeminfo),temp ) );
	st.exchange( "desc","OEM信息" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_oeminfo) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_res2" );
	wsprintfA( temp,"\"" );
	for ( int i = 0 ; i < sizeof(dosHeader.e_res2) / sizeof(dosHeader.e_res2[0])  ; i ++ ){
		wsprintfA( &temp[strlen(temp)],"%X",dosHeader.e_res2[i] );
	}
	wsprintfA( &temp[strlen(temp)],"\"" );
	st.exchange( "value", temp );
	st.exchange( "desc","保留字" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_res2) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_lfanew" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_lfanew),temp ) );
	st.exchange( "desc","PE头相对于文件开始处的偏移地址" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_lfanew) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);

	writeStringToFile(hFile,"]}");
}

void PEBase::writeNTFileHeader( HANDLE hFile )
{
	char separator[2] = ",";

	char temp[1024];
	char retu[1024];

	writeStringToFile(hFile,"{ name:'IMAGE_FILE_HEADER',title:'NT标准头',value:[");

	st.setBaseStr( StringTemplate::baseTemplate );

	switch ((int)fileHeader.Machine)
	{
	case IMAGE_FILE_MACHINE_UNKNOWN	:wsprintfA(temp,"运行平台：适用于任何处理器");break;
	case IMAGE_FILE_MACHINE_AM33				:wsprintfA(temp,"运行平台：Matsushita AM33 处理器");break;
	case IMAGE_FILE_MACHINE_AMD64			:wsprintfA(temp,"运行平台：x64 处理器");break;
	case IMAGE_FILE_MACHINE_ARM				:wsprintfA(temp,"运行平台：ARM 小尾处理器");break;
	case IMAGE_FILE_MACHINE_EBC					:wsprintfA(temp,"运行平台：EFI 字节码处理器");break;
	case IMAGE_FILE_MACHINE_I386				:wsprintfA(temp,"运行平台：Intel 386 处理器或后续兼容处理器");break;
	case IMAGE_FILE_MACHINE_IA64				:wsprintfA(temp,"运行平台：Intel Itanium 处理器系列");break;
	case IMAGE_FILE_MACHINE_M32R				:wsprintfA(temp,"运行平台：Mitsubishi M32R 小尾处理器");break;
	case IMAGE_FILE_MACHINE_MIPS16			:wsprintfA(temp,"运行平台：MIPS16 处理器");break;
	case IMAGE_FILE_MACHINE_MIPSFPU		:wsprintfA(temp,"运行平台：带 FPU 的 MIPS 处理器");break;
	case IMAGE_FILE_MACHINE_MIPSFPU16	:wsprintfA(temp,"运行平台：带 FPU 的 MIPS16 处理器");break;
	case IMAGE_FILE_MACHINE_POWERPC		:wsprintfA(temp,"运行平台：Power PC 小尾处理器");break;
	case IMAGE_FILE_MACHINE_POWERPCFP	:wsprintfA(temp,"运行平台：带浮点支持的 Power PC 处理器");break;
	case IMAGE_FILE_MACHINE_R4000				:wsprintfA(temp,"运行平台：MIPS 小尾处理器");break;
	case IMAGE_FILE_MACHINE_SH3				:wsprintfA(temp,"运行平台：Hitachi SH3 处理器");break;
	case IMAGE_FILE_MACHINE_SH3DSP			:wsprintfA(temp,"运行平台：Hitachi SH3 DSP 处理器");break;
	case IMAGE_FILE_MACHINE_SH4				:wsprintfA(temp,"运行平台：Hitachi SH4 处理器");break;
	case IMAGE_FILE_MACHINE_SH5				:wsprintfA(temp,"运行平台：Hitachi SH5 处理器");break;
	case IMAGE_FILE_MACHINE_THUMB			:wsprintfA(temp,"运行平台：ARM 或 Thumb 处理器");break;
	case IMAGE_FILE_MACHINE_WCEMIPSV2	:wsprintfA(temp,"运行平台：MIPS 小尾 WCE v2 处理器");break;
	default:wsprintfA(temp,"运行平台：未知");
	}
	st.exchange( "desc",temp );
	st.exchange( "name","Machine" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.Machine),temp ) );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.Machine) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfSections" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.NumberOfSections),temp ) );
	st.exchange( "desc","节的数量（IMAGE_SECTION_HEADER的个数）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.NumberOfSections) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","TimeDateStamp" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.TimeDateStamp),temp ) );
	st.exchange( "desc","文件创建日期和时间" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.TimeDateStamp) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","PointerToSymbolTable" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.PointerToSymbolTable),temp ) );
	st.exchange( "desc","指向符号表（用于调试，没有则为0）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.PointerToSymbolTable) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfSymbols" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.NumberOfSymbols),temp ) );
	st.exchange( "desc","符号表中的符号数量（用于调试，没有则为0）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.NumberOfSymbols) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfOptionalHeader" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.SizeOfOptionalHeader),temp ) );
	st.exchange( "desc","扩展头结构的长度（0x00E0 == 32位PE文件，0x00F0 == 64位PE文件）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.SizeOfOptionalHeader) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"文件属性" );
	bool sign = false;
    WORD a;
	for ( int i = 0 ; i < 16 ; i ++ ){	
         a = 1<<i;
		if ( ( a & fileHeader.Characteristics) == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"、%s",CharacterStr[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"：%s",CharacterStr[i] );
			sign = true;
		}
	}
	st.exchange( "desc",temp );
	st.exchange( "name","Characteristics" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.Characteristics),temp ) );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.Characteristics) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);

	writeStringToFile(hFile,"]}");
}


void PEBase::writeNTOptionalDataDirectory( HANDLE hFile )
{
    IMAGE_DATA_DIRECTORY dataDirectory;

	char separator[2] = ",";

	char temp[1024];
	char retu[1024];

	writeStringToFile(hFile,"{ name:'IMAGE_OPTIONAL_HEADER::DATA_DIRECTORY',title:'数据目录项',value:[");

    int length = this->optionHeader->getDataDirectoryNum();
    for ( int i = 0 ; i < length ; i ++ ){
        dataDirectory = optionHeader->getDataDirectory(i);
        st.setBaseStr("{name:'<<name>>',valueRVA:<<valueRVA>>,valueFOA:<<valueFOA>>,valueTableSize:<<valueTableSize>>,length:<<length>>,desc:'<<desc>>'}");
		
        st.exchange( "name", ( wsprintfA( temp,"%s",PEBase::DataDirectoryName[i]),temp ) );		
		st.exchange( "valueRVA", ( wsprintfA( temp,"%u",dataDirectory.VirtualAddress),temp )  );
		st.exchange( "valueFOA", ( wsprintfA( temp,"%u",RvaToFoa(dataDirectory.VirtualAddress) ),temp )  );
		st.exchange( "valueTableSize", ( wsprintfA( temp,"%u",dataDirectory.Size),temp )  );
		st.exchange( "desc",  ( wsprintfA( temp,"%s",PEBase::DataDirectoryName[i]),temp ) );
        st.exchange( "length",( wsprintfA( temp,"%u",sizeof(dataDirectory.VirtualAddress) ),temp ) );
		st.getString(retu);
		writeStringToFile(hFile,retu);

        if ( i < length - 1 )
            writeSeparatorToFile(hFile);
    }

	writeStringToFile(hFile,"]}");
}

void PEBase::writeSectionInfo( HANDLE hFile,int index )
{
	char separator[2] = ",";

	char temp[1024];
	char retu[1024];

	IMAGE_SECTION_HEADER section = ((PIMAGE_SECTION_HEADER)pSection)[index];

	wsprintfA( retu,"{ name:'IMAGE_SECTION_HEADER[%u]',title:'节表[%u]',value:[",index,index );
	writeStringToFile(hFile,retu);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Name" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.Name),temp ) );
	st.exchange( "desc", ( wsprintfA(temp,"8字节节名：%s",section.Name),temp ) );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.Name) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","VirtualSize" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.Misc.VirtualSize),temp ) );
	st.exchange( "desc","节的数据在没有对齐前的真实尺寸（但是很多PE文件中这个值都不准确）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.Misc.VirtualSize) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","VirtualAddress" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.VirtualAddress),temp ) );
	st.exchange( "desc","节区的RVA地址" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.VirtualAddress) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfRawData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.SizeOfRawData),temp ) );
	st.exchange( "desc","在文件中对齐后的尺寸" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.SizeOfRawData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","PointerToRawData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.PointerToRawData),temp ) );
	st.exchange( "desc","在文件中的偏移" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.PointerToRawData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","PointerToRelocations" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.PointerToRelocations),temp ) );
	st.exchange( "desc","在OBJ文件中使用" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.PointerToRelocations) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","PointerToLinenumbers" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.PointerToLinenumbers),temp ) );
	st.exchange( "desc","行号表的位置（供调试用）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.PointerToLinenumbers) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfRelocations" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.NumberOfRelocations),temp ) );
	st.exchange( "desc","在OBJ文件中使用" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.NumberOfRelocations) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfLinenumbers" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.NumberOfLinenumbers),temp ) );
	st.exchange( "desc","行号表中行号的数量" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.NumberOfLinenumbers) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"节的属性");
	bool sign = false;
	int i ;
	for ( i = 0 ; i < 3 ; i ++ )
	{
		if ( (i << (i+5)) && section.Characteristics == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"、%s",SectionCharacteristics_Base5[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"：%s",SectionCharacteristics_Base5[i] );
			sign = true;
		}
	}
	for ( i = 0 ; i < 7 ; i ++ )
	{
		if ( (i << (i+25)) && section.Characteristics == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"、%s",SectionCharacteristics_Base25[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"：%s",SectionCharacteristics_Base25[i] );
			sign = true;
		}
	}
	st.exchange( "desc",temp );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.Characteristics ),temp ) );
	st.exchange( "name","Characteristics" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.Characteristics) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);

	writeStringToFile(hFile,"]}");
}




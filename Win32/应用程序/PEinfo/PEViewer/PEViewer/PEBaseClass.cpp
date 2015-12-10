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
		wsprintf(ErrorString,TEXT("���ļ�ʧ��"));
		return FALSE;
	}
	
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);
	ReadFile(hFile,&dosHeader,sizeof(IMAGE_DOS_HEADER),&byteNum,NULL);
	if(strncmp((char*)&dosHeader.e_magic,"MZ",2)!=0)
	{	
		CloseHandle(hFile);
		wsprintf(ErrorString,TEXT("MZ�ֶ��޷�ƥ��"));
		return FALSE;
	}
	
    DWORD signature;
	SetFilePointer(hFile,dosHeader.e_lfanew,NULL,FILE_BEGIN);
    ReadFile(hFile,&signature,sizeof(DWORD),&byteNum,NULL);
    if(strncmp((char*)(&signature),"PE\0\0",4)!=0 )
	{	
		CloseHandle(hFile);
		wsprintf(ErrorString,TEXT("PE�ֶ��޷�ƥ��"));
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
		wsprintf(ErrorString,TEXT("δ��ȡ����Ϣ,�޷����е�ַת��"));
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
	wsprintf(ErrorString,TEXT("rva��ַ������֪����,���ʵ"));
	
	return 0;
}


const TCHAR* PEBase::getPEFilePath()
{
    return this->fFullPath;
}

const char PEBase::CharacterStr[16][50]=
{
	"�ļ��в������ض�λ��Ϣ",
	"�ļ��ǿ�ִ�е�",
	"�ļ�����������Ϣ",
	"�ļ������ڷ�����Ϣ",
	"����������",
	"�ɴ���2GB���ϵ�ַ",
	"����",
	"Сβ��ʽ",
	"ֻ��32λƽ̨������",
	"������������Ϣ",
	"���ܴӿ��ƶ�������",
	"���ܴ���������",
	"ϵͳ�ļ�������ֱ������",
	"DLL�ļ�",
	"�ļ������ٶദ���������������",
	"��β��ʽ"
};

const char PEBase::DataDirectoryName[16][25]=
{
	"������(.edata)",
	"�����(.idata)",
	"��Դ��(.rsrc)",
	"�쳣��(.pdata)",
	"��ȫ��(�ļ�ƫ��)",
	"�ض�λ��(.reloc)",
	"���Ա�(.debug)",
	"��Ȩ��(����Ϊ0)",
	"ȫ��ָ���",
	"�̱߳��ش洢��(.tls)",
	"�������ñ�",
	"�󶨵����",
	"IAT��",
	"�ӳٵ����",
	"CLR��(.cormeta)",
	"Ԥ������"
};

const char PEBase::SectionCharacteristics_Base5[][50] = 
{
	"���а�������",
	"���а����ѳ�ʼ��������",
	"���а���δ��ʼ��������"
};
const char PEBase::SectionCharacteristics_Base25[][50] = 
{
	"���е������ڽ��̿�ʼ�󽫱�����",
	"���е����ݲ��ᾭ������",
	"���е����ݲ��ᱻ����������",
	"���е����ݽ��ᱻ��ͬ����������",
	"ӳ�䵽�ڴ���ҳ�������ִ������",
	"ӳ�䵽�ڴ���ҳ������ɶ�����",
	"ӳ�䵽�ڴ���ҳ�������д����"
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
		wsprintf(ErrorString,TEXT("���������ļ�ʧ��"));
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

	writeStringToFile(hFile,"{ name:'IMAGE_DOS_HEADER',title:'DOSͷ',value:[");

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_magic" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_magic ),temp ) );
	st.exchange( "desc","EXE ��־,MZ" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_magic) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_cblp" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_cblp ),temp ) );
	st.exchange( "desc","��󣨲��֣�ҳ�е��ֽ���" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_cblp) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_cp" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_cp ),temp ) );
	st.exchange( "desc","�ļ��е�ȫ���Ͳ���ҳ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_cp) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_crlc" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_crlc ),temp ) );
	st.exchange( "desc","�ض�λ���е�ָ����" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_crlc) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_cparhdr" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_cparhdr ),temp ) );
	st.exchange( "desc","ͷ���ߴ磬�Զ���Ϊ��λ" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_cparhdr) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_minalloc" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_minalloc ),temp ) );
	st.exchange( "desc","�������С���Ӷ�" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_minalloc) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_maxalloc" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_maxalloc ),temp ) );
	st.exchange( "desc","�������󸽼Ӷ�" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_maxalloc) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_ss" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_ss ),temp ) );
	st.exchange( "desc","��ʼ��SSֵ" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_ss) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_sp" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_sp),temp ) );
	st.exchange( "desc","��ʼ��SPֵ" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_sp) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_csum" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_csum),temp ) );
	st.exchange( "desc","����У��ֵ" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_csum) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_ip" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_ip),temp ) );
	st.exchange( "desc","��ʼ��IPֵ" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_ip) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_cs" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_cs),temp ) );
	st.exchange( "desc","��ʼ��CSֵ" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_cs) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_lfarlc" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_lfarlc),temp ) );
	st.exchange( "desc","�ض�λ����ֽ�ƫ����" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_lfarlc) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_ovno" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_ovno),temp ) );
	st.exchange( "desc","���Ǻ�" );
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
	st.exchange( "desc","������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_res) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_oemid" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_oemid),temp ) );
	st.exchange( "desc","OEM��ʾ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_oemid) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_oeminfo" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_oeminfo),temp ) );
	st.exchange( "desc","OEM��Ϣ" );
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
	st.exchange( "desc","������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(this->dosHeader.e_res2) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","e_lfanew" );
	st.exchange( "value", ( wsprintfA( temp,"%u",this->dosHeader.e_lfanew),temp ) );
	st.exchange( "desc","PEͷ������ļ���ʼ����ƫ�Ƶ�ַ" );
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

	writeStringToFile(hFile,"{ name:'IMAGE_FILE_HEADER',title:'NT��׼ͷ',value:[");

	st.setBaseStr( StringTemplate::baseTemplate );

	switch ((int)fileHeader.Machine)
	{
	case IMAGE_FILE_MACHINE_UNKNOWN	:wsprintfA(temp,"����ƽ̨���������κδ�����");break;
	case IMAGE_FILE_MACHINE_AM33				:wsprintfA(temp,"����ƽ̨��Matsushita AM33 ������");break;
	case IMAGE_FILE_MACHINE_AMD64			:wsprintfA(temp,"����ƽ̨��x64 ������");break;
	case IMAGE_FILE_MACHINE_ARM				:wsprintfA(temp,"����ƽ̨��ARM Сβ������");break;
	case IMAGE_FILE_MACHINE_EBC					:wsprintfA(temp,"����ƽ̨��EFI �ֽ��봦����");break;
	case IMAGE_FILE_MACHINE_I386				:wsprintfA(temp,"����ƽ̨��Intel 386 ��������������ݴ�����");break;
	case IMAGE_FILE_MACHINE_IA64				:wsprintfA(temp,"����ƽ̨��Intel Itanium ������ϵ��");break;
	case IMAGE_FILE_MACHINE_M32R				:wsprintfA(temp,"����ƽ̨��Mitsubishi M32R Сβ������");break;
	case IMAGE_FILE_MACHINE_MIPS16			:wsprintfA(temp,"����ƽ̨��MIPS16 ������");break;
	case IMAGE_FILE_MACHINE_MIPSFPU		:wsprintfA(temp,"����ƽ̨���� FPU �� MIPS ������");break;
	case IMAGE_FILE_MACHINE_MIPSFPU16	:wsprintfA(temp,"����ƽ̨���� FPU �� MIPS16 ������");break;
	case IMAGE_FILE_MACHINE_POWERPC		:wsprintfA(temp,"����ƽ̨��Power PC Сβ������");break;
	case IMAGE_FILE_MACHINE_POWERPCFP	:wsprintfA(temp,"����ƽ̨��������֧�ֵ� Power PC ������");break;
	case IMAGE_FILE_MACHINE_R4000				:wsprintfA(temp,"����ƽ̨��MIPS Сβ������");break;
	case IMAGE_FILE_MACHINE_SH3				:wsprintfA(temp,"����ƽ̨��Hitachi SH3 ������");break;
	case IMAGE_FILE_MACHINE_SH3DSP			:wsprintfA(temp,"����ƽ̨��Hitachi SH3 DSP ������");break;
	case IMAGE_FILE_MACHINE_SH4				:wsprintfA(temp,"����ƽ̨��Hitachi SH4 ������");break;
	case IMAGE_FILE_MACHINE_SH5				:wsprintfA(temp,"����ƽ̨��Hitachi SH5 ������");break;
	case IMAGE_FILE_MACHINE_THUMB			:wsprintfA(temp,"����ƽ̨��ARM �� Thumb ������");break;
	case IMAGE_FILE_MACHINE_WCEMIPSV2	:wsprintfA(temp,"����ƽ̨��MIPS Сβ WCE v2 ������");break;
	default:wsprintfA(temp,"����ƽ̨��δ֪");
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
	st.exchange( "desc","�ڵ�������IMAGE_SECTION_HEADER�ĸ�����" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.NumberOfSections) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","TimeDateStamp" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.TimeDateStamp),temp ) );
	st.exchange( "desc","�ļ��������ں�ʱ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.TimeDateStamp) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","PointerToSymbolTable" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.PointerToSymbolTable),temp ) );
	st.exchange( "desc","ָ����ű����ڵ��ԣ�û����Ϊ0��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.PointerToSymbolTable) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfSymbols" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.NumberOfSymbols),temp ) );
	st.exchange( "desc","���ű��еķ������������ڵ��ԣ�û����Ϊ0��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.NumberOfSymbols) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfOptionalHeader" );
	st.exchange( "value", ( wsprintfA( temp,"%u",fileHeader.SizeOfOptionalHeader),temp ) );
	st.exchange( "desc","��չͷ�ṹ�ĳ��ȣ�0x00E0 == 32λPE�ļ���0x00F0 == 64λPE�ļ���" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(fileHeader.SizeOfOptionalHeader) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"�ļ�����" );
	bool sign = false;
    WORD a;
	for ( int i = 0 ; i < 16 ; i ++ ){	
         a = 1<<i;
		if ( ( a & fileHeader.Characteristics) == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"��%s",CharacterStr[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"��%s",CharacterStr[i] );
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

	writeStringToFile(hFile,"{ name:'IMAGE_OPTIONAL_HEADER::DATA_DIRECTORY',title:'����Ŀ¼��',value:[");

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

	wsprintfA( retu,"{ name:'IMAGE_SECTION_HEADER[%u]',title:'�ڱ�[%u]',value:[",index,index );
	writeStringToFile(hFile,retu);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Name" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.Name),temp ) );
	st.exchange( "desc", ( wsprintfA(temp,"8�ֽڽ�����%s",section.Name),temp ) );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.Name) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","VirtualSize" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.Misc.VirtualSize),temp ) );
	st.exchange( "desc","�ڵ�������û�ж���ǰ����ʵ�ߴ磨���Ǻܶ�PE�ļ������ֵ����׼ȷ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.Misc.VirtualSize) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","VirtualAddress" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.VirtualAddress),temp ) );
	st.exchange( "desc","������RVA��ַ" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.VirtualAddress) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfRawData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.SizeOfRawData),temp ) );
	st.exchange( "desc","���ļ��ж����ĳߴ�" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.SizeOfRawData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","PointerToRawData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.PointerToRawData),temp ) );
	st.exchange( "desc","���ļ��е�ƫ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.PointerToRawData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","PointerToRelocations" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.PointerToRelocations),temp ) );
	st.exchange( "desc","��OBJ�ļ���ʹ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.PointerToRelocations) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","PointerToLinenumbers" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.PointerToLinenumbers),temp ) );
	st.exchange( "desc","�кű��λ�ã��������ã�" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.PointerToLinenumbers) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfRelocations" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.NumberOfRelocations),temp ) );
	st.exchange( "desc","��OBJ�ļ���ʹ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.NumberOfRelocations) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfLinenumbers" );
	st.exchange( "value", ( wsprintfA( temp,"%u",section.NumberOfLinenumbers),temp ) );
	st.exchange( "desc","�кű����кŵ�����" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(section.NumberOfLinenumbers) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"�ڵ�����");
	bool sign = false;
	int i ;
	for ( i = 0 ; i < 3 ; i ++ )
	{
		if ( (i << (i+5)) && section.Characteristics == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"��%s",SectionCharacteristics_Base5[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"��%s",SectionCharacteristics_Base5[i] );
			sign = true;
		}
	}
	for ( i = 0 ; i < 7 ; i ++ )
	{
		if ( (i << (i+25)) && section.Characteristics == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"��%s",SectionCharacteristics_Base25[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"��%s",SectionCharacteristics_Base25[i] );
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




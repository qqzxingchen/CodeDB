#include "PEOptionHeader.h"
#include "stringTemplate.h"
#include "global.h"


const char PEOptionHeader::DllCharacterStr[16][35]=
{
	"",
	"",
	"",
	"",
	"",
	"",				//0-5λǿ��Ϊ0
	"DLL�����ڼ���ʱ���ض�λ",
	"ǿ�ƴ���ʵʩ�����Լ��",
	"��ӳ�����DEP",
	"���Ը��룬���������ӳ��",
	"ӳ��ʹ��SEH",
	"����ӳ��",
	"",
	"��ӳ��ΪWDM driver",
	"",
	"�������ն˷�����"
};



void PEOptionHeader32::writeDataToFile(  HANDLE tempFile  )
{
    StringTemplate st;

    HANDLE hFile = tempFile;

    char separator[2] = ",";

	char temp[1024];
	char retu[1024];

	writeStringToFile(hFile,"{ name:'IMAGE_OPTIONAL_HEADER::Base',title:'NT��չͷ',value:[");

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Magic" );
    st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.Magic),temp ) );
	st.exchange( "desc","ħ���֣�˵���ļ�������(107H == ROM ����10BH == PE32��20BH == PE64)" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Magic) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorLinkerVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorLinkerVersion),temp ) );
	st.exchange( "desc","���������汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorLinkerVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorLinkerVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorLinkerVersion),temp ) );
	st.exchange( "desc","���������汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorLinkerVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfCode" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfCode),temp ) );
	st.exchange( "desc","���к�����Ľڵ��ܴ�С�������ļ����룩" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfCode) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfInitializedData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfInitializedData),temp ) );
	st.exchange( "desc","���к��ѳ�ʼ�����ݵĽڵ��ܴ�С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfInitializedData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfUninitializedData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfUninitializedData),temp ) );
	st.exchange( "desc","���к�δ��ʼ�����ݵĽڵ��ܴ�С�����ļ��в�ռ�ÿռ䣬�����ڱ����ص��ڴ�֮��PE���س��򽫻�Ϊ��Щ���ݷ����ʵ��Ŀռ䣩" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfUninitializedData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","AddressOfEntryPoint" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.AddressOfEntryPoint),temp ) );
	st.exchange( "desc","����ִ�����RVA����¼��������������PE���غ����ʼλ�õ��׶��ٸ��ֽڡ����ڳ���ӳ������������ַ�������豸�����������ǳ�ʼ�������ĵ�ַ������DLL��˵�����ǿ�ѡ�ģ������������Ҫ����Ϊ0��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.AddressOfEntryPoint) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","BaseOfCode" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.BaseOfCode),temp ) );
	st.exchange( "desc","����Ľڵ���ʼRVA����ʾӳ�񱻼��ص��ڴ��к󣬴���ڵĿ�ͷ�����ӳ���ַ��ƫ�Ƶ�ַ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.BaseOfCode) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","BaseOfData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.BaseOfData),temp ) );
	st.exchange( "desc","���ݵĽڵ���ʼRVA����ʾӳ�񱻼��ص��ڴ��к����ݽڵĿ�ͷ�����ӳ���ַ��ƫ�Ƶ�ַ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.BaseOfData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","ImageBase" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.ImageBase),temp ) );
	st.exchange( "desc","PEӳ�������װ���ַ��EXE�ļ����ǿ��������λ��װ�룻�������һ�����������˶��DLL�ļ�����DLLʵ��װ���ַ���ֵ���ܾͻ᲻ͬ����ֵ������64K����������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.ImageBase) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SectionAlignment" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SectionAlignment),temp ) );
	st.exchange( "desc","�ڴ��еĽڵĶ�������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SectionAlignment) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","FileAlignment" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.FileAlignment),temp ) );
	st.exchange( "desc","�ļ��еĽڵĶ�������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.FileAlignment) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorOperatingSystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorOperatingSystemVersion),temp ) );
	st.exchange( "desc","����ϵͳ���汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorOperatingSystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorOperatingSystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorOperatingSystemVersion),temp ) );
	st.exchange( "desc","����ϵͳ���汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorOperatingSystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorImageVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorImageVersion),temp ) );
	st.exchange( "desc","��PE���汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorImageVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorImageVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorImageVersion),temp ) );
	st.exchange( "desc","��PE���汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorImageVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorSubsystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorSubsystemVersion),temp ) );
	st.exchange( "desc","������ϵͳ�����汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorSubsystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorSubsystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorSubsystemVersion),temp ) );
	st.exchange( "desc","������ϵͳ�ĸ��汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorSubsystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Win32VersionValue" );
	st.exchange( "value", "\'\'" );
	st.exchange( "desc","��ϵͳ�汾��ֵ��δ�ã���������Ϊ0��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Win32VersionValue) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfImage" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfImage),temp ) );
	st.exchange( "desc","�ڴ�������PE��ӳ��ߴ磨���Ա�ʵ�ʵ�ֵ�󣬵����ܱ���С���ұ��뱣֤����SectionAlignment����������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfImage) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeaders" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfHeaders),temp ) );
	st.exchange( "desc","����ͷ+�ڱ����ļ��������ȶ����Ĵ�С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeaders) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","CheckSum" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.CheckSum),temp ) );
	st.exchange( "desc","У��ͣ������PE�ļ��У���ֵ��0������һЩ�ں�ģʽ�����������ϵͳDLL�У��������������Ч��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.CheckSum) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"ָ��ʹ�ý������ϵͳ��" );
	switch( optionHeader.Subsystem ){
	case IMAGE_SUBSYSTEM_UNKNOWN            : wsprintfA( &temp[strlen(temp)],"δ֪����ϵͳ" ); break;
	case IMAGE_SUBSYSTEM_NATIVE             : wsprintfA( &temp[strlen(temp)],"�豸��������� Native Windows ����" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_GUI        : wsprintfA( &temp[strlen(temp)],"Windows ͼ���û�����" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_CUI        : wsprintfA( &temp[strlen(temp)],"Windows �ַ�ģʽ" ); break;
	case IMAGE_SUBSYSTEM_POSIX_CUI          : wsprintfA( &temp[strlen(temp)],"POSIX�ַ�ģʽ" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI     : wsprintfA( &temp[strlen(temp)],"Windows CE ͼ�ν���" ); break;
	case IMAGE_SUBSYSTEM_EFI_APPLICATION    : wsprintfA( &temp[strlen(temp)],"����չ�̼��ӿڣ�EFI��Ӧ�ó���" ); break;
	case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER    : wsprintfA( &temp[strlen(temp)],"�����������EFI��������" ); break;
	case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER : wsprintfA( &temp[strlen(temp)],"������ʱ�����EFI��������" ); break;
	case IMAGE_SUBSYSTEM_EFI_ROM            : wsprintfA( &temp[strlen(temp)],"EFI ROM ����" ); break;
	case IMAGE_SUBSYSTEM_XBOX               : wsprintfA( &temp[strlen(temp)],"XBOX" ); break;
	}
	st.exchange( "desc",temp );
	st.exchange( "name","Subsystem" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.Subsystem),temp ) );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Subsystem) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"DLL�ļ����ԣ��������DLL�ļ��ģ������������PE�ļ��ģ�" );
	bool sign = false;
	for ( int i = 0 ; i < 16 ; i ++ ){	
		if ( ( ( 1 << i) & optionHeader.DllCharacteristics) == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"��%s",DllCharacterStr[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"��%s",DllCharacterStr[i] );
			sign = true;
		}
	}
	st.exchange( "desc",temp );
	st.exchange( "name","DllCharacteristics" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.DllCharacteristics),temp ) );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.DllCharacteristics) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfStackReserve" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfStackReserve),temp ) );
	st.exchange( "desc","��ʼ����ջ��С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfStackReserve) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfStackCommit" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfStackCommit),temp ) );
	st.exchange( "desc","��ʼ��ʱʵ���ύ��ջ��С��ջ��С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfStackCommit) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeapReserve" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfHeapReserve),temp ) );
	st.exchange( "desc","��ʼ��ʱ�����ĶѴ�С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeapReserve) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeapCommit" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfHeapCommit),temp ) );
	st.exchange( "desc","��ʼ��ʱʵ���ύ�ĶѴ�С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeapCommit) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","LoaderFlags" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.LoaderFlags),temp ) );
	st.exchange( "desc","���ر�־" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.LoaderFlags) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfRvaAndSizes" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.NumberOfRvaAndSizes),temp ) );
	st.exchange( "desc","����Ŀ¼�ṹ����Ŀ����" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.NumberOfRvaAndSizes) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);

	writeStringToFile(hFile,"]}");
}
void PEOptionHeader64::writeDataToFile( HANDLE tempFile )
{
    StringTemplate st;

    HANDLE hFile = tempFile;

    char separator[2] = ",";

	char temp[1024];
	char retu[1024];

	writeStringToFile(hFile,"{ name:'IMAGE_OPTIONAL_HEADER::Base',title:'NT��չͷ',value:[");

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Magic" );
    st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.Magic),temp ) );
	st.exchange( "desc","ħ���֣�˵���ļ�������(107H == ROM ����10BH == PE32��20BH == PE64)" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Magic) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorLinkerVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorLinkerVersion),temp ) );
	st.exchange( "desc","���������汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorLinkerVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorLinkerVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorLinkerVersion),temp ) );
	st.exchange( "desc","���������汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorLinkerVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfCode" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfCode),temp ) );
	st.exchange( "desc","���к�����Ľڵ��ܴ�С�������ļ����룩" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfCode) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfInitializedData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfInitializedData),temp ) );
	st.exchange( "desc","���к��ѳ�ʼ�����ݵĽڵ��ܴ�С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfInitializedData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfUninitializedData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfUninitializedData),temp ) );
	st.exchange( "desc","���к�δ��ʼ�����ݵĽڵ��ܴ�С�����ļ��в�ռ�ÿռ䣬�����ڱ����ص��ڴ�֮��PE���س��򽫻�Ϊ��Щ���ݷ����ʵ��Ŀռ䣩" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfUninitializedData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","AddressOfEntryPoint" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.AddressOfEntryPoint),temp ) );
	st.exchange( "desc","����ִ�����RVA����¼��������������PE���غ����ʼλ�õ��׶��ٸ��ֽڡ����ڳ���ӳ������������ַ�������豸�����������ǳ�ʼ�������ĵ�ַ������DLL��˵�����ǿ�ѡ�ģ������������Ҫ����Ϊ0��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.AddressOfEntryPoint) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","BaseOfCode" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.BaseOfCode),temp ) );
	st.exchange( "desc","����Ľڵ���ʼRVA����ʾӳ�񱻼��ص��ڴ��к󣬴���ڵĿ�ͷ�����ӳ���ַ��ƫ�Ƶ�ַ��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.BaseOfCode) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","ImageBase" );
	st.exchange( "value", ( wsprintfA( temp,"%I64u",optionHeader.ImageBase),temp ) );
	st.exchange( "desc","PEӳ�������װ���ַ��EXE�ļ����ǿ��������λ��װ�룻�������һ�����������˶��DLL�ļ�����DLLʵ��װ���ַ���ֵ���ܾͻ᲻ͬ����ֵ������64K����������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.ImageBase) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SectionAlignment" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SectionAlignment),temp ) );
	st.exchange( "desc","�ڴ��еĽڵĶ�������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SectionAlignment) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","FileAlignment" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.FileAlignment),temp ) );
	st.exchange( "desc","�ļ��еĽڵĶ�������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.FileAlignment) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorOperatingSystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorOperatingSystemVersion),temp ) );
	st.exchange( "desc","����ϵͳ���汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorOperatingSystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorOperatingSystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorOperatingSystemVersion),temp ) );
	st.exchange( "desc","����ϵͳ���汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorOperatingSystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorImageVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorImageVersion),temp ) );
	st.exchange( "desc","��PE���汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorImageVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorImageVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorImageVersion),temp ) );
	st.exchange( "desc","��PE���汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorImageVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorSubsystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorSubsystemVersion),temp ) );
	st.exchange( "desc","������ϵͳ�����汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorSubsystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorSubsystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorSubsystemVersion),temp ) );
	st.exchange( "desc","������ϵͳ�ĸ��汾��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorSubsystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Win32VersionValue" );
	st.exchange( "value", "\'\'" );
	st.exchange( "desc","��ϵͳ�汾��ֵ��δ�ã���������Ϊ0��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Win32VersionValue) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfImage" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfImage),temp ) );
	st.exchange( "desc","�ڴ�������PE��ӳ��ߴ磨���Ա�ʵ�ʵ�ֵ�󣬵����ܱ���С���ұ��뱣֤����SectionAlignment����������" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfImage) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeaders" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfHeaders),temp ) );
	st.exchange( "desc","����ͷ+�ڱ����ļ��������ȶ����Ĵ�С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeaders) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","CheckSum" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.CheckSum),temp ) );
	st.exchange( "desc","У��ͣ������PE�ļ��У���ֵ��0������һЩ�ں�ģʽ�����������ϵͳDLL�У��������������Ч��" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.CheckSum) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"ָ��ʹ�ý������ϵͳ��" );
	switch( optionHeader.Subsystem ){
	case IMAGE_SUBSYSTEM_UNKNOWN            : wsprintfA( &temp[strlen(temp)],"δ֪����ϵͳ" ); break;
	case IMAGE_SUBSYSTEM_NATIVE             : wsprintfA( &temp[strlen(temp)],"�豸��������� Native Windows ����" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_GUI        : wsprintfA( &temp[strlen(temp)],"Windows ͼ���û�����" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_CUI        : wsprintfA( &temp[strlen(temp)],"Windows �ַ�ģʽ" ); break;
	case IMAGE_SUBSYSTEM_POSIX_CUI          : wsprintfA( &temp[strlen(temp)],"POSIX�ַ�ģʽ" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI     : wsprintfA( &temp[strlen(temp)],"Windows CE ͼ�ν���" ); break;
	case IMAGE_SUBSYSTEM_EFI_APPLICATION    : wsprintfA( &temp[strlen(temp)],"����չ�̼��ӿڣ�EFI��Ӧ�ó���" ); break;
	case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER    : wsprintfA( &temp[strlen(temp)],"�����������EFI��������" ); break;
	case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER : wsprintfA( &temp[strlen(temp)],"������ʱ�����EFI��������" ); break;
	case IMAGE_SUBSYSTEM_EFI_ROM            : wsprintfA( &temp[strlen(temp)],"EFI ROM ����" ); break;
	case IMAGE_SUBSYSTEM_XBOX               : wsprintfA( &temp[strlen(temp)],"XBOX" ); break;
	}
	st.exchange( "desc",temp );
	st.exchange( "name","Subsystem" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.Subsystem),temp ) );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Subsystem) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"DLL�ļ����ԣ��������DLL�ļ��ģ������������PE�ļ��ģ�" );
	bool sign = false;
	for ( int i = 0 ; i < 16 ; i ++ ){	
		if ( ( ( 1 << i) & optionHeader.DllCharacteristics) == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"��%s",DllCharacterStr[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"��%s",DllCharacterStr[i] );
			sign = true;
		}
	}
	st.exchange( "desc",temp );
	st.exchange( "name","DllCharacteristics" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.DllCharacteristics),temp ) );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.DllCharacteristics) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfStackReserve" );
	st.exchange( "value", ( wsprintfA( temp,"%I64u",optionHeader.SizeOfStackReserve),temp ) );
	st.exchange( "desc","��ʼ����ջ��С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfStackReserve) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfStackCommit" );
	st.exchange( "value", ( wsprintfA( temp,"%I64u",optionHeader.SizeOfStackCommit),temp ) );
	st.exchange( "desc","��ʼ��ʱʵ���ύ��ջ��С��ջ��С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfStackCommit) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeapReserve" );
	st.exchange( "value", ( wsprintfA( temp,"%I64u",optionHeader.SizeOfHeapReserve),temp ) );
	st.exchange( "desc","��ʼ��ʱ�����ĶѴ�С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeapReserve) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeapCommit" );
	st.exchange( "value", ( wsprintfA( temp,"%I64u",optionHeader.SizeOfHeapCommit),temp ) );
	st.exchange( "desc","��ʼ��ʱʵ���ύ�ĶѴ�С" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeapCommit) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","LoaderFlags" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.LoaderFlags),temp ) );
	st.exchange( "desc","���ر�־" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.LoaderFlags) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfRvaAndSizes" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.NumberOfRvaAndSizes),temp ) );
	st.exchange( "desc","����Ŀ¼�ṹ����Ŀ����" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.NumberOfRvaAndSizes) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);

	writeStringToFile(hFile,"]}");
}

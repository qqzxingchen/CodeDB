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
	"",				//0-5位强制为0
	"DLL可以在加载时被重定位",
	"强制代码实施完整性检查",
	"该映像兼容DEP",
	"可以隔离，但不隔离此映像",
	"映像不使用SEH",
	"不绑定映像",
	"",
	"该映像为WDM driver",
	"",
	"可用于终端服务器"
};



void PEOptionHeader32::writeDataToFile(  HANDLE tempFile  )
{
    StringTemplate st;

    HANDLE hFile = tempFile;

    char separator[2] = ",";

	char temp[1024];
	char retu[1024];

	writeStringToFile(hFile,"{ name:'IMAGE_OPTIONAL_HEADER::Base',title:'NT扩展头',value:[");

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Magic" );
    st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.Magic),temp ) );
	st.exchange( "desc","魔术字，说明文件的类型(107H == ROM 镜像；10BH == PE32；20BH == PE64)" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Magic) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorLinkerVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorLinkerVersion),temp ) );
	st.exchange( "desc","链接器主版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorLinkerVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorLinkerVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorLinkerVersion),temp ) );
	st.exchange( "desc","链接器副版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorLinkerVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfCode" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfCode),temp ) );
	st.exchange( "desc","所有含代码的节的总大小（基于文件对齐）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfCode) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfInitializedData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfInitializedData),temp ) );
	st.exchange( "desc","所有含已初始化数据的节的总大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfInitializedData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfUninitializedData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfUninitializedData),temp ) );
	st.exchange( "desc","所有含未初始化数据的节的总大小（在文件中不占用空间，但是在被加载到内存之后，PE加载程序将会为这些数据分配适当的空间）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfUninitializedData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","AddressOfEntryPoint" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.AddressOfEntryPoint),temp ) );
	st.exchange( "desc","程序执行入口RVA（记录了启动代码距离该PE加载后的起始位置到底多少个字节。对于程序映像，它是启动地址；对于设备驱动程序，它是初始化函数的地址；对于DLL来说，它是可选的，如果不存在需要设置为0）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.AddressOfEntryPoint) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","BaseOfCode" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.BaseOfCode),temp ) );
	st.exchange( "desc","代码的节的起始RVA（表示映像被加载到内存中后，代码节的开头相对于映像基址的偏移地址）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.BaseOfCode) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","BaseOfData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.BaseOfData),temp ) );
	st.exchange( "desc","数据的节的起始RVA（表示映像被加载到内存中后，数据节的开头相对于映像基址的偏移地址）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.BaseOfData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","ImageBase" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.ImageBase),temp ) );
	st.exchange( "desc","PE映像的优先装入地址（EXE文件总是可以以这个位置装入；但是如果一个程序引入了多个DLL文件，则DLL实际装入地址与该值可能就会不同；该值必须是64K的整数倍）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.ImageBase) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SectionAlignment" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SectionAlignment),temp ) );
	st.exchange( "desc","内存中的节的对齐粒度" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SectionAlignment) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","FileAlignment" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.FileAlignment),temp ) );
	st.exchange( "desc","文件中的节的对齐粒度" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.FileAlignment) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorOperatingSystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorOperatingSystemVersion),temp ) );
	st.exchange( "desc","操作系统主版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorOperatingSystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorOperatingSystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorOperatingSystemVersion),temp ) );
	st.exchange( "desc","操作系统副版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorOperatingSystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorImageVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorImageVersion),temp ) );
	st.exchange( "desc","该PE主版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorImageVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorImageVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorImageVersion),temp ) );
	st.exchange( "desc","该PE副版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorImageVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorSubsystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorSubsystemVersion),temp ) );
	st.exchange( "desc","所需子系统的主版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorSubsystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorSubsystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorSubsystemVersion),temp ) );
	st.exchange( "desc","所需子系统的副版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorSubsystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Win32VersionValue" );
	st.exchange( "value", "\'\'" );
	st.exchange( "desc","子系统版本的值（未用，必须设置为0）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Win32VersionValue) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfImage" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfImage),temp ) );
	st.exchange( "desc","内存中整个PE的映像尺寸（可以比实际的值大，但不能比它小，且必须保证它是SectionAlignment的整数倍）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfImage) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeaders" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfHeaders),temp ) );
	st.exchange( "desc","所有头+节表按照文件对齐粒度对齐后的大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeaders) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","CheckSum" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.CheckSum),temp ) );
	st.exchange( "desc","校验和（大多数PE文件中，该值是0；但在一些内核模式的驱动程序和系统DLL中，它必须存在且有效）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.CheckSum) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"指定使用界面的子系统：" );
	switch( optionHeader.Subsystem ){
	case IMAGE_SUBSYSTEM_UNKNOWN            : wsprintfA( &temp[strlen(temp)],"未知的子系统" ); break;
	case IMAGE_SUBSYSTEM_NATIVE             : wsprintfA( &temp[strlen(temp)],"设备驱动程序和 Native Windows 进程" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_GUI        : wsprintfA( &temp[strlen(temp)],"Windows 图形用户界面" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_CUI        : wsprintfA( &temp[strlen(temp)],"Windows 字符模式" ); break;
	case IMAGE_SUBSYSTEM_POSIX_CUI          : wsprintfA( &temp[strlen(temp)],"POSIX字符模式" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI     : wsprintfA( &temp[strlen(temp)],"Windows CE 图形界面" ); break;
	case IMAGE_SUBSYSTEM_EFI_APPLICATION    : wsprintfA( &temp[strlen(temp)],"可扩展固件接口（EFI）应用程序" ); break;
	case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER    : wsprintfA( &temp[strlen(temp)],"带引导服务的EFI驱动程序" ); break;
	case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER : wsprintfA( &temp[strlen(temp)],"带运行时服务的EFI驱动程序" ); break;
	case IMAGE_SUBSYSTEM_EFI_ROM            : wsprintfA( &temp[strlen(temp)],"EFI ROM 镜像" ); break;
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
	wsprintfA( temp,"DLL文件特性（不是针对DLL文件的，而是针对所有PE文件的）" );
	bool sign = false;
	for ( int i = 0 ; i < 16 ; i ++ ){	
		if ( ( ( 1 << i) & optionHeader.DllCharacteristics) == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"、%s",DllCharacterStr[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"：%s",DllCharacterStr[i] );
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
	st.exchange( "desc","初始化的栈大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfStackReserve) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfStackCommit" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfStackCommit),temp ) );
	st.exchange( "desc","初始化时实际提交的栈大小的栈大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfStackCommit) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeapReserve" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfHeapReserve),temp ) );
	st.exchange( "desc","初始化时保留的堆大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeapReserve) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeapCommit" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfHeapCommit),temp ) );
	st.exchange( "desc","初始化时实际提交的堆大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeapCommit) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","LoaderFlags" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.LoaderFlags),temp ) );
	st.exchange( "desc","加载标志" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.LoaderFlags) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfRvaAndSizes" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.NumberOfRvaAndSizes),temp ) );
	st.exchange( "desc","数据目录结构的项目数量" );
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

	writeStringToFile(hFile,"{ name:'IMAGE_OPTIONAL_HEADER::Base',title:'NT扩展头',value:[");

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Magic" );
    st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.Magic),temp ) );
	st.exchange( "desc","魔术字，说明文件的类型(107H == ROM 镜像；10BH == PE32；20BH == PE64)" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Magic) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorLinkerVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorLinkerVersion),temp ) );
	st.exchange( "desc","链接器主版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorLinkerVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorLinkerVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorLinkerVersion),temp ) );
	st.exchange( "desc","链接器副版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorLinkerVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfCode" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfCode),temp ) );
	st.exchange( "desc","所有含代码的节的总大小（基于文件对齐）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfCode) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfInitializedData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfInitializedData),temp ) );
	st.exchange( "desc","所有含已初始化数据的节的总大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfInitializedData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfUninitializedData" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfUninitializedData),temp ) );
	st.exchange( "desc","所有含未初始化数据的节的总大小（在文件中不占用空间，但是在被加载到内存之后，PE加载程序将会为这些数据分配适当的空间）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfUninitializedData) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","AddressOfEntryPoint" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.AddressOfEntryPoint),temp ) );
	st.exchange( "desc","程序执行入口RVA（记录了启动代码距离该PE加载后的起始位置到底多少个字节。对于程序映像，它是启动地址；对于设备驱动程序，它是初始化函数的地址；对于DLL来说，它是可选的，如果不存在需要设置为0）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.AddressOfEntryPoint) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","BaseOfCode" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.BaseOfCode),temp ) );
	st.exchange( "desc","代码的节的起始RVA（表示映像被加载到内存中后，代码节的开头相对于映像基址的偏移地址）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.BaseOfCode) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","ImageBase" );
	st.exchange( "value", ( wsprintfA( temp,"%I64u",optionHeader.ImageBase),temp ) );
	st.exchange( "desc","PE映像的优先装入地址（EXE文件总是可以以这个位置装入；但是如果一个程序引入了多个DLL文件，则DLL实际装入地址与该值可能就会不同；该值必须是64K的整数倍）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.ImageBase) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SectionAlignment" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SectionAlignment),temp ) );
	st.exchange( "desc","内存中的节的对齐粒度" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SectionAlignment) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","FileAlignment" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.FileAlignment),temp ) );
	st.exchange( "desc","文件中的节的对齐粒度" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.FileAlignment) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorOperatingSystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorOperatingSystemVersion),temp ) );
	st.exchange( "desc","操作系统主版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorOperatingSystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorOperatingSystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorOperatingSystemVersion),temp ) );
	st.exchange( "desc","操作系统副版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorOperatingSystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorImageVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorImageVersion),temp ) );
	st.exchange( "desc","该PE主版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorImageVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorImageVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorImageVersion),temp ) );
	st.exchange( "desc","该PE副版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorImageVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MajorSubsystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MajorSubsystemVersion),temp ) );
	st.exchange( "desc","所需子系统的主版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MajorSubsystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","MinorSubsystemVersion" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.MinorSubsystemVersion),temp ) );
	st.exchange( "desc","所需子系统的副版本号" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.MinorSubsystemVersion) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","Win32VersionValue" );
	st.exchange( "value", "\'\'" );
	st.exchange( "desc","子系统版本的值（未用，必须设置为0）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.Win32VersionValue) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfImage" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfImage),temp ) );
	st.exchange( "desc","内存中整个PE的映像尺寸（可以比实际的值大，但不能比它小，且必须保证它是SectionAlignment的整数倍）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfImage) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeaders" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.SizeOfHeaders),temp ) );
	st.exchange( "desc","所有头+节表按照文件对齐粒度对齐后的大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeaders) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","CheckSum" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.CheckSum),temp ) );
	st.exchange( "desc","校验和（大多数PE文件中，该值是0；但在一些内核模式的驱动程序和系统DLL中，它必须存在且有效）" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.CheckSum) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	wsprintfA( temp,"指定使用界面的子系统：" );
	switch( optionHeader.Subsystem ){
	case IMAGE_SUBSYSTEM_UNKNOWN            : wsprintfA( &temp[strlen(temp)],"未知的子系统" ); break;
	case IMAGE_SUBSYSTEM_NATIVE             : wsprintfA( &temp[strlen(temp)],"设备驱动程序和 Native Windows 进程" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_GUI        : wsprintfA( &temp[strlen(temp)],"Windows 图形用户界面" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_CUI        : wsprintfA( &temp[strlen(temp)],"Windows 字符模式" ); break;
	case IMAGE_SUBSYSTEM_POSIX_CUI          : wsprintfA( &temp[strlen(temp)],"POSIX字符模式" ); break;
	case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI     : wsprintfA( &temp[strlen(temp)],"Windows CE 图形界面" ); break;
	case IMAGE_SUBSYSTEM_EFI_APPLICATION    : wsprintfA( &temp[strlen(temp)],"可扩展固件接口（EFI）应用程序" ); break;
	case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER    : wsprintfA( &temp[strlen(temp)],"带引导服务的EFI驱动程序" ); break;
	case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER : wsprintfA( &temp[strlen(temp)],"带运行时服务的EFI驱动程序" ); break;
	case IMAGE_SUBSYSTEM_EFI_ROM            : wsprintfA( &temp[strlen(temp)],"EFI ROM 镜像" ); break;
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
	wsprintfA( temp,"DLL文件特性（不是针对DLL文件的，而是针对所有PE文件的）" );
	bool sign = false;
	for ( int i = 0 ; i < 16 ; i ++ ){	
		if ( ( ( 1 << i) & optionHeader.DllCharacteristics) == 0 )
			continue;
		if ( sign ){
			wsprintfA( &temp[strlen(temp)],"、%s",DllCharacterStr[i] );
		}else{
			wsprintfA( &temp[strlen(temp)],"：%s",DllCharacterStr[i] );
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
	st.exchange( "desc","初始化的栈大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfStackReserve) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfStackCommit" );
	st.exchange( "value", ( wsprintfA( temp,"%I64u",optionHeader.SizeOfStackCommit),temp ) );
	st.exchange( "desc","初始化时实际提交的栈大小的栈大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfStackCommit) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeapReserve" );
	st.exchange( "value", ( wsprintfA( temp,"%I64u",optionHeader.SizeOfHeapReserve),temp ) );
	st.exchange( "desc","初始化时保留的堆大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeapReserve) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","SizeOfHeapCommit" );
	st.exchange( "value", ( wsprintfA( temp,"%I64u",optionHeader.SizeOfHeapCommit),temp ) );
	st.exchange( "desc","初始化时实际提交的堆大小" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.SizeOfHeapCommit) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","LoaderFlags" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.LoaderFlags),temp ) );
	st.exchange( "desc","加载标志" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.LoaderFlags) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);
	writeSeparatorToFile(hFile);

	st.setBaseStr( StringTemplate::baseTemplate );
	st.exchange( "name","NumberOfRvaAndSizes" );
	st.exchange( "value", ( wsprintfA( temp,"%u",optionHeader.NumberOfRvaAndSizes),temp ) );
	st.exchange( "desc","数据目录结构的项目数量" );
	st.exchange( "length",( wsprintfA( temp,"%u",sizeof(optionHeader.NumberOfRvaAndSizes) ),temp ) );
	st.getString(retu);
	writeStringToFile(hFile,retu);

	writeStringToFile(hFile,"]}");
}

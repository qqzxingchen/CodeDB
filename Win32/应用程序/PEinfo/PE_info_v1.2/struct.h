#ifndef include_struct
#define include_struct 


#include <windows.h>

#define XC_IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16
#define XC_IMAGE_SIZEOF_SHORT_NAME 8

typedef struct XC_IMAGE_DOS_HEADER {    // DOS .EXE header
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    LONG   e_lfanew;                    // File address of new exe header
} XC_IMAGE_DOS_HEADER ;

typedef struct XC_IMAGE_DATA_DIRECTORY {
    DWORD   VirtualAddress;
    DWORD   Size;
} XC_IMAGE_DATA_DIRECTORY;

typedef struct XC_IMAGE_FILE_HEADER {
    WORD    Machine;
    WORD    NumberOfSections;
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;
} XC_IMAGE_FILE_HEADER ;

typedef struct XC_IMAGE_OPTIONAL_HEADER {
    WORD    Magic;
    BYTE    MajorLinkerVersion;
    BYTE    MinorLinkerVersion;
    DWORD   SizeOfCode;
    DWORD   SizeOfInitializedData;
    DWORD   SizeOfUninitializedData;
    DWORD   AddressOfEntryPoint;
    DWORD   BaseOfCode;
    DWORD   BaseOfData;
    DWORD   ImageBase;
    DWORD   SectionAlignment;
    DWORD   FileAlignment;
    WORD    MajorOperatingSystemVersion;
    WORD    MinorOperatingSystemVersion;
    WORD    MajorImageVersion;
    WORD    MinorImageVersion;
    WORD    MajorSubsystemVersion;
    WORD    MinorSubsystemVersion;
    DWORD   Win32VersionValue;
    DWORD   SizeOfImage;
    DWORD   SizeOfHeaders;
    DWORD   CheckSum;
    WORD    Subsystem;
    WORD    DllCharacteristics;
    DWORD   SizeOfStackReserve;
    DWORD   SizeOfStackCommit;
    DWORD   SizeOfHeapReserve;
    DWORD   SizeOfHeapCommit;
    DWORD   LoaderFlags;
    DWORD   NumberOfRvaAndSizes;
    XC_IMAGE_DATA_DIRECTORY DataDirectory[XC_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} XC_IMAGE_OPTIONAL_HEADER32;

typedef struct XC_IMAGE_NT_HEADERS {
    DWORD Signature;
    XC_IMAGE_FILE_HEADER FileHeader;
    XC_IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} XC_IMAGE_NT_HEADERS32;

typedef struct XC_IMAGE_SECTION_HEADER {
    BYTE    Name[XC_IMAGE_SIZEOF_SHORT_NAME];
    union {
		DWORD   PhysicalAddress;
		DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;
} XC_IMAGE_SECTION_HEADER;

typedef struct XC_IMAGE_IMPORT_DESCRIPTOR {
    union {
        DWORD   Characteristics;
        DWORD   OriginalFirstThunk;
    };
    DWORD   TimeDateStamp;               
    DWORD   ForwarderChain;                
    DWORD   Name;
    DWORD   FirstThunk;
} XC_IMAGE_IMPORT_DESCRIPTOR;

typedef struct XC_IMAGE_IMPORT_BY_NAME {
    WORD    Hint;
    char    Name[50];
} XC_IMAGE_IMPORT_BY_NAME;

typedef struct XC_IMAGE_THUNK_DATA {
    union {
        DWORD ForwarderString;
        DWORD Function;
        DWORD Ordinal;
        DWORD AddressOfData;
    }u1;
} XC_IMAGE_THUNK_DATA;

typedef struct XC_IMAGE_EXPORT_DIRECTORY {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   Name;
    DWORD   Base;
    DWORD   NumberOfFunctions;
    DWORD   NumberOfNames;
    DWORD   AddressOfFunctions;     // RVA from base of image
    DWORD   AddressOfNames;         // RVA from base of image
    DWORD   AddressOfNameOrdinals;  // RVA from base of image
} XC_IMAGE_EXPORT_DIRECTORY;


typedef struct XC_IMAGE_BASE_RELOCATION {
    DWORD   VirtualAddress;
    DWORD   SizeOfBlock;
} XC_IMAGE_BASE_RELOCATION;

typedef struct XC_IMAGE_DELAY_IMPORT_DESCRIPTOR{
	DWORD Attributes;
	DWORD Name;
	DWORD ModuleHandle;
	DWORD DelayIAT;
	DWORD DelayINT;
	DWORD BoundDelayIT;
	DWORD UnLoadDelayIT;
	DWORD TimeStamp;
}XC_IMAGE_DELAY_IMPORT_DESCRIPTOR;


typedef struct XC_IMAGE_RESOURCE_DIRECTORY {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    WORD    NumberOfNamedEntries;
    WORD    NumberOfIdEntries;
} XC_IMAGE_RESOURCE_DIRECTORY;


typedef struct XC_IMAGE_RESOURCE_DIRECTORY_ENTRY {
    union {
        struct {
            DWORD NameOffset:31;
            DWORD NameIsString:1;
        }bit1;
        DWORD   Name;
        WORD    Id;
    };
    union {
        DWORD   OffsetToData;
        struct {
            DWORD   OffsetToDirectory:31;
            DWORD   DataIsDirectory:1;
        }bit2;
    };
} XC_IMAGE_RESOURCE_DIRECTORY_ENTRY;


typedef struct XC_IMAGE_RESOURCE_DATA_ENTRY {
    DWORD   OffsetToData;
    DWORD   Size;
    DWORD   CodePage;
    DWORD   Reserved;
} XC_IMAGE_RESOURCE_DATA_ENTRY;


typedef struct XC_MenuHeader {
	WORD wVersion;			//版本号，暂时为0
	WORD cbHeaderSize;		//头大小，暂时未0
}XC_MenuHeader;

typedef struct XC_ICO_DIR_ENTRY{
	BYTE  bWidth;			//宽度
	BYTE  bHeight;			//高度
	BYTE  bColorCount;		//颜色数
	BYTE  bReserved;		//保留字，必须为0
	WORD  wPlane;			//调色板
	WORD  wBitCount;		//每个像素的位数
	DWORD dwBytesInRes;		//资源长度
	WORD  dwImageOffset;	//资源在文件中的位移
							//标准的ICO文件，这里是一个DWORD类型
							//但是PE存储的时候，这里是一个WORD(sizeof的时候，为了对齐，这个结构体会被算为16字节)
							//PE存储的时候，这个值为一个索引，就是图标二级目录的ID号。
							//也即XC_RESOURCE_TYPE_AND_OFFSET.ID_2
}XC_ICO_DIR_ENTRY;


typedef struct XC_ICO_DIR {
	WORD idreserved;		//保留字，必须为0
	WORD idtype;			//资源类别。若为1，则表示为ICO文件
	WORD idcount;			//图标数量
}XC_ICO_DIR;

typedef struct XC_RESOURCE_TYPE_AND_OFFSET{
	int ID_1;				//资源的种类.参照P197预定义的种类
	int ID_2;				//资源的ID,在程序的有唯一标识
	int offset;				//资源在FOA
	int size;				//资源的大小
}XC_RESOURCE_TYPE_AND_OFFSET;


typedef struct XC_IMAGE_TLS_DIRECTORY {
    DWORD   StartAddressOfRawData;
    DWORD   EndAddressOfRawData;
    DWORD	AddressOfIndex;
    DWORD	AddressOfCallBacks;
    DWORD   SizeOfZeroFill;
    DWORD   Characteristics;
} XC_IMAGE_TLS_DIRECTORY;

typedef struct XC_IMAGE_POINTER_TO_TLS_CALLBACK{
	PVOID	DllHandle;		//DLL的句柄
	DWORD	Reason;			//调用该回调函数的时机
	PVOID	Reserved;		//预留，为0
}XC_IMAGE_POINTER_TO_TLS_CALLBACK;

// typedef struct XC_IMAGE_LOAD_CONFIG_DIRECTORY {
//     DWORD   Characteristics;
//     DWORD   TimeDateStamp;
//     WORD    MajorVersion;
//     WORD    MinorVersion;
//     DWORD   GlobalFlagsClear;
//     DWORD   GlobalFlagsSet;
//     DWORD   CriticalSectionDefaultTimeout;
//     DWORD   DeCommitFreeBlockThreshold;
//     DWORD   DeCommitTotalFreeThreshold;
//     DWORD   LockPrefixTable;
//     DWORD   MaximumAllocationSize;
//     DWORD   VirtualMemoryThreshold;
//     DWORD   ProcessAffinityMask;
//     DWORD   ProcessHeapFlags;
//     WORD    CSDVersion;
//     WORD    Reserved1;
//     DWORD   EditList;
//     DWORD   SecurityCookie;
// 	DWORD	SEHandlerTable;
// 	DWORD	SEHandlerCount;
// } XC_IMAGE_LOAD_CONFIG_DIRECTORY;

typedef struct XC_BITMAPINFOHEADER {
	DWORD	biSize;
	LONG	biWidth;
	LONG	biHeight; 
	WORD	biPlanes;        
	WORD	biBitCount;      
	DWORD	biCompression;   
	DWORD	biSizeImage;     
	LONG	biXPelsPerMeter;  
	LONG	biYPelsPerMeter; 
	DWORD	biClrUsed; 
	DWORD	biClrImportant;
}XC_BITMAPINFOHEADER;





#endif
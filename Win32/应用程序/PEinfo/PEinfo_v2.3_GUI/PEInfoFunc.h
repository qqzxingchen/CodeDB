#ifndef PEINFOFUNC__H 
#define PEINFOFUNC__H

#include <winuser.h>

///////////////////////////////////////////////////
//message
#define WM_MOUSEWHEEL	0x020A			//因为系统版本过低，所以需要在这里手动定义

#define	XC_SB_MOUSEWHEEL WM_USER+9		//lParam为一个int表示向上或者向下的行数
#define XC_OPENWINDOW   WM_USER+10		//wParam存储将要创建子窗口的标题
#define XC_FILENAME		WM_USER+11		//当文件名符合要求时，对文件内PE要求的MZ字段和PE字段进行审核
#define XC_INITBITMAP	WM_USER+12		//wParam表示窗口的ID号，即sign数组中的值;即初始化绘图板

/////////////////////////////////////////////////
//用来区分子窗口
#define	CHILDWINDOWNUM			3
#define XC_NT_INFO				0
#define XC_SECTION_INFO			1
#define XC_IMPORT_DESCRIPTOR	2

static int		sign[CHILDWINDOWNUM]={XC_NT_INFO,XC_SECTION_INFO,XC_IMPORT_DESCRIPTOR};
static TCHAR	title[CHILDWINDOWNUM][40]={	
					TEXT("NT Header Info"),
					TEXT("Section And Data Directory Info"),
					TEXT("Import Descriptor Info")};


//////////////////////////////////////////////////
//constant
#define IDM_FIRSTCHILD	50000
#define XC_SCROLL_PAGE	100

static int pageNum[CHILDWINDOWNUM]={3,2,20};

//////////////////////////////////////////////////
//display constant
#define XC_ONELINEHEIGHT		20
#define RIGHTSTEP				20

/////////////////////////////////////////////////
static char CharacterStr[][35]={
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
	"系统文件，不能直接运行"
	"DLL文件",
	"文件不能再多处理器计算机上运行",
	"大尾方式"
};
static char DllCharacterStr[][35]={
	"","","","","","",				//0-5位强制为0
	"DLL可以在加载时被重定位",
	"强制代码实施完整性检查",
	"该映像兼容DEP",
	"可以隔离，但不隔离此映像",
	"映像不使用SEH",
	"不绑定映像","",
	"该映像为WDM driver","",
	"可用于终端服务器"
};
static char DataDirectoryName[16][25]={
	".edata:  导出表RVA",
	".idata:  导入表RVA",
	".pdata:  资源表RVA",
	".rsrc:   异常表RVA",
	"(文件偏移):安全表",
	".reloc:  重定位表RVA",
	".debug:  调试表RVA",
	"(必须为0):版权表RVA",
	"全局指针表RVA",
	".tls:    线程本地存储RVA",
	"加载配置表RVA",
	"绑定导入表RVA",
	"IAT表RVA",
	"延迟导入表RVA",
	".cormeta:CLR表RVA",
	"预留类型RVA"
};




//////////////////////////////////////////////////
//pe struct
typedef struct tagPEINFO 
{
	LPVOID			 pFileName;
	HWND			 hWnd[CHILDWINDOWNUM];
	POINT			 windowSize[CHILDWINDOWNUM];	//用来存储每个子窗口的当前窗口大小
	SCROLLINFO		 scrollInfo[CHILDWINDOWNUM];	//用来存储每个子窗口的滚动条状态
	IMAGE_DOS_HEADER dosHeader;
	IMAGE_NT_HEADERS ntHeader;
	LPVOID			 pSections;						//用来保存从堆中申请的内存指针
}PEINFO,*PPEINFO;



///////////////////////////////////////////////
//function
LRESULT CALLBACK FrameWndProc	(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL	CALLBACK CloseEnumProc	(HWND hWnd,LPARAM lParam);
LRESULT	CALLBACK InfoWndProc	(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);






//************************************
// Method:    ListImportDescriptorInfo
// FullName:  ListImportDescriptorInfo
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: HDC hdc
// Parameter: PRECT rect
// Parameter: const PEINFO peInfo	
// 其中文件名可以通过peInfo中的(char*)pFileName得到
//************************************
void ListImportDescriptorInfo(HDC hdc,PRECT rect,const PEINFO peInfo);


//************************************
// Method:    DisplayInfo
// FullName:  DisplayInfo
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: HDC hdc
// Parameter: PRECT rect	显示的区域
// Parameter: int disId		hdc所属的子窗口 可以根据这个判定该使用哪种数据结构进行解析
// Parameter: const PEINFO peInfo	只包含子窗口句柄数组 DOSHEADER NTHEADER结构体
// 为普通子窗口增加一行
//************************************
void DisplayInfo(HDC hdc,PRECT rect,int disId,const PEINFO peInfo);	

//************************************
// Method:    DisplayInfo
// FullName:  DisplayInfo
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: HDC hdc		
// Parameter: PRECT rect	显示的区域，默认为该区域的左上角，并且单行显示
// Parameter: char *ID		表示将要增加的这一行的属性名
// Parameter: char *VALUE	表示将要增加的这一行的值及描述
// Parameter: int direct	rect的缩进,<0表示向左,0表示不动,>0表示向右.但是不会改变rect中left的值
// 但是rect的top一定会被增加一行
//************************************
void DisOneLine(HDC hdc,PRECT rect,char *ID,char *Value,int direct);


//************************************
// Method:    FoaToRva
// FullName:  FoaToRva
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: LPVOID pSections	//这里是把peInfo的pSections传过来
// Parameter: int section		//节的数量
// Parameter: int rva
// 该函数可以通过peInfo中的section信息将rva转换成foa
//************************************
int RvaToFoa(LPVOID pSections,int sectionNum,int rva);





#endif


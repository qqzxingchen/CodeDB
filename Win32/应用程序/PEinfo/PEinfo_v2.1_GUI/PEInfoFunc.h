#ifndef PEINFOFUNC__H 
#define PEINFOFUNC__H

#include <winuser.h>

#define XC_NT_INFO				0
#define XC_SECTION_INFO			1
#define	CHILDWINDOWNUM			2

#define XC_ONELINEHEIGHT		20

#define RIGHTSTEP				20

typedef struct tagPEINFO 
{
	HWND			 hWnd[CHILDWINDOWNUM];
	POINT			 windowSize[CHILDWINDOWNUM];	//用来存储每个子窗口的当前窗口大小
	SCROLLINFO		 scrollInfo[CHILDWINDOWNUM];	//用来存储每个子窗口的滚动条状态
	IMAGE_DOS_HEADER dosHeader;
	IMAGE_NT_HEADERS ntHeader;
	LPVOID			 pSections;				//用来保存从堆中申请的内存指针
}PEINFO,*PPEINFO;

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


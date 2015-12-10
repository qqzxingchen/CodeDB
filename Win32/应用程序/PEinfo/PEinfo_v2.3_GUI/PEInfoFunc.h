#ifndef PEINFOFUNC__H 
#define PEINFOFUNC__H

#include <winuser.h>

///////////////////////////////////////////////////
//message
#define WM_MOUSEWHEEL	0x020A			//��Ϊϵͳ�汾���ͣ�������Ҫ�������ֶ�����

#define	XC_SB_MOUSEWHEEL WM_USER+9		//lParamΪһ��int��ʾ���ϻ������µ�����
#define XC_OPENWINDOW   WM_USER+10		//wParam�洢��Ҫ�����Ӵ��ڵı���
#define XC_FILENAME		WM_USER+11		//���ļ�������Ҫ��ʱ�����ļ���PEҪ���MZ�ֶκ�PE�ֶν������
#define XC_INITBITMAP	WM_USER+12		//wParam��ʾ���ڵ�ID�ţ���sign�����е�ֵ;����ʼ����ͼ��

/////////////////////////////////////////////////
//���������Ӵ���
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
	"ϵͳ�ļ�������ֱ������"
	"DLL�ļ�",
	"�ļ������ٶദ���������������",
	"��β��ʽ"
};
static char DllCharacterStr[][35]={
	"","","","","","",				//0-5λǿ��Ϊ0
	"DLL�����ڼ���ʱ���ض�λ",
	"ǿ�ƴ���ʵʩ�����Լ��",
	"��ӳ�����DEP",
	"���Ը��룬���������ӳ��",
	"ӳ��ʹ��SEH",
	"����ӳ��","",
	"��ӳ��ΪWDM driver","",
	"�������ն˷�����"
};
static char DataDirectoryName[16][25]={
	".edata:  ������RVA",
	".idata:  �����RVA",
	".pdata:  ��Դ��RVA",
	".rsrc:   �쳣��RVA",
	"(�ļ�ƫ��):��ȫ��",
	".reloc:  �ض�λ��RVA",
	".debug:  ���Ա�RVA",
	"(����Ϊ0):��Ȩ��RVA",
	"ȫ��ָ���RVA",
	".tls:    �̱߳��ش洢RVA",
	"�������ñ�RVA",
	"�󶨵����RVA",
	"IAT��RVA",
	"�ӳٵ����RVA",
	".cormeta:CLR��RVA",
	"Ԥ������RVA"
};




//////////////////////////////////////////////////
//pe struct
typedef struct tagPEINFO 
{
	LPVOID			 pFileName;
	HWND			 hWnd[CHILDWINDOWNUM];
	POINT			 windowSize[CHILDWINDOWNUM];	//�����洢ÿ���Ӵ��ڵĵ�ǰ���ڴ�С
	SCROLLINFO		 scrollInfo[CHILDWINDOWNUM];	//�����洢ÿ���Ӵ��ڵĹ�����״̬
	IMAGE_DOS_HEADER dosHeader;
	IMAGE_NT_HEADERS ntHeader;
	LPVOID			 pSections;						//��������Ӷ���������ڴ�ָ��
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
// �����ļ�������ͨ��peInfo�е�(char*)pFileName�õ�
//************************************
void ListImportDescriptorInfo(HDC hdc,PRECT rect,const PEINFO peInfo);


//************************************
// Method:    DisplayInfo
// FullName:  DisplayInfo
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: HDC hdc
// Parameter: PRECT rect	��ʾ������
// Parameter: int disId		hdc�������Ӵ��� ���Ը�������ж���ʹ���������ݽṹ���н���
// Parameter: const PEINFO peInfo	ֻ�����Ӵ��ھ������ DOSHEADER NTHEADER�ṹ��
// Ϊ��ͨ�Ӵ�������һ��
//************************************
void DisplayInfo(HDC hdc,PRECT rect,int disId,const PEINFO peInfo);	

//************************************
// Method:    DisplayInfo
// FullName:  DisplayInfo
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: HDC hdc		
// Parameter: PRECT rect	��ʾ������Ĭ��Ϊ����������Ͻǣ����ҵ�����ʾ
// Parameter: char *ID		��ʾ��Ҫ���ӵ���һ�е�������
// Parameter: char *VALUE	��ʾ��Ҫ���ӵ���һ�е�ֵ������
// Parameter: int direct	rect������,<0��ʾ����,0��ʾ����,>0��ʾ����.���ǲ���ı�rect��left��ֵ
// ����rect��topһ���ᱻ����һ��
//************************************
void DisOneLine(HDC hdc,PRECT rect,char *ID,char *Value,int direct);


//************************************
// Method:    FoaToRva
// FullName:  FoaToRva
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: LPVOID pSections	//�����ǰ�peInfo��pSections������
// Parameter: int section		//�ڵ�����
// Parameter: int rva
// �ú�������ͨ��peInfo�е�section��Ϣ��rvaת����foa
//************************************
int RvaToFoa(LPVOID pSections,int sectionNum,int rva);





#endif


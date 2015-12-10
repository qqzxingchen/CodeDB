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
	POINT			 windowSize[CHILDWINDOWNUM];	//�����洢ÿ���Ӵ��ڵĵ�ǰ���ڴ�С
	SCROLLINFO		 scrollInfo[CHILDWINDOWNUM];	//�����洢ÿ���Ӵ��ڵĹ�����״̬
	IMAGE_DOS_HEADER dosHeader;
	IMAGE_NT_HEADERS ntHeader;
	LPVOID			 pSections;				//��������Ӷ���������ڴ�ָ��
}PEINFO,*PPEINFO;

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


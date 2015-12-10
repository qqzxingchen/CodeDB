#ifndef PEBASECLASS__H
#define PEBASECLASS__H

#include <windows.h>
#include <winnt.h>

//�������PE�ļ���ǰ�漸����Сȷ���Ľṹ���һЩ��Ϣ������DOSheader��ntheader��section
class PEBase	
{
public:
	static const char CharacterStr[16][50];
	static const char DllCharacterStr[][35];
	static const char DataDirectoryName[16][25];


private:
	char				fFullPath[256];		//PE�ļ���ȫ·��
public:
	IMAGE_DOS_HEADER	dosHeader;			//
	IMAGE_NT_HEADERS	ntHeader;
	LPVOID				pSection;
	char				ErrorString[100];	//��BOOL�ͺ�����������ʱ�����Զ�ȡ����ַ�����֪����ԭ��

private:
	
	//************************************
	// Method:    checkExtendName
	// FullName:  PEBase::checkExtendName
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: char * extendName
	// �˲��ļ��ĺ�׺���Ƿ����Ҫ��
	//************************************
	BOOL checkExtendName(char * extendName);

	//************************************
	// Method:    getDosNTSection
	// FullName:  PEBase::getDosNTSection
	// Access:    private 
	// Parameter: char * filePath		�ļ���
	// Returns:   BOOL
	// ��ȡ�ļ���dosheader��NTheader�ṹ���Section��Ϣ�����˲��Ƿ����PE�ļ���ʽҪ��
	//************************************
	BOOL getDosNTSection(char *filePath);	

public:

	//************************************
	// Method:    Init
	// FullName:  PEBase::Init
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: char * filePath		�ļ���
	// Parameter: char * extendName		�ļ���׺��
	//ͨ��������·��������ļ��Ƿ�Ϊexe����dll����sys��׺���Ƿ����PE�ļ���ʽ�Ļ���Ҫ��		
	//************************************
	BOOL Init(char * filePath,char * extendName);

	//************************************
	// Method:    RvaToFoa
	// FullName:  PESection::RvaToFoa
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: int rva
	// ͨ��section�ڵ���Ϣ��ȡrva��Ӧ��foa
	//************************************
	int RvaToFoa(int rva);


	//************************************
	// Method:    getPEName
	// FullName:  PEBase::getPEName
	// Access:    public 
	// Returns:   void
	// ��ȡ��ǰPEClass����pe��ȫ·������
	//************************************
	void getPEName(char *temp);

	//************************************
	// Method:    ~PEBase
	// FullName:  PEBase::~PEBase
	// Access:    public 
	// Returns:   
	// �ͷ�������ڴ�
	//************************************
	void Release();
};


#endif

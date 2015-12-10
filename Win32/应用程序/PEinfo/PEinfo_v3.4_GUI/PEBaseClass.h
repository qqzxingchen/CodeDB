#ifndef PEBASECLASS__H
#define PEBASECLASS__H

#include <windows.h>
#include <winnt.h>

//用来存放PE文件中前面几个大小确定的结构体的一些信息，包括DOSheader，ntheader，section
class PEBase	
{
public:
	static const char CharacterStr[16][50];
	static const char DllCharacterStr[][35];
	static const char DataDirectoryName[16][25];


private:
	char				fFullPath[256];		//PE文件的全路径
public:
	IMAGE_DOS_HEADER	dosHeader;			//
	IMAGE_NT_HEADERS	ntHeader;
	LPVOID				pSection;
	char				ErrorString[100];	//当BOOL型函数发生错误时，可以读取这个字符串得知错误原因

private:
	
	//************************************
	// Method:    checkExtendName
	// FullName:  PEBase::checkExtendName
	// Access:    private 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: char * extendName
	// 核查文件的后缀名是否符合要求
	//************************************
	BOOL checkExtendName(char * extendName);

	//************************************
	// Method:    getDosNTSection
	// FullName:  PEBase::getDosNTSection
	// Access:    private 
	// Parameter: char * filePath		文件名
	// Returns:   BOOL
	// 获取文件的dosheader和NTheader结构体和Section信息，并核查是否符合PE文件格式要求
	//************************************
	BOOL getDosNTSection(char *filePath);	

public:

	//************************************
	// Method:    Init
	// FullName:  PEBase::Init
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: char * filePath		文件名
	// Parameter: char * extendName		文件后缀名
	//通过传来的路径，检查文件是否为exe或者dll或者sys后缀，是否符合PE文件格式的基本要求		
	//************************************
	BOOL Init(char * filePath,char * extendName);

	//************************************
	// Method:    RvaToFoa
	// FullName:  PESection::RvaToFoa
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: int rva
	// 通过section内的信息获取rva对应的foa
	//************************************
	int RvaToFoa(int rva);


	//************************************
	// Method:    getPEName
	// FullName:  PEBase::getPEName
	// Access:    public 
	// Returns:   void
	// 获取当前PEClass类中pe的全路径名称
	//************************************
	void getPEName(char *temp);

	//************************************
	// Method:    ~PEBase
	// FullName:  PEBase::~PEBase
	// Access:    public 
	// Returns:   
	// 释放申请的内存
	//************************************
	void Release();
};


#endif

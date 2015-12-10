#ifndef PEBASECLASS__H
#define PEBASECLASS__H

#include <windows.h>
#include <winnt.h>
#include "StringTemplate.h"
#include "PEOptionHeader.h"

//用来存放PE文件中前面几个大小确定的结构体的一些信息，包括DOSheader，ntheader，section
class PEBase	
{
private:
	TCHAR				fFullPath[256];		//PE文件的全路径
	
	StringTemplate st;

public:
	static const char CharacterStr[16][50];
	static const char DataDirectoryName[16][25];
	static const char SectionCharacteristics_Base5[][50];
	static const char SectionCharacteristics_Base25[][50];

	IMAGE_DOS_HEADER	dosHeader;
    IMAGE_FILE_HEADER   fileHeader;
    PEOptionHeader    * optionHeader;
	LPVOID				pSection;
	TCHAR				ErrorString[256];

private :
	// 将会向文件里输出以下格式的数据：			{...},{...},{...},{...},{...}
	void writeDOSHeader( HANDLE hFile );
	void writeNTFileHeader( HANDLE hFile );
	void writeNTOptionalDataDirectory( HANDLE hFile );
	void writeSectionInfo( HANDLE hFile,int index );

    // 释放申请的内存
	void Release();
public:

	PEBase ( TCHAR filePath[] );

    // 获取文件的dosheader和NTheader结构体和Section信息，并核查是否符合PE文件格式要求
	BOOL getDosNTSection();	

    // 获取pe文件的路径名，但是无法保证该文件完全符合pe规范，仅仅是将该类初始化时的传入的文件名传出去
	const TCHAR * getPEFilePath();

    // 将数据写到temp文件中，文件的名字可以通过getTempFileName（）获取到
	BOOL writeToTempFile();

	const TCHAR * GetTempFileName( ){
		return TEXT("temp\\dos_header.txt");
	}

	// 通过section内的信息获取rva对应的foa
	ULONGLONG RvaToFoa(ULONGLONG rva);
};


#endif

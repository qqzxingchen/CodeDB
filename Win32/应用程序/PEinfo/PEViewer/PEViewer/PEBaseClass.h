#ifndef PEBASECLASS__H
#define PEBASECLASS__H

#include <windows.h>
#include <winnt.h>
#include "StringTemplate.h"
#include "PEOptionHeader.h"

//�������PE�ļ���ǰ�漸����Сȷ���Ľṹ���һЩ��Ϣ������DOSheader��ntheader��section
class PEBase	
{
private:
	TCHAR				fFullPath[256];		//PE�ļ���ȫ·��
	
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
	// �������ļ���������¸�ʽ�����ݣ�			{...},{...},{...},{...},{...}
	void writeDOSHeader( HANDLE hFile );
	void writeNTFileHeader( HANDLE hFile );
	void writeNTOptionalDataDirectory( HANDLE hFile );
	void writeSectionInfo( HANDLE hFile,int index );

    // �ͷ�������ڴ�
	void Release();
public:

	PEBase ( TCHAR filePath[] );

    // ��ȡ�ļ���dosheader��NTheader�ṹ���Section��Ϣ�����˲��Ƿ����PE�ļ���ʽҪ��
	BOOL getDosNTSection();	

    // ��ȡpe�ļ���·�����������޷���֤���ļ���ȫ����pe�淶�������ǽ������ʼ��ʱ�Ĵ�����ļ�������ȥ
	const TCHAR * getPEFilePath();

    // ������д��temp�ļ��У��ļ������ֿ���ͨ��getTempFileName������ȡ��
	BOOL writeToTempFile();

	const TCHAR * GetTempFileName( ){
		return TEXT("temp\\dos_header.txt");
	}

	// ͨ��section�ڵ���Ϣ��ȡrva��Ӧ��foa
	ULONGLONG RvaToFoa(ULONGLONG rva);
};


#endif

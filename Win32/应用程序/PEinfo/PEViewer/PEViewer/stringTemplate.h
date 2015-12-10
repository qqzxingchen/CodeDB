#pragma once

#include <Windows.h>

class StringTemplate
{
// �������ģ�壬Ҳ�Ǹ�PEviewer���õ�����һ��ģ��
public:
	static const char baseTemplate[];
	static const char importTemplate[];
    static const char exportTemplate[];

private:
	char baseStr[1024];
private:
	//************************************
	// Method:    stringContains
	// FullName:  StringTemplate::stringContains
	// Access:    private 
	// Returns:   int
	// Qualifier:
	// Parameter: char * source
	// Parameter: char * checkStr
	// ���ж�source���Ƿ����checkStr�ַ��������������򷵻ظô���source�е�λ��
	// ���򣬷���-1
	//************************************
	int stringContains( char * source,char * checkStr );

public:
	// �����ַ���ģ�塣������Ҫ���滻�Ĵ���Ҫʹ�� <<>> ������
	void setBaseStr ( const char baseStr[] );

	// name		��<<>>���������ַ���
	// value			��Ҫ���滻�ɵ��ַ���
	// ��<<name>>�滻��value���������<<name>>���򷵻�true�����滻�����򣬷���false
	void exchange( char * name,char * value );

	// ��ȡ��ǰbaseStr���ַ�������
	int getLength();

	// ��ȡ��ǰ��baseStr
	void getString( char * temp );
};

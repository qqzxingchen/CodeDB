#pragma once

#include <Windows.h>

class StringTemplate
{
// 最基本的模板，也是该PEviewer中用的最多的一个模板
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
	// 来判断source中是否包含checkStr字符串，若包含，则返回该串再source中的位置
	// 否则，返回-1
	//************************************
	int stringContains( char * source,char * checkStr );

public:
	// 设置字符串模板。其中需要被替换的串需要使用 <<>> 括起来
	void setBaseStr ( const char baseStr[] );

	// name		被<<>>括起来的字符串
	// value			将要被替换成的字符串
	// 将<<name>>替换成value。如果存在<<name>>，则返回true，并替换，否则，返回false
	void exchange( char * name,char * value );

	// 获取当前baseStr的字符串长度
	int getLength();

	// 获取当前的baseStr
	void getString( char * temp );
};

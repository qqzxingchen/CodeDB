#ifndef GLOBAL____H
#define GLOBAL____H

#include <Windows.h>


// 向文件句柄所指向的文件中写入n个字符（要求以\0结尾），要求hFile已经打开且已经指向想要写的位置
inline void writeStringToFile( HANDLE hFile,char c[] ){
	DWORD num;
	WriteFile( hFile,c,strlen(c),(LPDWORD)(&num),NULL);
}

// 写入一个逗号
inline void writeSeparatorToFile( HANDLE hFile ){
	writeStringToFile(hFile,",");
}

// 将一个文件中的内容写到另一个文件中去
inline void writeFileDataToFile( HANDLE fileTo,HANDLE fileFrom ){
    DWORD num;
    char str[1024];
    while(1){
        memset( str,0,1024 );
        ReadFile(fileFrom,str,1022,&num,NULL);

        if ( num == 0 ){
            break;
        }
        WriteFile(fileTo,str,num,(LPDWORD)(&num),NULL);
	}
}

// 宽字节串转换为多字节串
inline void wCharToMChar( const TCHAR* wChar,char* mChar ){
    DWORD num=WideCharToMultiByte(CP_ACP,0,wChar,-1,NULL,0,NULL,0);  
    WideCharToMultiByte(CP_ACP,0,wChar,-1,mChar,num,NULL,0);  
}
// 多字节串转换为宽字节串
inline void mCharToWChar( const char* mChar,TCHAR* wChar ){
//    DWORD num = MultiByteToWideChar(CP_ACP,0,mChar,-1,wChar,0);
    MultiByteToWideChar(CP_ACP,0,mChar,-1,wChar,MAX_PATH);
}


// 如果一个串中存在'\'，则将其转换为'\\'
inline void addTranslate( char * str ){
    char temp[MAX_PATH*2];
    for ( int i = strlen(str)-1 ; i >= 0 ; i -- ){
        if ( str[i] != '\\' )
            continue;
        wsprintfA( temp,"%s",&str[i] );
        wsprintfA( &str[i+1],"%s",temp );
    }
}

inline void getFileNameFromFullPath( const TCHAR * filePath,TCHAR * fileName ){
    for ( int i = wcslen(filePath)-1 ; i >= 0 ; i --  )
        if ( filePath[i] == '\\' ){
            wsprintf( fileName,TEXT("%s"),&filePath[i+1] );
            return ;
        }
}
#endif

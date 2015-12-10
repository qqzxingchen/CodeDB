#ifndef GLOBAL____H
#define GLOBAL____H

#include <Windows.h>


// ���ļ������ָ����ļ���д��n���ַ���Ҫ����\0��β����Ҫ��hFile�Ѿ������Ѿ�ָ����Ҫд��λ��
inline void writeStringToFile( HANDLE hFile,char c[] ){
	DWORD num;
	WriteFile( hFile,c,strlen(c),(LPDWORD)(&num),NULL);
}

// д��һ������
inline void writeSeparatorToFile( HANDLE hFile ){
	writeStringToFile(hFile,",");
}

// ��һ���ļ��е�����д����һ���ļ���ȥ
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

// ���ֽڴ�ת��Ϊ���ֽڴ�
inline void wCharToMChar( const TCHAR* wChar,char* mChar ){
    DWORD num=WideCharToMultiByte(CP_ACP,0,wChar,-1,NULL,0,NULL,0);  
    WideCharToMultiByte(CP_ACP,0,wChar,-1,mChar,num,NULL,0);  
}
// ���ֽڴ�ת��Ϊ���ֽڴ�
inline void mCharToWChar( const char* mChar,TCHAR* wChar ){
//    DWORD num = MultiByteToWideChar(CP_ACP,0,mChar,-1,wChar,0);
    MultiByteToWideChar(CP_ACP,0,mChar,-1,wChar,MAX_PATH);
}


// ���һ�����д���'\'������ת��Ϊ'\\'
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

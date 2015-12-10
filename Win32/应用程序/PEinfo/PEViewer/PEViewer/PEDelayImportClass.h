#pragma once

#include "PEBaseClass.h"
#include "stringTemplate.h"

typedef struct IMAGE_DELAY_IMPORT_DESCRIPTOR{
    DWORD Attributes;
    DWORD Name;
    DWORD ModuleHandle;
    DWORD DelayIAT;
    DWORD DelayINT;
    DWORD BoundDelayIT;
    DWORD UnLoadDelayIT;
    DWORD TimeStamp;
}IMAGE_DELAY_IMPORT_DESCRIPTOR,*PIMAGE_DELAY_IMPORT_DESCRIPTOR;

class PEDelayImportClass
{
private :
    PEBase * peBase;
    StringTemplate st;

    TCHAR ErrorString[256];

    void writeDataToFile( HANDLE peFile, HANDLE hFile );
public:
    PEDelayImportClass(PEBase *peBase);

    bool writeToTempFile();

    const TCHAR * GetTempFileName( ){
        return TEXT("temp\\delay_import_data.txt");
    }
};


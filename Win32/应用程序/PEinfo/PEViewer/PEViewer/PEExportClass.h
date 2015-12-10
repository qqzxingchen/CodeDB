#pragma once

#include "PEBaseClass.h"
#include "stringTemplate.h"

class PEExportClass
{
private :
    PEBase * peBase;
    StringTemplate st;

    TCHAR ErrorString[256];

    void writeDataToFile( HANDLE peFile, HANDLE hFile );
public:
    PEExportClass(PEBase *peBase);

    bool writeToTempFile();

    const TCHAR * GetTempFileName( ){
        return TEXT("temp\\export_data.txt");
    }
};

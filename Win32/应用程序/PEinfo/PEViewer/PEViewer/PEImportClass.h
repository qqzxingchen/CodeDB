#pragma once

#include "PEBaseClass.h"
#include "stringTemplate.h"

class PEImportClass
{
private :
	PEBase * peBase;
	StringTemplate st;

	TCHAR ErrorString[256];

	void writeDataToFile( HANDLE peFile, HANDLE hFile );
public:
	PEImportClass(PEBase *peBase);

	bool writeToTempFile();
	
	const TCHAR * GetTempFileName( ){
		return TEXT("temp\\import_data.txt");
	}

};

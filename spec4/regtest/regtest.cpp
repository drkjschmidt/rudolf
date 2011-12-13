// regtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// QueryKey - Enumerates the subkeys of key and its associated values.
//     hKey - Key whose subkeys and values are to be enumerated.

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
 
void QueryKey(HKEY hKey) 
{ 
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
 
    DWORD i, retCode; 
 
    TCHAR  achValue[MAX_VALUE_NAME]; 
    DWORD cchValue = MAX_VALUE_NAME; 
 
    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    
    if (cSubKeys)
    {
        printf( "\nNumber of subkeys: %d\n", cSubKeys);

        for (i=0; i<cSubKeys; i++) 
        { 
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                     achKey, 
                     &cbName, 
                     NULL, 
                     NULL, 
                     NULL, 
                     &ftLastWriteTime); 
            if (retCode == ERROR_SUCCESS) 
            {
                _tprintf(TEXT("(%d) %s\n"), i+1, achKey);
            }
        }
    } 
 
    // Enumerate the key values. 

    if (cValues) 
    {
        printf( "\nNumber of values: %d\n", cValues);

        for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
        { 
            cchValue = MAX_VALUE_NAME; 
            achValue[0] = '\0'; 
            retCode = RegEnumValue(hKey, i, 
                achValue, 
                &cchValue, 
                NULL, 
                NULL,
                NULL,
                NULL);
 
            if (retCode == ERROR_SUCCESS ) 
            { 
                _tprintf(TEXT("(%d) %s\n"), i+1, achValue); 
            } 
        }
    }
}

#define HKEY_READ_BUFFER_SIZE 256
#define BYTEINCREMENT HKEY_READ_BUFFER_SIZE

void ReadValue(HKEY hKey)
{

	DWORD BufferSize = HKEY_READ_BUFFER_SIZE;
    DWORD cbData;
    DWORD dwRet;
	DWORD dType;

    PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK) malloc( BufferSize );
    cbData = BufferSize;

    printf("\nRetrieving the data...");

    dwRet = RegQueryValueEx( hKey,
                             TEXT("caldir"),
                             NULL,
                             &dType,
                             (LPBYTE) PerfData,
                             &cbData );
    while( dwRet == ERROR_MORE_DATA )
    {
        // Get a buffer that is big enough.

        BufferSize += BYTEINCREMENT;
        PerfData = (PPERF_DATA_BLOCK) realloc( PerfData, BufferSize );
        cbData = BufferSize;

        printf(".");
        dwRet = RegQueryValueEx( hKey,
                         TEXT("caldir"),
                         NULL,
                         &dType,
                         (LPBYTE) PerfData,
                         &cbData );
	}
	if( dwRet == ERROR_SUCCESS ) {
		if (dType == REG_SZ) {
		  _tprintf(_T("\n\nFinal buffer size is %d\nContent is: %s"), BufferSize, (char *)PerfData);
		} else {
		  _tprintf(_T("\n\nFinal buffer size is %d\nWrong content type %ld"), BufferSize, dType);
		}
	}
    else printf("\nRegQueryValueEx failed (%d)\n", dwRet);

	free(PerfData);
}

void WriteValue(HKEY hKey)
{
	DWORD BufferSize = HKEY_READ_BUFFER_SIZE;
    DWORD dwRet;
	DWORD dType = REG_SZ;
	CString tval;

	tval=_T("123456789abcdefghij");
	dwRet = RegSetValueEx(
		hKey,
		TEXT("caldir"),
		NULL,
		REG_SZ,
		(const BYTE *)tval.GetString(),
		tval.GetAllocLength()*sizeof(TCHAR)
	);
	if( dwRet == ERROR_SUCCESS )
		printf("\nRegSetValueEx suceeded (%d)\n", dwRet);
    else 
		printf("\nRegQueryValueEx failed (%d)\n", dwRet);
}

#define HKEY_LIST 1
#define HKEY_SHOW 2
#define HKEY_ACTION HKEY_SHOW

void __cdecl _tmain(void)
{
   HKEY hTestKey;

   if( RegOpenKeyEx( HKEY_CURRENT_USER,
        /* TEXT("SOFTWARE\\Microsoft"), */
        TEXT("SOFTWARE\\Wilson Analytical\\LightPilot QB"),
		0,
        /* KEY_READ */ KEY_ALL_ACCESS,
        &hTestKey) == ERROR_SUCCESS
      )
   {
#if HKEY_ACTION == HKEY_LIST
	QueryKey(hTestKey); 
#else
	ReadValue(hTestKey);
	WriteValue(hTestKey);
#endif
   }
   else 
   {
	   printf("\nOOPS key not found!\n");
   }
   
   RegCloseKey(hTestKey);
}

/*
int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}
*/

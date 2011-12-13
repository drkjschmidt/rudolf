#include "StdAfx.h"
#include "RegistryHelper.h"

RegistryHelper::RegistryHelper(void)
{
}

RegistryHelper::~RegistryHelper(void)
{
}


bool
RegistryHelper::Init(CString &ClassPath)
{
	HKEY hKey;
	bool res=false;

	if(RegOpenKeyEx(
		HKEY_CURRENT_USER,
		ClassPath.GetString(),
		0,
		KEY_READ,
		&hKey) == ERROR_SUCCESS
		)
	{
		StoredClassPath=ClassPath;
		res=true;
	}
	else
	{
		StoredClassPath=_T("");
		res=false;
	}
	RegCloseKey(hKey);
	return res;
}

bool
RegistryHelper::ReadStringValue(const CString &name,CString &value)
{
	HKEY hKey;
	bool res=false;

	if(RegOpenKeyEx(
		HKEY_CURRENT_USER,
		StoredClassPath.GetString(),
		0,
		KEY_READ,
		&hKey) == ERROR_SUCCESS
		)
	{
		DWORD BufferSize = REGHELPER_BUF_SIZE;
		DWORD cbData;
		DWORD dwRet;
		DWORD dType;

		LPBYTE PerfData = (LPBYTE) malloc( BufferSize );
		cbData = BufferSize;

		// printf("\nRetrieving the data...");

		// Read the value ... probably we will be fine with one read
		dwRet = RegQueryValueEx( hKey,
			name.GetString(),
			NULL,
			&dType,
			(LPBYTE) PerfData,
			&cbData );

		// Keep increasing buffer size until we have read all available data ...
		// This is totally parnoid code. We could probably just set the string length
		// to something reasonable like 256 or 1024 ... but hey, this isn't *wrong* ...
		while( dwRet == ERROR_MORE_DATA )
		{
			// Get a buffer that is big enough.

			BufferSize += REGHELPER_BUF_INCR;
			PerfData = (LPBYTE) realloc( PerfData, BufferSize );
			cbData = BufferSize;

			printf(".");
			dwRet = RegQueryValueEx( hKey,
				name.GetString(),
				NULL,
				&dType,
				(LPBYTE) PerfData,
				&cbData );
		}

		if( dwRet == ERROR_SUCCESS ) {
			if (dType == REG_SZ) {
				res=true;
				value=_T(PerfData);
				// _tprintf(_T("\n\nFinal buffer size is %d\nContent is: %s"), BufferSize, (char *)PerfData);
			} else {
				res=false;
				value=_T("");
				// _tprintf(_T("\n\nFinal buffer size is %d\nWrong content type %ld"), BufferSize, dType);
			}
		}
		else {
			res=false;
			value=_T("");
			// printf("\nRegQueryValueEx failed (%d)\n", dwRet);
		}
		free(PerfData);
	}
	RegCloseKey(hKey);
	return res;
}

bool
RegistryHelper::WriteStringValue(const CString &name,const CString &value)
{
	HKEY hKey;
	bool res=false;

	if(RegOpenKeyEx(
		HKEY_CURRENT_USER,
		StoredClassPath.GetString(),
		0,
		KEY_WRITE,
		&hKey) == ERROR_SUCCESS
		)
	{

		// printf("\nRetrieving the data...");

		// Write the value 
		DWORD dwRet;
		DWORD dType = REG_SZ;

		dwRet = RegSetValueEx(
			hKey,
			name.GetString(),
			NULL,
			REG_SZ,
			(const BYTE *)value.GetString(),
			value.GetAllocLength()*sizeof(TCHAR)
			);


		if( dwRet == ERROR_SUCCESS ) {
			// printf("\nRegSetValueEx suceeded (%d)\n", dwRet);
			res=true;
		}
		else {
			//printf("\nRegQueryValueEx failed (%d)\n", dwRet);	}
			res=false;
		}
	}

	RegCloseKey(hKey);
	return res;
}

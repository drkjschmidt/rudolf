// gps_generic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gps_generic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		char dummy;
		unsigned int ii;
		printf("Code here ...\n");

		/*
char a[]="asdf";
printf (a); printf("\n");

CString ss="short";
printf("%S\n",ss.GetString());

CString sl;
sl.Format(L"LongFixed NormalInsert:%s TranscodeInsert:%S\n",L"asdf","asdf");
printf("%S\n",sl.GetString());

scanf("%c",&dummy);
exit(1);
*/

		CGPSgeneric myGPS;
		CGPSportList myList = myGPS.getPorts(CGPSPortTypesAll);
		printf("Found %d ports\n",myList.size());
		for (ii=0; ii<myList.size(); ii++) {
			_tprintf(_T("... type: %d\n"),myList[ii].getType());
			_tprintf(_T("... name: %s\n"),myList[ii].getDeviceName().GetString());
			_tprintf(_T("... port: %s\n"),myList[ii].getPortName().GetString());
			_tprintf(_T("... ID:   %s\n"),myList[ii].getDeviceID());
		}

		scanf("%c",&dummy);

	}

	return nRetCode;
}

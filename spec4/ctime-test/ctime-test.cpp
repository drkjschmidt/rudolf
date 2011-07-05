// ctime-test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ctime-test.h"

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
	}

    char dummy;
	time_t now;
	time(&now);
	CTime test(now);

	printf(test.FormatGmt(_T("Format string without length is ok: Hour: %H Minute: %M Second: %S\n")).GetString());
	scanf("%c",&dummy);
	printf(test.FormatGmt(_T("Format string with length crashes: Hour: %02H Minute: %M Second: %S\n")).GetString());
	scanf("%c",&dummy);

	return nRetCode;
}

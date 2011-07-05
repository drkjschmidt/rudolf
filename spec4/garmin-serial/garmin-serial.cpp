// garmin-serial.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "garmin-serial.h"
#include "RPGarmCom.h"

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
		CRPGarmCom garminPort;
		CStringArray ports;
		CString tstr,name;
		int ii;
		double lat,lon,alt;

		printf("hello world\n");

		garminPort.getGarminPorts(ports);
		printf("found %d serial ports\n",ports.GetCount());

		for (ii=0; ii< ports.GetCount(); ii++)
		{
			_tprintf(_T("Found port %s\n"),ports[ii]);
		}

		if (garminPort.setGarminPort(ports[0]) && garminPort.openGarminPort())
		{
			do {
				garminPort.updateInfo();
			} while (! garminPort.getName(&name)); // @@@ is this right?

			garminPort.getLoc(&lat, &lon, &alt);
			_tprintf(_T("Location: %g %g %g\n"),lat,lon,alt);
			_tprintf(_T("%s\n"),garminPort.cser(CGPSData::SerializeModes::outputCSV|CGPSData::SerializeModes::outputHDR).GetString());
			_tprintf(_T("%s\n"),garminPort.cser(CGPSData::SerializeModes::outputCSV).GetString());

			garminPort.closeComPort();

		}
		else
			_tprintf(_T("Failed to init garmin port\n"));
	}

	return nRetCode;
}

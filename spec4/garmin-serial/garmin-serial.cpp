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
			_tprintf(_T("[%d] Found port %s\n"),ii+1,ports[ii]);
		}
		_tprintf(_T("Select port"));
		int sel;
		scanf("%d",&sel);

		if ((sel>0) && (sel<=ports.GetCount()+1) && (garminPort.setGarminPort(ports[sel-1])) && garminPort.openGarminPort())
		{
			int count=0;
			do {
				printf(".");
				garminPort.updateInfo();
				Sleep(100);
			} while ((count++<100) && (! garminPort.getName(&name))); // @@@ is this right?

			if (garminPort.isValid()) {
				garminPort.getLoc(&lat, &lon, &alt);
				_tprintf(_T("Location: %g %g %g\n"),lat,lon,alt);
				_tprintf(_T("%s\n"),garminPort.cser(CGPSData::SerializeModes::outputCSV|CGPSData::SerializeModes::outputHDR).GetString());
				_tprintf(_T("%s\n"),garminPort.cser(CGPSData::SerializeModes::outputCSV).GetString());
			}
			else {
				printf("Doesn't look line NMEA data ...\n");
			}

			garminPort.closeComPort();

		}
		else
			_tprintf(_T("Failed to init garmin port\n"));
	}
    char dummy;
	scanf("%c",&dummy);
	scanf("%c",&dummy);

	return nRetCode;
}

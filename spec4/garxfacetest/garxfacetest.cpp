// garxfacetest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "garxfacetest.h"
#include "GarXfaceall.h"


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

		printf("Code to follow\n");

		GarXface4::Gps m_gps;
		// GarXface4::GpsUsbDevice m_GpsUsbDevice;
		// GarXface4::GpsComPortDevice m_GpsComPortDevice;

		GarXface4::ComPortNameList m_comportlst;
		GarXface4::UsbDeviceNameList m_usbdevicelst;

		// COM port devices (not NMEA)
		printf("Number of com ports: %d\n",m_comportlst.GetCount());


		// c = min(m_comportlst.GetCount(),MAX_PORTS);
		unsigned int c=m_comportlst.GetCount();
		unsigned int u;

		unsigned int m_nTotalPorts = 0;

	/*
		for(u=0; u<c; u++)
		{
			
			GarXface4::ComPortName *cpn = m_comportlst[u];
			GarXface4::GpsComPortDevice comDevice;

			try
			{				
				
				const char *c = cpn->GetDeviceName();
				printf("... %s\n",c);
				comDevice.SetPortName(cpn->GetDeviceName());
				m_gps.Open(&comDevice);		
				// m_Ports[m_nTotalPorts] = new CGarXfaceGpsPort(c,CGarXfaceGpsPort::ComPort);
				CString fullName;
				fullName.Format(_T("%s\n"),m_gps.GetProductData()->GetDescription());
				
							
				CString tmp;
				tmp.Format(_T("%s (%s)"),fullName,c); 			
				// m_ctrlDevices.AddString(tmp);
				printf("... %s\n",tmp.GetString());
				m_nTotalPorts++;

			}
			catch (GarXface4::Exceptions::GxException)
			{
				

			}

			if (m_gps.IsOpen())
					m_gps.Close();

		}	

		printf("Number of usb ports: %d\n",m_usbdevicelst.GetCount());
	*/

		c = m_usbdevicelst.GetCount();

		
		for(u=0; u<c; u++)
		{
			
			GarXface4::UsbDeviceNamePtr upn = m_usbdevicelst[u];
#define TEST_KILLABLE 1
#if TEST_KILLABLE == 1
			GarXface4::GpsUsbDevice *usbKillDevice = new GarXface4::GpsUsbDevice;
			try
			{				
				const char *c=upn->GetDeviceName();
				printf("... devname (1): %s\n",c);
				usbKillDevice->SetDeviceName(c);
				m_gps.Open(usbKillDevice);
				delete usbKillDevice;


				const char *portName = upn->GetDeviceName();
				printf("... devname (2): %s\n",portName);
				// m_Ports[m_nTotalPorts] = new CGarXfaceGpsPort(portName,CGarXfaceGpsPort::UsbPort);
				m_nTotalPorts++;
				
				CString fullName;
				fullName.Format(_T("%s\n"),m_gps.GetProductData()->GetDescription());
				
				m_gps.RxUnitID();
				unsigned long ul = m_gps.GetUnitID();
				
				
				CString tmp;
				tmp.Format(_T("%s (Unit ID %d)"),fullName,m_gps.GetUnitID()); 			
				// m_ctrlDevices.AddString(tmp);				
				printf("... %d: %s\n",u,tmp.GetString());


			}
#else
			GarXface4::GpsUsbDevice usbDevice;
			try
			{				
				const char *c=upn->GetDeviceName();
				printf("... devname (1): %s\n",c);
				usbDevice.SetDeviceName(c);
				m_gps.Open(&usbDevice);

				const char *portName = upn->GetDeviceName();
				printf("... devname (2): %s\n",portName);
				// m_Ports[m_nTotalPorts] = new CGarXfaceGpsPort(portName,CGarXfaceGpsPort::UsbPort);
				m_nTotalPorts++;
				
				CString fullName;
				fullName.Format(_T("%s\n"),m_gps.GetProductData()->GetDescription());
				
				m_gps.RxUnitID();
				unsigned long ul = m_gps.GetUnitID();
				
				
				CString tmp;
				tmp.Format(_T("%s (Unit ID %d)"),fullName,m_gps.GetUnitID()); 			
				// m_ctrlDevices.AddString(tmp);				
				printf("... %d: %s\n",u,tmp.GetString());


			}
#endif

			catch (GarXface4::Exceptions::GxException)
			{
				printf("OOPS: caught exception\n");

			}

				if (m_gps.IsOpen())
					m_gps.Close();
		}


		// This is ugly because it assumes that a GPS *is* available on USB port 0 ... but it's merely inteded to
		// test PVT function ...
		if (m_usbdevicelst.GetCount() > 0) {
			GarXface4::GpsUsbDevice usbDevice;

			usbDevice.SetDeviceName(
				((GarXface4::UsbDeviceNamePtr)(m_usbdevicelst[0]))->GetDeviceName()
				);
			int res = m_gps.Open(&usbDevice);

			printf("Open of device %d resulted in code %d\n",0,res);

			if (m_gps.IsOpen())
			{
				try
				{				
					
					m_gps.PvtDataOn();
					
					// m_nTimer = SetTimer(1, 50, 0);
					
				}
				catch(GarXface4::Exceptions::GxException &e)
				{				
					e.ReportError();
				}
			}

			for (int ii=0; ii<20; ii++) {
				Sleep(500);

				GarXface4::PVTdata *p;
				try
				{
					// long t = CTime::GetCurrentTime().GetSecond();       
					p = m_gps.GetPVTdata();

					

					// t =  CTime::GetCurrentTime().GetSecond();
				}
				catch(GarXface4::Exceptions::GxException &e)
				{
					e.ReportError();
				}

				if (p->GetGpsConnect()) {
					float ve = p->GetVelocityEast();
					float vn = p->GetVelocityNorth();
					float vu = p->GetVelocityUp();
					
					float m_nAltitude = p->GetAltitude();
					float m_nEpe = p->GetEstimatedPositionError();
					int m_nFix = p->GetFixType();
					double m_dLatitude = GarXface4::Math::WGS84::radiansToDegrees(p->GetLatitude());
					double m_dLongitude = GarXface4::Math::WGS84::radiansToDegrees(p->GetLongitude());
					double m_dSpeed = GarXface4::Math::Velocity::calculateSpeed(ve,vn,vu);
					double m_dHeading = GarXface4::Math::Velocity::calculateHeadingDeg(ve,vn);

					long timeRaw = GarXface4::Math::UTC::GarminTimeToLongTime(
						p->GetWnDays(),  
						p->GetTimeOfWeek(), 
						p->GetLeapSeconds(), 
						false);
		
					CTime t(timeRaw);
					SYSTEMTIME systime;
					GetSystemTime(&systime);
					CTime nowtime(systime);
					CTimeSpan diff=nowtime - t;
					printf("TZ diff: %dh = %ds\n",diff.GetHours(),diff.GetTotalSeconds());
					
					CString m_strDateTime=t.FormatGmt( "%A, %B %d, %Y  %I:%M:%S %p" );

					if (p->GetValidData())
					{
						// printf("Data is currently valid:\n");
						wprintf(m_strDateTime.GetString());
						printf("... lat %f lon %f alt %f fix %d err %f\n",
							m_dLatitude,m_dLongitude,m_nAltitude,m_nFix,m_nEpe);
					}
					else
						printf("Data is currently invalid:\n");      

					
				}
			}

			try
			{
				m_gps.PvtDataOff();
				m_gps.Close();
			}
			catch(GarXface4::Exceptions::GxException &e)
			{				
				
			}


			if (m_gps.IsOpen())
				m_gps.Close();
		}


		scanf("%c",&dummy);
	}

	

	return nRetCode;
}

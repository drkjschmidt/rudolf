#include "StdAfx.h"
#include "GPSgeneric.h"

CGPSgeneric::CGPSgeneric(void)
: isopen(false)
{
}

CGPSgeneric::~CGPSgeneric(void)
{
}

CGPS8bitStringContainer::CGPS8bitStringContainer(void) 
: strData(0)
{
}

CGPS8bitStringContainer::CGPS8bitStringContainer(const CGPS8bitStringContainer &that)
{
	// Copy constructor should be simpler than assignment:
	// * we don't need to check for self assignment
	// * we don't need to delete things

	// using str functions here should be safe because we
	// assume that devName will always be a narrow string
	strData=new char[strlen(that.strData) + 1];
	if (strData != NULL) {
		strncpy(strData,that.strData,strlen(that.strData)+1);
	}
}


CGPS8bitStringContainer::~CGPS8bitStringContainer(void) 
{
	delete strData;
}


CGPS8bitStringContainer& 
CGPS8bitStringContainer::operator =(const CGPS8bitStringContainer &that)
{
	if (this != &that) {
		delete strData;

		// using str functions here should be safe because we
		// assume that devName will always be a narrow string
		strData=new char[strlen(that.strData) + 1];
		if (strData != NULL) {
			strncpy(strData,that.strData,strlen(that.strData)+1);
		}
	}
	return *this;
}


void 
CGPS8bitStringContainer::setStr8bit(const char *set)
{
	delete strData;

	// using str functions here should be safe because we
	// assume that devName will always be a narrow string
	strData=new char[strlen(set) + 1];
	if (strData != NULL) {
		strncpy(strData,set,strlen(set)+1);
	}
}

const char *
CGPS8bitStringContainer::getStr8bit()
{ 
	return strData; 
}



CGPSport::CGPSport()
: type(CGPSPortTypesUnset)
, isValidDeviceName(false)
, isValidDeviceID(false)
, isValidLat(false)
, isValidLon(false)
, isValidAlt(false)
, isValidSpd(false)
, isValidDir(false)
, isValidUTCTime(false)
{
}

CGPSport::CGPSport(CGPSPortType stype, const char *spname, const char *sdname, const char *sID)
: type(CGPSPortTypesUnset)
, isValidDeviceName(false)
, isValidDeviceID(false)
, isValidLat(false)
, isValidLon(false)
, isValidAlt(false)
, isValidSpd(false)
, isValidDir(false)
, isValidUTCTime(false)

{
	type=stype;
	// NOTE: these should be ASCII strings as they come from the USB device
	// as such it should be ok, to use str functions ...
	/* pretty but hard to debug ... 
	if (portName  =(char *)malloc(strlen(spname)+1)) strcpy(portName  ,spname);
	if (deviceName=(char *)malloc(strlen(sdname)+1)) strcpy(deviceName,sdname);
	if (deviceID  =(char *)malloc(strlen(sID)   +1)) strcpy(deviceID  ,sID);
	*/

	size_t len;
	size_t mul=sizeof(char);

	portName.setStr8bit(spname);
	deviceName.setStr8bit(sdname);
	deviceID.setStr8bit(sID);
}

/*
CGPSport::CGPSport(CGPSPortType stype, const char* spname, const char* sdname,const char *sID)
{
	type=stype;
	portName=spname;
	deviceName=sdname;
	deviceID=sID;
}
*/

CGPSport::~CGPSport(void)
{
}




CGPSportList CGPSgeneric::getPorts(CGPSPortType filter) {
	CGPSportList myList;

#define listNMEA 1
#define listGSER 2
#define listGUSB 4
	int listTypes=0;

	int ii;
	int count,unit;


	switch (filter) {
		case CGPSPortTypesAll:
			printf("List all available ports\n");
			listTypes=listNMEA|listGSER|listGUSB;
			break;
		case CGPSPortTypesSerialNMEA:
			printf("List available COM/NMEA ports\n");
			listTypes=listNMEA;
			break;
		case CGPSPortTypesSerialGarXface:
			printf("List available GarXface serical ports\n");
			listTypes=listGSER;
			break;
		case CGPSPortTypesUSBGarXface:
			printf("List available GarXface usb ports\n");
			listTypes=listGUSB;
			break;
		default:
			break;
	}

#if USE_RPNMEA == 1
	if (listTypes & listNMEA) {
		CStringArray ports;
		garNMEA.getGarminPorts(ports);
		for (ii=0; ii<ports.GetCount(); ii++)
		{
			CString deviceName=_T("(NMEA)");
			CString deviceID=_T("(unknown)");

			// @@@ I think this will go all wrong ... 
			myList.push_back(
				CGPSport(
					CGPSPortTypesSerialNMEA,
					ports[ii].GetBuffer(),
					deviceName,
					deviceID
					)
				);
		}
	}
#endif

#if USE_GARXFACE == 1
	if (listTypes & listGSER) {
		// Create objects for each USB port we think has a device attached
		count=garCOMList.GetCount();
		if (count > 0) {

			for(unit=0; unit<count; unit++)
			{
#if GARXFACE_HAS_SCOPE_ISSUES == 1
				cpn = garCOMList[unit];
#else
				GarXface4::ComPortNamePtr cpn = garCOMList[unit];
				GarXface4::GpsComPortDevice comDevice;
#endif				
				try
				{				
					const char *portname=cpn->GetDeviceName();
					comDevice.SetPortName(portname);
					garGPS.Open(&comDevice);

					//CString portName;
					//portName.Format(_T("%s"),portname);
					// printf("... %s\n",portName.GetString());

					//CString deviceName;
					//deviceName.Format(_T("%s"),garGPS.GetProductData()->GetDescription());
					
					garGPS.RxUnitID(); // update info from GPS
					unsigned long ul = garGPS.GetUnitID(); // get info from GPS class
					//CString deviceID;
					//deviceID.Format(_T("%lu"),ul);
										
					char deviceid[80]; // total overkill for a long ...
					sprintf(deviceid,"%lu",ul);

					myList.push_back(
						CGPSport(
							CGPSPortTypesSerialGarXface,
							portname,
							garGPS.GetProductData()->GetDescription(),
							deviceid
							)
						);
				}
				catch (GarXface4::Exceptions::GxException)
				{
					// Do nothing ... an exception here usually just means there is no 
					// Garmin device on this port ... 
				}

				if (garGPS.IsOpen())
					garGPS.Close();
			}
		}
	}
	
	if (listTypes & listGUSB) {
		// Create objects for each USB port we think has a device attached
		count=garUSBList.GetCount();
		if (count > 0) {

			for(unit=0; unit<count; unit++)
			{
#if GARXFACE_HAS_SCOPE_ISSUES == 1
				upn = garUSBList[unit];
#else
				GarXface4::UsbDeviceNamePtr upn = garUSBList[unit];
				GarXface4::GpsUsbDevice usbDevice;
#endif
				try
				{				
					const char *portname=upn->GetDeviceName();
					usbDevice.SetDeviceName(portname);
					garGPS.Open(&usbDevice);

					// CString portName;
					// portName.Format(_T("%s"),portname);
					// printf("... %s\n",portName.GetString());

					// CString deviceName;
					// deviceName.Format(_T("%s"),garGPS.GetProductData()->GetDescription());
					
					garGPS.RxUnitID(); // update info from GPS
					unsigned long ul = garGPS.GetUnitID(); // get info from GPS class
					// CString deviceID;
					// deviceID.Format(_T("%lu"),ul);
					
					char deviceid[80]; // total overkill for a long ...
					sprintf(deviceid,"%lu",ul);

					myList.push_back(
						CGPSport(
							CGPSPortTypesUSBGarXface,
							portname,
							garGPS.GetProductData()->GetDescription(),
							deviceid
							)
						);

				}
				catch (GarXface4::Exceptions::GxException)
				{
					// Do nothing ... an exception here usually just means there is no 
					// Garmin device on this port ... 
				}

				if (garGPS.IsOpen())
					garGPS.Close();
			}
		}
	}	
	
	
#endif


	return myList;
}

bool CGPSgeneric::openPort(CGPSport &select)
{
#if USE_GARXFACE == 1
#if GARXFACE_HAS_SCOPE_ISSUES != 1
	GarXface4::GpsComPortDevice comDevice;
	GarXface4::GpsUsbDevice usbDevice;
#endif
#endif

	switch (select.getType()) {
#if USE_RPNMEA == 1
		case CGPSPortTypesSerialNMEA:
			if (garNMEA.setGarminPort(select.getPortNameHR()) &&
			    garNMEA.openGarminPort())
			{
				// @@@ we should really do a test read here to validate this is an NMEA device ...
				isopen=true;
				curPort=select;
				return true;
			}
			isopen=false;
			return false;
			break;
#endif
#if USE_GARXFACE == 1
		case CGPSPortTypesSerialGarXface:
			// I am tempted to just do nothing *here* and return trune
			// then do the open close for each read ...

			if (garGPS.IsOpen()) {
				garGPS.Close();
				isopen=false;
			}


			try
			{				
				const char *portname=CT2CA(select.getPortName());
				comDevice.SetPortName(portname);
				garGPS.Open(&comDevice);
			}
			catch (GarXface4::Exceptions::GxException)
			{
				// Something went wrong ... bail
				isopen=false;
				return false;
			}
			isopen=true;
			curPort=select;
			return true;
			break;

		case CGPSPortTypesUSBGarXface:
			// I am tempted to just do nothing *here* and return trune
			// then do the open close for each read ...

			if (garGPS.IsOpen()) {
				garGPS.Close();
				isopen=false;
			}

			try
			{				
				const char *portname=CT2CA(select.getPortName());
				usbDevice.SetDeviceName(portname);
				garGPS.Open(&usbDevice);
			}
			catch (GarXface4::Exceptions::GxException)
			{
				// Something went wrong ... bail
				isopen=false;
				return false;
			}
			isopen=true;
			curPort=select;
			return true;
			break;
#endif
		default:
			return false;

	}

	return true;
}

bool CGPSgeneric::findPort(CGPSport *portinfo,CString *portname, CString *devicename, CString *deviceid)
{
	CGPSportList wholeList,shortList;
	unsigned int ll,ii;

	// No port destination object ... err ... no
	if (portinfo==NULL)
		return false;

	// No info, no dice
	if (portname==NULL && devicename==NULL && deviceid==NULL)
		return false;

	wholeList=getPorts();

	for (ll=0; ll<4; ll++) {

		// no point looking if there are no devices to be found
		if (wholeList.size() == 0)
			return false;

		// no point looking any further if there is just one match
		if (wholeList.size() == 1u) {
			*portinfo=wholeList.front();
			return true;
		}

		switch (ll) {
			case 0:
				// See if we know any device by name
				if (devicename!=NULL) {
					for (ii=0; ii<wholeList.size(); ii++) {
						if (wholeList[ii].getDeviceName() == *devicename) {
							shortList.push_back(wholeList[ii]);
						}
					}
					if (shortList.size() > 0)
					{
						wholeList=shortList;
						shortList.clear();
					}
				}
				break;
			case 1:
				// See if we know any device by ID
				if (deviceid!=NULL) {
					for (ii=0; ii<wholeList.size(); ii++) {
						if (wholeList[ii].getDeviceID() == *deviceid) {
							shortList.push_back(wholeList[ii]);
						}
					}
					if (shortList.size() > 0)
					{
						wholeList=shortList;
						shortList.clear();
					}
				}
				break;
			case 2:
				// See if we know any device by port
				if (portname!=NULL) {
					for (ii=0; ii<wholeList.size(); ii++) {
						if (wholeList[ii].getPortName() == *portname) {
							shortList.push_back(wholeList[ii]);
						}
					}
					if (shortList.size() > 0)
					{
						wholeList=shortList;
						shortList.clear();
					}
				}
				break;
			case 3:
				// empty loop to do simple returns
			default:
				// WTF?
				break;
		}
	}

	// If we haven't returned yet, it means we have devices but don't have any matches, so pick a random
	// device. At a guess the ports should be ordered NMEA, Ser, USB, so pick from the back
	*portinfo=wholeList.back();

	return true;


}

bool CGPSgeneric::closePort(CGPSport &select)
{
	isopen=false;
	switch (select.getType()) {
#if USE_RPNMEA == 1
		case CGPSPortTypesSerialNMEA:
			// @@@ really? no validation whatsoever? And which port are we closing?
			garNMEA.closeGarminPort();
			return true;
			break;
#endif
#if USE_GARXFACE == 1
		case CGPSPortTypesSerialGarXface:
		case CGPSPortTypesUSBGarXface:
			// I am tempted to just do nothing *here* and return trune
			// then do the open close for each read ...

			if (garGPS.IsOpen()) {
				return (garGPS.Close() == 0);
			}
			return true;
			break;

#endif
		default:
			return false;

	}

	return true;
}


bool CGPSgeneric::updateInfo(CGPSport &select,bool invalidate,int maxwait)
{
	int ii;
	int sleeptime=50; // ms
	int loopcount;
	bool res=true;

	double err;
	int fix;
	double lat,lon,alt;
	double spd,hdg;
	float ve,vn,vu;
	CString name;
	CTime utctime;

	// Easy waiting ... loop ... 
	if (maxwait>0) {
		loopcount=maxwait*1000/sleeptime;
	}
	else {
		loopcount=2;
	}


	switch (select.getType()) {
#if USE_RPNMEA == 1
		case CGPSPortTypesSerialNMEA:
			// should we be invalidating GPS info first?
			if (invalidate) {
				select.invalidateInfo();
				select.invalidateDeviceInfo();
			}

			for (ii=0; ii<loopcount; ii++) {
				res=garNMEA.updateInfo((ii==0)?(invalidate):(false));
				if (res&&
					garNMEA.getName(NULL) && 
					garNMEA.getLoc(NULL,NULL,NULL) &&
					garNMEA.getTimeData(NULL,NULL)) 
				{
					// we are done ...
					break;
				}
			}
			if (res) {				
				CGPSData garData=garNMEA.getGPSInfo();
				if (garData.getLoc(&lat,&lon,&alt))
					select.setInfo(&lat,&lon,&alt,NULL,NULL,NULL,NULL,NULL);
				if (garData.getMov(&spd,&hdg))
					select.setInfo(NULL,NULL,NULL,&spd,&hdg,NULL,NULL,NULL);
				if (garData.getName(&name)) {
					// @@@ probably totally wrong
					const char *tname=name.GetString();
					select.setInfo(NULL,NULL,NULL,NULL,NULL,NULL,tname,NULL);
				}
				if (garData.getTimeDate(&utctime))
					select.setInfo(NULL,NULL,NULL,NULL,NULL,&utctime,NULL,NULL);
			}
			break;
#endif
#if USE_GARXFACE == 1
		case CGPSPortTypesSerialGarXface:
		case CGPSPortTypesUSBGarXface:
			// should we be invalidating GPS info first?
			if (invalidate) {
				select.invalidateInfo();
				// @@@ should we keep this and set the device info below?
				// select.invalidateDeviceInfo();
			}


			if (! garGPS.IsOpen()) 
				return false;

			// Set GPS to return PVT data ... 
			try
			{				
				garGPS.PvtDataOn();
			}
			catch(GarXface4::Exceptions::GxException &e)
			{				
				// e.ReportError();
				return false;
			}

			// GPS should now be delivering PVT data ...
			// ... read data until we are happy or tired ...
			GarXface4::PVTdata *p;
			for (int ii=0; ii<loopcount; ii++) {

				try
				{
					p = garGPS.GetPVTdata();
				}
				catch(GarXface4::Exceptions::GxException &e)
				{
					// e.ReportError();
					// @@@ do we want to return or first stop PVT?
					//return false;
					res=false;
					break;
				}

				// Stop looping if we have what we wanted ...
				if (p->GetGpsConnect() && p->GetValidData())
					break;

				// Data is no good yet ... sleep a bit
				Sleep(sleeptime);
			}

			// Make sure we exited the loop with good data ...
			if (p->GetGpsConnect() && p->GetValidData()) {

				ve = p->GetVelocityEast(); // m/s
				vn = p->GetVelocityNorth();
				vu = p->GetVelocityUp();
					
				alt = (double)p->GetAltitude(); // m
				err = (double)p->GetEstimatedPositionError(); // m
				fix = (int)p->GetFixType(); // we might want to check this for minimum quality ...
				lat = GarXface4::Math::WGS84::radiansToDegrees(p->GetLatitude());
				lon = GarXface4::Math::WGS84::radiansToDegrees(p->GetLongitude());
				spd = GarXface4::Math::Velocity::calculateSpeed(ve,vn,vu); // undocumented units
				hdg = GarXface4::Math::Velocity::calculateHeadingDeg(ve,vn); // undocumented units

				long timeRaw = GarXface4::Math::UTC::GarminTimeToLongTime(
					p->GetWnDays(),  
					p->GetTimeOfWeek(), 
					p->GetLeapSeconds(), 
					true);
				CTime UTC(timeRaw);

				select.setInfo(&lat,&lon,&alt,&spd,&hdg,&UTC,NULL,NULL);
			}
			else {
				res=false;
			}

			// No more PVT data for now ... thank you
			try
			{
				garGPS.PvtDataOff();
				// garGPS.Close();
			}
			catch(GarXface4::Exceptions::GxException &e)
			{				
				return false;	
			}
			return res;
			break;
#endif
		default:
			return false;

	}

	return true;
}


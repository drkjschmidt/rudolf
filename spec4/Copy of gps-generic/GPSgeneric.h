#pragma once
// Generic GPS class 
//
// Currently this is a wrapper for 
//
//  * RPGarmCom - class to read data from Serial Port attached NMEA devices
//  * GarXface4 - class to read data from USB/Serial Port attached Garmin GPS devices
//
#define USE_GARXFACE 1
#define USE_RPNMEA   1

enum CGPSPortTypes {
	all = 0,
	serialNMEA = 1,
	serialGarXface = 2,
	usbGarXface = 3,
} PortType;

class CGPSport {
public:
	CGPSport(void);
	~CGPSport(void);
	
	CGPSPortType getType(void);
	CString getPortName(void);
	CString getDeviceName(void);

protected:
}

class CGPSgeneric
{
public:
	CGPSgeneric(void);
	~CGPSgeneric(void);



	CGPSportList getPorts(PortType filter);

};

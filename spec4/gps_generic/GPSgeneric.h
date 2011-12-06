#pragma once
// Generic GPS class 
//
// Currently this is a wrapper for 
//
//  * RPGarmCom - class to read data from Serial Port attached NMEA devices
//  * GarXface4 - class to read data from USB/Serial Port attached Garmin GPS devices
//
// also currently the com port classes for NMEA and the GARXFACE library do not play 
// together well at all so it should only be under exceptional circumstances that we want to
// enable both!
//
// These constants should really be set in the build definitions, but just in case set a sane
// default here ... 
#if !defined USE_RPNMEA
#define USE_RPNMEA   0
#endif
#if !defined USE_GARXFACE
#define USE_GARXFACE 1
#endif

#if USE_RPNMEA == 1
#define NMEAINFO "[NMEA] "
#else
# define NMEAINFO "[no NMEA] "
#endif
#if USE_GARXFACE == 1
#define GARXFACEINFO "[GarXface4]"
#else
#define GARXFACEINFO "[no GarXface4]"
#endif

// Don't exactly remember but I think this was to indicate that you cannot instanciate multiple
// garxface objects because it internally uses static variables
#define GARXFACE_HAS_SCOPE_ISSUES 1

#include <vector> // ATL vector template
// #include "atlconv.h" // I don't think this is useful unless we use variable length (UNICODE) strings ... 

// @@@ currently we need to include this no matter what because we have painted ourselves into
// @@@ a corner with using CGPSData ...
//#if USE_RPNMEA == 1
#  include "RPGarmCom.h"
//#endif

#if USE_GARXFACE == 1
#  include "GarXfaceAll.h"
#endif


typedef enum CGPSPortTypes {
	CGPSPortTypesUnset = 0,
	CGPSPortTypesAll = 7,
	CGPSPortTypesSerialNMEA = 1,
	CGPSPortTypesSerialGarXface = 2,
	CGPSPortTypesUSBGarXface = 4,
} CGPSPortType;


class CGPSgeneric;

//! String type wrapper to deal with the char/wchar_t conversion problems ...
class CGPS8bitStringContainer {

public:
	//! void constructor
	CGPS8bitStringContainer(void);

	//! copy constructor
	CGPS8bitStringContainer(const CGPS8bitStringContainer &that);

	//! destructor
	~CGPS8bitStringContainer(void);

	//! explicit assignment operator
	CGPS8bitStringContainer& operator =(const CGPS8bitStringContainer &that);


	//! This function assumes that the string given is 8bit (as returned by serial device)
	void setStr8bit(const char *set);

	//! returns the unaltered 8bit string
	const char *getStr8bit();

private:
	// Store the name in narrow 8 bit ASCII
	char *strData;
};

class CGPSport {
public:
	//! Default constructor ... really shouldn't be using this ...
	CGPSport();

	//! Create a port/device object to store all the relevant information. 
	//! Generally this should be for internal use only.
	//
	//! @param type - pick from CGPSport enum
	//! @param port_name - some sort of identifier for the port to which the device is attached
	//! @param device_name - the name of the device if we know it
	//! @param device_id - a unique ID reported by the device if we know it
	CGPSport(CGPSPortType stype, const char *spname, const char *sdname, const char *sid);

	// This seemed like a good idea but probably isn't even if we *did* use UNICODE ... 
	// CGPSport(CGPSPortType stype, const char *spname, const char *sdname, const char *sid);
	//! Destructor ... this should make sure that ports are appropriately closed to release devices
	~CGPSport(void);
	
	//! Query type (see constructor for more details)
	inline CGPSPortType getType(void) { return type; };

	//! Query port_name (see constructor for more details)
	inline CString getPortNameHR(void) { CString tpn; tpn.Format(_T("%s"),portName.getStr8bit()); return tpn; };
	inline const char* getPortName(void) { return portName.getStr8bit(); };

	//! Query device_name (see constructor for more details)
	inline CString getDeviceNameHR(void) { CString tdn; tdn.Format(_T("%s"),deviceName.getStr8bit()); return tdn; }; 
	inline const char * getDeviceName(void) { return deviceName.getStr8bit(); };

	//! Query device_id (see constructor for more details)
	inline CString getDeviceIDHR(void) { CString tdi; tdi.Format(_T("%s"),deviceID.getStr8bit()); return tdi; }; 
	inline const char * getDeviceID(void) { return deviceID.getStr8bit(); };


	//! Get location info
	//
	//! @param lat - latitude in degrees
	//! @param lon - longitude in degrees
	//! @param alt - altitude in m
	//! @param update - if this is set to true query the device, if it is set to false, return cached data.
	//!                 This allows for quick calls on NMEA devices that would otherwise need to wait for
	//!                 up to 60s for a valid response
	//! @param maxwait - How long to wait for a "valid" response before aborting and returning an error
	inline bool getLoc(double *lat, double *lon, double *alt) { 
		if (!(isValidLat && isValidLon && isValidAlt))
			return false;
		if (lat != NULL) *lat=locLat;
		if (lon != NULL) *lon=locLon;
		if (alt != NULL) *alt=locAlt;
		return true;
	};

	//! Get motion info
	//
	//! @param spd - movement speed ... m/s?
	//! @param dir - heading ... ?
	inline bool getMov(double *spd, double *dir) { 
		if (!(isValidSpd && isValidDir))
			return false;
		if (spd != NULL) *spd=locSpd;
		if (dir != NULL) *dir=locDir;
		return true;
	};

	//! Get GPS time
	//
	// @param utctime - windows system time ... sort of like epoch
	inline bool getTimeData(CTime *utctime) {
		if (!(isValidUTCTime))
			return false;
		if (utctime != NULL) *utctime=utcTime;
		return true;
	}

	friend class CGPSgeneric;

protected:
	//! For use by CGPSgeneric only: set info in object
	//
	//! All values are passed as pointers. If the pointer is NULL it is assumed the value
	//! is not supposed to be altered.
	inline void setInfo(
		const double *lat, const double *lon, const double *alt,
		const double *spd, const double *dir,
		const CTime *utctime,
		const char *devicename,
		const char *deviceid
	) {
		if (lat!=NULL)        { locLat     = *lat;        isValidLat       = true; }
		if (lon!=NULL)        { locLon     = *lon;        isValidLon       = true; }
		if (alt!=NULL)        { locAlt     = *alt;        isValidAlt       = true; }
		if (spd!=NULL)        { locSpd     = *spd;        isValidSpd       = true; }
		if (dir!=NULL)        { locDir     = *dir;        isValidDir       = true; }
		if (utctime!=NULL)    { utcTime    = *utctime;    isValidUTCTime   = true; }

		if (devicename!=NULL) { 
			deviceName.setStr8bit(devicename); 
			isValidDeviceName= true; 
		}
		if (deviceid!=NULL) { 
			deviceID.setStr8bit(deviceid);
			isValidDeviceID  = true; 
		}
	};

	inline void invalidateInfo() {
		isValidLat       = false;
		isValidLon       = false;
		isValidAlt       = false;
		isValidSpd       = false;
		isValidDir       = false;
		isValidUTCTime   = false;
	};

	inline void invalidateDeviceInfo() {
		isValidDeviceName= false; deviceName.setStr8bit("");
		isValidDeviceID  = false; deviceID.setStr8bit("");
	};

private:
	CGPSPortType type;
	CGPS8bitStringContainer deviceName; bool isValidDeviceName;
	CGPS8bitStringContainer portName;
	CGPS8bitStringContainer deviceID; bool isValidDeviceID;

	double locLat;  bool isValidLat;
	double locLon;  bool isValidLon;
	double locAlt;  bool isValidAlt;
	double locSpd;  bool isValidSpd;
	double locDir;  bool isValidDir;
	CTime  utcTime; bool isValidUTCTime;

};

typedef std::vector<CGPSport> CGPSportList;

class CGPSgeneric
{
public:
	CGPSgeneric(void);
	~CGPSgeneric(void);


	//! return a list of available ports
	CGPSportList getPorts(CGPSPortType filter = CGPSPortTypesAll);

	//! select a GPS port using saved info
	//
	//! One or more parameters may be provided to find the best matching
	//! device. Use NULL for unset parameters
	//
	//! @params portname - mostly useful for NMEA serial ports
	//! @params devicename - device info string
	//! @params deviceid - device id
	bool findPort(CGPSport *portinfo,CString *portname, CString *devicename, CString *deviceid);

	//! open a GPS port
	//
	//! Note that only one GPS device can be "open" at a time. Calling open while another
	//! device is open will close that other device and open the new one.
	//
	// @@@ ideally this should return false for COM ports that don't send NMEA data
	// @@@ ideally this should update the port value with information about the device if not already available
	// @@@ ideally this should update the information that getPorts() will return
	bool openPort(CGPSport &select);

	//! return the "currently open" port
	//
	//! This is a convenience function to keep cached GPS data stored in an associated object 
	//! instead of having to pass that object around ...
	inline bool getCurPort(CGPSport &select) { 
		if (isopen) { select=curPort; return true; }
		return false;
	};

	//! close a GPS port
	bool closePort(CGPSport &select);
	inline bool closePort() { return (! isopen) || closePort(curPort); } 

	//! update location info cached in device object
	//
	//! Use of this function for GarXface ports is straightforward: all information will be updated in one call.
	//! For NMEA ports information trickles in slowly and in different chunks. For NMEA data this function will
	//! return when any of these conditions is met:
	//
	//!   (1) all data is considered valid/up to date
	//!   (2) at least one read from the device has taken place and more than maxwait seconds have passed
	//
	//! Due to the way GPS devices communicate, this function can take a long time (multiple seconds for GarXface)
	//! or never return at all (NMEA if the device is having issues getting a proper fix). For this reason it may
	//! be useful to use this sparingly and use the cached values when appropriate.
	//
	//! @param invalidate [false] - if set to true, this will first invalidate previously cached data
	//! @param maxwait [60] - maximum wait time before returning
	//
	bool updateInfo(CGPSport &select,bool invalidate=false,int maxwait=60);
	inline bool updateInfo(bool invalidate=false,int maxwait=60) { 
		if (isopen) { return updateInfo(curPort,invalidate,maxwait); }
		return false;
	};


	//! Query type (see constructor for more details)
	//
	//! This is a convenience wrapper that will update the given port object to new data
	//! and return that data at the same time.
	inline bool getType(CGPSport &select,
						CGPSPortType *type,
						bool update=false, bool invalidate=false, int maxwait=60) {
	   if ((update) && (! this->updateInfo(select,invalidate,maxwait)))
		   return false;
	   CGPSPortType ttype=select.getType();
	   if (type!=NULL)
		   *type=ttype;
	   return (ttype == CGPSPortTypesUnset);
	};
	inline bool getType(CGPSPortType *type,
					   bool update=false, bool invalidate=false, int maxwait=60) {
		if (isopen) { return getType(curPort,type,update,invalidate,maxwait); }
		return false;
	};

	//! Query port_name (see constructor for more details)
	//
	//! This is a convenience wrapper that will update the given port object to new data
	//! and return that data at the same time.
	inline bool getPortName(CGPSport &select,
							CString *portname,
							bool update=false, bool invalidate=false, int maxwait=60) {
		if ((update) && (! this->updateInfo(select,invalidate,maxwait)))
		return false;
		CString tportname=select.getPortName();
		if (portname!=NULL)
			*portname=tportname;
		return (tportname != _T(""));
	};
	inline bool getPortName(CString *portname,
					   bool update=false, bool invalidate=false, int maxwait=60) {
		if (isopen) { return getPortName(curPort,portname,update,invalidate,maxwait); }
		return false;
	};

	//! Query device_name (see constructor for more details)
	//
	//! This is a convenience wrapper that will update the given port object to new data
	//! and return that data at the same time.
	inline bool getDeviceName(CGPSport &select,
							  CString *devicename,
							  bool update=false, bool invalidate=false, int maxwait=60) {
		if ((update) && (! this->updateInfo(select,invalidate,maxwait)))
		return false;
		CString tdevicename=select.getDeviceName();
		if (devicename!=NULL)
			*devicename=tdevicename;
		return (tdevicename != _T(""));
	};
	inline bool getDeviceName(CString *devicename,
					   bool update=false, bool invalidate=false, int maxwait=60) {
		if (isopen) { return getDeviceName(curPort,devicename,update,invalidate,maxwait); }
		return false;
	};

	//! Query device_id (see constructor for more details)
	//
	//! This is a convenience wrapper that will update the given port object to new data
	//! and return that data at the same time.
	inline bool getDeviceID(CGPSport &select,
							CString *deviceid,
							bool update=false, bool invalidate=false, int maxwait=60) {
		if ((update) && (! this->updateInfo(select,invalidate,maxwait)))
		return false;
		CString tdeviceid=select.getDeviceID();
		if (deviceid!=NULL)
			*deviceid=tdeviceid;
		return (tdeviceid != _T(""));
	};
	inline bool getDeviceID(CString *deviceid,
					   bool update=false, bool invalidate=false, int maxwait=60) {
		if (isopen) { return getDeviceID(curPort,deviceid,update,invalidate,maxwait); }
		return false;
	};


	//! Get location
	//
	//! This is a convenience wrapper that will update the given port object to new data
	//! and return that data at the same time.
	inline bool getLoc(CGPSport &select,
		               double *lat, double *lon, double *alt,
					   bool update=false, bool invalidate=false, int maxwait=60) {
	   if ((update) && (! this->updateInfo(select,invalidate,maxwait)))
		   return false;
	   return select.getLoc(lat,lon,alt);
	};

	inline bool getLoc(double *lat, double *lon, double *alt,
					   bool update=false, bool invalidate=false, int maxwait=60) {
		if (isopen) { return getLoc(curPort,lat,lon,alt,update,invalidate,maxwait); }
		return false;
	};

	//! Get GPS time
	//
	//! This is a convenience wrapper that will update the given port object to new data
	//! and return that data at the same time.
	inline bool getTimeData(CGPSport &select,
							CTime *utctime,
							bool update=false, bool invalidate=false, int maxwait=60) {
		if ((update) && (! this->updateInfo(select,invalidate,maxwait)))
			return false;
		return select.getTimeData(utctime);
	};
	inline bool getTimeData(CTime *utctime,
							bool update=false, bool invalidate=false, int maxwait=60) {
		if (isopen) { return getTimeData(curPort,utctime,update,invalidate,maxwait); }
		return false;
	};


	// @@@ AAARGHH ... no no no no ... fix this
	inline bool getGPSInfo(CGPSport &select,
						   CGPSData *gpsinfo,
						   bool update=false, bool invalidate=false, int maxwait=60) 
	{
		if ((update) && (! this->updateInfo(select,invalidate,maxwait)))
			return false;

		double lat,lon,alt;
		double spd,dir;
		CTime utctime;
		CString devicename;

		bool res=(
			select.getLoc(&lat,&lon,&alt) &&
			select.getMov(&spd,&dir) &&
			select.getTimeData(&utctime) &&
			getDeviceName(&devicename,false,false,0));

		if (res) {
			CGPSData tdata(&lat,&lon,&alt,&spd,&dir,&utctime,&devicename);
			if (gpsinfo != NULL) 
				*gpsinfo=tdata;
			return true;
		}
		return false;
	};
	inline bool getGPSInfo(CGPSData *gpsinfo,
						   bool update=false, bool invalidate=false, int maxwait=60) {
		if (isopen) { return getGPSInfo(curPort,gpsinfo,update,invalidate,maxwait); }
		return false;
	};
private:

#if USE_GARXFACE == 1
	GarXface4::Gps garGPS;
	GarXface4::ComPortNameList garCOMList;
	GarXface4::UsbDeviceNameList garUSBList;

#if GARXFACE_HAS_SCOPE_ISSUES == 1
	// Version 4.0 has some pointer scoping issues where deleting the info object
	// will lead to crashes if the Gps class is subsequently used on the "open"
	// device. To avoid that, hold on to those info objects 

	GarXface4::ComPortNamePtr cpn;
	GarXface4::GpsComPortDevice comDevice;

	GarXface4::UsbDeviceNamePtr upn;
	GarXface4::GpsUsbDevice usbDevice;

#endif
#endif

#if USE_RPNMEA == 1
	CRPGarmCom garNMEA;
#endif

	bool isopen;
	CGPSport curPort;

};

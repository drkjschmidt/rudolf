#pragma once

#include <serialport.h>

# define GARM_SER_BUF 4096
# define GARM_NAM_BUF 256

#define CGPS_ALT_ILLEGAL 36000000 // illegal altitude (geostationary orbit)
#define CGPS_LAT_ILLEGAL 255 // illegal lat / lon (should be +-128)
#define CGPS_LON_ILLEGAL 255

class CGPSTime;

//! Manipulate time stamps as used by NMEA / GPS
//
//! NMEA time is reported as an int32 value in human-readable
//! hh-mm-ss format (000000 to 235959)
class CGPSTime
{
public:
	CGPSTime(void);
	~CGPSTime(void);

	//! directly set int32 time stamp
	inline void setTimeInt(INT32 set) { GPStime=set; }

	//! retrieve int32 time stamp
	inline INT32 getTimeInt(void) { return GPStime; }

	//! retrieve hh mm ss values of timestamp
	inline void getHMS(int *hh, int *mm, int *ss) {
			*hh=(int)(GPStime/3600);
			*mm=(int)((GPStime - *hh*3600)/60);
			*ss=GPStime - *hh*3600 - *mm*60;
	}

	//! compare timestamps 
	//
	// why is this so complicated ... shouldn't simple comparison of int32 values do the trick?
	inline bool operator<(CGPSTime &right) {
		int lh,rh,lm,rm,ls,rs;
		this->getHMS(&lh,&lm,&ls);
		right.getHMS(&rh,&rm,&rs);

		if ( (lh<rh) || ((lh==rh)&&( (lm<rm) || ((lm==rm)&&(ls<rs)) )))
			return true;
		return false;
	}

	//! compare timestamps 
	//
	// why is this so complicated ... shouldn't simple comparison of int32 values do the trick?
	inline bool operator>(CGPSTime &right) {
		int lh,rh,lm,rm,ls,rs;
		this->getHMS(&lh,&lm,&ls);
		right.getHMS(&rh,&rm,&rs);

		if ( (lh>rh) || ((lh==rh)&&( (lm>rm) || ((lm==rm)&&(ls>rs)) )))
			return true;
		return false;
	}

	//! compare timestamps
	inline bool operator==(CGPSTime &right) {
		return this->getTimeInt() == right.getTimeInt();
	}

protected:
	INT32 GPStime;
};

//! Manipulate date stamps as used by NMEA / GPS
//
//! NMEA date is reported as an int32 value in human-readable
//! dd-mm-yy format (010100 to 311299) ... considering the age
//! of NMEA there should be a rollover from 1999 to 2000 and
//! maybe another from 2099 to 2100 but since this code is 
//! written in 2009/2010 neither of these should bother us!
class CGPSDate
{
public:
	CGPSDate(void);
	~CGPSDate(void);

	//! directly set int32 date stamp
	inline void setDateInt(INT32 set) { GPSdate=set; }

	// retrieve int32 date stamp
	inline INT32 getDateInt(void) { return GPSdate; }

	// retrieve dd mm yy values of datestamp 
	inline void getDMY(int *dd, int *mm, int *yy) {
			*dd=(int)(GPSdate / 10000);
			*mm=(int)((GPSdate - *dd*10000)/100);
			*yy=GPSdate - *dd*10000 - *mm*100 + 2000;
	}

	//! compare datestamps
	//
	// as opposed to time stamps, this one does need to
	// be so complicated because the most significant number
	// is last ... 
	inline bool operator<(CGPSDate &right) {
		int ld,rd,lm,rm,ly,ry;
		this->getDMY(&ld,&lm,&ly);
		right.getDMY(&rd,&rm,&ry);

		if ( (ly<ry) || ((ly==ry)&&( (lm<rm) || ((lm==rm)&&(ld<rd)) )))
			return true;
		return false;
	}

	//! compare datestamps
	//
	// as opposed to time stamps, this one does need to
	// be so complicated because the most significant number
	// is last ... 
	inline bool operator>(CGPSDate &right) {
		int ld,rd,lm,rm,ly,ry;
		this->getDMY(&ld,&lm,&ly);
		right.getDMY(&rd,&rm,&ry);

		if ( (ly>ry) || ((ly==ry)&&( (lm>rm) || ((lm==rm)&&(ld>rd)) )))
			return true;
		return false;
	}

	//! compare datestamps
	inline bool operator==(CGPSDate &right) {
		return this->getDateInt() == right.getDateInt();
	}


protected:
	INT32 GPSdate;
};


//! Store and manipulate GPS data (time, date, location)
class CGPSData 
{
public:
	CGPSData(void);
	CGPSData(double *lat, double *lon, double *alt, 
		double *spd, double *dir,
		INT32 *time, INT32 *date,
		char *name);
	CGPSData(double *lat, double *lon, double *alt, 
		double *spd, double *dir,
		INT32 *xtime, INT32 *xdate,
		CString *name);
	~CGPSData(void);

	//! Store data in class
	//
	//! All parameters are passed as pointers and a NULL pointer
	//! value indicates that no valid data for this parameter is 
	//! known / being supplied.
	//
	//! @param lat - latitude [deg], negative values S
	//! @param lon - longitude [deg], negative values W
	//! @param alt - altitude [m]
	//! @param spd - speed [knots?]
	//! @param dir - direction [deg?]
	//! @param xtime - time [int32 = hhmmss]
	//! @param xdate - date [int32 = ddmmyy]
	//! @param name - GPS name
	//
	void setData(double *lat, double *lon, double *alt, 
		double *spd, double *dir,
		INT32 *xtime, INT32 *xdate,
		CString *name);

	//! Store data in class
	//
	//! Wrapper function. Only difference to previous function is
	//! GPS name in char* format
	void setData(double *lat, double *lon, double *alt, 
		double *spd, double *dir,
		INT32 *xtime, INT32 *xdate,
		char *name);


	//! Retrieve latitude [deg, S<0] ... return false if not known
	inline bool   getLat(double *lat) { if (lat != NULL) { *lat = loc_lat; } return val_lat; } 

	//! Retrieve latitude [deg, S<0] ... return 255 if not known
	inline double getLat(void)        { if (val_lat) return loc_lat; else return CGPS_LAT_ILLEGAL; }

	//! Retrieve longitude [deg, W<0] ... return false if not known
	inline bool   getLon(double *lon) { if (lon != NULL) { *lon = loc_lon; } return val_lon; } 

	//! Retrieve longitude [deg, W<0] ... return 255 if not known
	inline double getLon(void)        { if (val_lon) return loc_lon; else return CGPS_LON_ILLEGAL; }

	//! Retrieve altitude [m] ... return false if not known
	inline bool   getAlt(double *alt) { if (alt != NULL) { *alt = loc_alt; } return val_alt; } 

	//! Retrieve altitude [m] ... return 36000000 if not known
	inline double getAlt(void)        { if (val_alt) return loc_alt; else return CGPS_ALT_ILLEGAL; }

	//! Retrieve location: latitude [deg, S<0], longitude [deg, W<0], and altitude [m] 
	//! ... return false if *any* value not known
	inline bool getLoc(double *lat, double *lon, double *alt) {
		return getLat(lat)&getLon(lon)&getAlt(alt);
	}


	//! Retrieve speed [knots?] ... return false if not known
	inline bool getSpd(double *spd) { if (spd != NULL) { *spd = mov_spd; } return val_spd; } 

	//! Retrieve direction [deg?] ... return false if not known
	inline bool getDir(double *dir) { if (dir != NULL) { *dir = mov_dir; } return val_dir; } 

	//! Retrieve movement vector: speed [knots?], and direction [deg?] 
	//! ... return false if *any* value not known
	inline bool getMov(double *spd, double *dir) {
		return getSpd(spd)&getDir(dir);
	}


	//! Retrieve time [int32 = hhmmss] ... return false if not known
	inline bool getTime(INT32 *time) { if (time != NULL) { *time = gps_time; } return val_time; } 

	//! Retrieve time [int32 = hhmmss] ... return -1 if not known
	inline INT32 getTime(void)       { if (val_time) return gps_time; else return -1; }

	//! Retrieve time [CGPStime] ... there is no validity check?!?
	// @@@ // need validity check?
	inline CGPSTime getGPSTime(void) { CGPSTime t; t.setTimeInt(gps_time); return t; }

	//! Retrieve date [int32 = ddmmyy] ... return false if not known
	inline bool getDate(INT32 *date) { if (date != NULL) { *date = gps_date; } return val_date; } 

	//! Retrieve date [int32 = ddmmyy] ... return -1 if not known
	inline INT32 getDate(void)       { if (val_date) return gps_date; else return -1; }

	//! Retrieve date [CGPSdate] ... there is no validity check?!?
	// @@@ // need validity check?
	inline CGPSDate getGPSDate(void) { CGPSDate t; t.setDateInt(gps_date); return t; }

	//! Retrieve time and date: time [int32 = hhmmss], date [int32 = ddmmyy] 
	//! ... return false if *any* value not known
	inline bool getTimeDate(INT32 *time, INT32 *date) {
		return getTime(time)&getDate(date);
	}

	//! Retrieve gps name ... return false if not known
	inline bool getName(CString *name) { if (name != NULL) { *name = gps_name; } return val_name; } 


	//! Output format flags for seriazlize function
	enum SerializeModes {
		outputCSV =   (unsigned char) 0x01,
		outputXML =   (unsigned char) 0x02,

		outputHDR =   (unsigned char) 0x80, // only valid in combination with outputCSV
	};

	//! Output GPS data in CSV or XML format (don't think XML is implemented)
	CString serialize(unsigned char mode);

	//! Output GPS location as formatted string: 
	//! "[deg][N/S] [deg][E/W] [m][m] "
	//! Unknown lat/lon/alt values will be replaced with "??"
	//
	inline CString formatLoc() {
		CString tval;

		tval.Empty();
		if (val_lat) {
			tval.AppendFormat(_T("%.6f%s "),
			(loc_lat>0)?(loc_lat):(-loc_lat), (loc_lat>0)?(_T("N")):(_T("S")));
		} else {
			tval.Append(_T("??N "));
		}

		if (val_lon) {
			tval.AppendFormat(_T("%.6f%s "),
			(loc_lon>0)?(loc_lon):(-loc_lon), (loc_lon>0)?(_T("E")):(_T("W")));
		} else {
			tval.Append(_T("??W "));
		}

		if (val_alt) {
			tval.AppendFormat(_T("%.1fm "), loc_alt);
		} else {
			tval.Append(_T("??m "));
		}

		return tval;
	}

	//! Output GPS time (UTC) as formatted string: 
	//! "hh:mm:ss UTC "
	//! Unknown time will be printed as "Unknown time "
	//
	inline CString formatTime() {
		CString tval;
		INT32 hh,mm,ss;

		if (val_time) {
			hh=(int)(gps_time/3600);
			mm=(int)((gps_time - hh*3600)/60);
			ss=gps_time - hh*3600 - mm*60;

			tval.Format(_T("%02d:%02d:%02d UTC "),hh,mm,ss);
		} else {
			tval=_T("Unknown time ");
			// tval=_T("??:??:?? UTC ");
		}

		return tval;
	}

	//! Output GPS date as formatted string (note that this date is based on GPS UTC clock): 
	//! "yyyy-mm-dd " with the assumption that the recorded year is between 2000 and 2099
	//! Unknown date will be printed as "Unknown date "
	//
	inline CString formatDate() {
		CString tval;
		INT32 dd,mm,yy;

		if (val_date) {
			dd=(int)(gps_date / 10000);
			mm=(int)((gps_date - dd*10000)/100);
			yy=gps_date - dd*10000 - mm*100 + 2000;

			tval.Format(_T("%04d-%02d-%02d "),yy,mm,dd);
		} else {
			tval=_T("Unknown date ");
			// tval=_T("????-??-?? ");
		}

		return tval;
	}

	//! Invalidate some or all stored settings. By default all settings are invaldated
	inline void invalidateData(
		bool inv_lat =true,
		bool inv_lon =true,
		bool inv_alt =true,
		bool inv_spd =true,
		bool inv_dir =true,
		bool inv_time=true,
		bool inv_date=true,
		bool inv_name=true
	) {
		if (inv_lat ) val_lat  = false;
		if (inv_lon ) val_lon  = false;
		if (inv_alt ) val_alt  = false;
		if (inv_spd ) val_spd  = false;
		if (inv_dir ) val_dir  = false;
		if (inv_time) val_time = false;
		if (inv_date) val_date = false;
		if (inv_name) val_name = false;
	}

protected:
	// lat / lon are in degrees ... note that this is different from the Spanner
	// response which is in some sort of DDDDMM.SSSSS format.
	double loc_lat, loc_lon, loc_alt;
	bool val_lat, val_lon, val_alt;

	// movement speed and direction as reported by GPS
	double mov_spd, mov_dir;
	bool val_spd, val_dir;

	// UTC Time in seconds since midnight 
	INT32 gps_time;
	bool val_time;
	// UTC Date in ... DDMMYY?
	INT32 gps_date;
	bool val_date;

	// GPS name / info
	CString gps_name;
	bool val_name;
};

//! GPS class ... currently this only allows interacting with
//! a serial port GPS sending continuous NMEA data
class CRPGarmCom
{
public:
	CRPGarmCom(void);
	~CRPGarmCom(void);

public:
	//! list available ports (currently only COM ports are supported)
	bool getGarminPorts(CStringArray &ports);

	//! list available COM ports
	bool getComPorts(CUIntArray &portnos);

	//! get the currently selected com port
	CString getComPort(void);

	//! set the port on which to operate (currently only COM ports are supported)
	inline bool setGarminPort(CString &port) { return setComPort(port); }

	//! set the com port on which to operate
	bool setComPort(CString &port);

	//! open previously selected port for reading (currently only COM ports are supported)
	inline bool openGarminPort(void) { return openComPort(); }
	//! open previously selected port for reading ... with default Spanner values
	bool openComPort(void);

	//! close the port (currently only COM ports are supported)
	inline void closeGarminPort(void) { return closeComPort(); }

	//! close the COM port
	void closeComPort(void);

	//! read data from COM port
	int readComPort(void);

	//! check whether we seem to have a valid GPS
	//
	//! This function will only return meaningful data after a 
	//! successful checksum is found, so a call to updataInfo preceding
	//! this function is advised!
	inline bool isValid() { return is_valid; }

	//! invalidate stored info selectively. By default *all* data will be invalidated
	inline void invalidateInfo(
		bool inv_lat =true,
		bool inv_lon =true,
		bool inv_alt =true,
		bool inv_spd =true,
		bool inv_dir =true,
		bool inv_time=true,
		bool inv_date=true,
		bool inv_name=true
	) {
		curData.invalidateData(inv_lat,inv_lon,inv_alt,inv_spd,inv_dir,inv_time,inv_date,inv_name);
	}


	//! update location info
	//
	//! This will read *one* set of GPS info and can be used to invalidate data before reading
	//! @param invalidate [false] - if set to true, this will first invalidate previously stored data
	bool updateInfo(bool invalidate=false);

	inline char* getIObuf(void) { return iobuf; }

	inline bool getLoc(double *lat, double *lon, double *alt) {
		return curData.getLoc(lat,lon,alt);
	}
	inline bool getMov(double *spd, double *dir) {
		return curData.getMov(spd,dir);
	}
	inline bool getTimeData(INT32 *time, INT32 *date) {
		return curData.getTimeDate(time,date);
	}
	inline bool getName(CString *xname) { 
		return curData.getName(xname);
	}

	// we need a way to tell whether this data is still valid ...
	CTime lastGPRMC,lastGPGGA,lastPGRMT;

	inline CString cser(unsigned char mode) { return curData.serialize(mode); };
	inline CString formatLoc()  { return curData.formatLoc(); }
	inline CString formatTime() { return curData.formatTime(); }
	inline CString formatDate() { return curData.formatDate(); }

	inline CGPSData getGPSInfo() { return curData; }

protected:
	CString comPort;
	CSerialPort comPortCtrl;

	CGPSData curData;

/*
	// lat / lon are in degrees ... note that this is different from the Spanner
	// response which is in some sort of DDDDMM.SSSSS format.
	double loc_lat, loc_lon, loc_alt;

	// movement speed and direction as reported by GPS
	double mov_spd, mov_dir;

	// UTC Time in seconds since midnight 
	INT32 time;
	// UTC Date in ... DDMMYY?
	INT32 date;

	// GPS name / info
	char name[GARM_NAM_BUF];
*/

	bool check_csum(const char *temptoks, unsigned char *return_cs=NULL);
	//! tokenize GPS sentences
	//
	//! strtok() will skip multiple delimiters at once. We need a single delimiter
	//! at a time
	char *strstok(char *s, const char *delim);
	char *strstok_r(char *s, const char *delim, char **lasts);

	bool parseGPRMC(const char *temptoks,
		INT32 *time,INT32 *date,
		double *loc_lat,double *loc_lon, 
		double *mov_spd,double *mov_dir);
	bool parseGPGGA(const char *temptoks,
		INT32 *time,
		double *loc_lat,double *loc_lon,double *loc_alt);
	bool parsePGRMT(const char *temptoks,char *name);

private:
	char iobuf[GARM_SER_BUF];
	double garloc_to_realloc(CString* garloc);
	bool is_open; // keep track of open / close operations
	bool is_valid; // *guess* at validity of port ... 


	
};

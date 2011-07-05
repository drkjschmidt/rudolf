#pragma once

#include <serialport.h>

# define GARM_SER_BUF 4096

class CRPGarmCom
{
public:
	CRPGarmCom(void);
	~CRPGarmCom(void);

public:
	CString getComPort(void);
	bool setComPort(CString port);

	bool openComPort(void);
	void closeComPort(void);
	int readComPort(void);

	bool updateInfo(void);

	inline char* getIObuf(void) { return iobuf; }
	inline void getloc(double * lat, double * lon, double * alt) {
		*lat = loc_lat; *lon = loc_lon; *alt = loc_alt; }

	// we need a way to tell whether this data is still valid ...
	CTime lastGPRMC,lastGPGGA,lastPGRMT;

protected:
	CString comPort;
	CSerialPort comPortCtrl;

	// lat / lon are in degrees ... note that this is different from the Spanner
	// response which is in some sort of DDDDMM.SSSSS format.
	double loc_lat, loc_lon, loc_alt;

	// movement speed and direction as reported by GPS
	double mov_spd, mov_dir;

	// UTC Time in seconds since midnight 
	INT32 time;
	// UTC Date in ... DDMMYY?
	INT32 date;


	bool check_csum(const char *temptoks);
	bool parseGPRMC(const char *temptoks,
		INT32 *time,INT32 *date,
		double *loc_lat,double *loc_lon, 
		double *mov_spd,double *mov_dir);
	bool parseGPGGA(const char *temptoks,
		INT32 *time,
		double *loc_lat,double *loc_lon,double *loc_alt);

private:
	char iobuf[GARM_SER_BUF];
	double garloc_to_realloc(CString* garloc);


	
};

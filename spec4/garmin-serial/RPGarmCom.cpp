#include "StdAfx.h"
#include "RPGarmCom.h"
#include <serialport.h>
#include <enumser.h>

CGPSTime::CGPSTime()
: GPStime(0)
{
}

CGPSTime::~CGPSTime()
{
}

CGPSDate::CGPSDate()
: GPSdate(0)
{
}

CGPSDate::~CGPSDate()
{
}




CGPSData::CGPSData(void)
: loc_lat(0), loc_lon(0), loc_alt(0), mov_spd(0), mov_dir(0), gps_time(0), gps_date(0)
, val_lat(0), val_lon(0), val_alt(0), val_spd(0), val_dir(0), val_time(0), val_date(0), val_name(0)
{
}

CGPSData::CGPSData(double *lat, double *lon, double *alt, 
				   double *spd, double *dir,
				   INT32 *time, INT32 *date,
				   CString *name)
: loc_lat(0), loc_lon(0), loc_alt(0), mov_spd(0), mov_dir(0), gps_time(0), gps_date(0)
, val_lat(0), val_lon(0), val_alt(0), val_spd(0), val_dir(0), val_time(0), val_date(0), val_name(0)
{
	setData(lat,lon,alt,spd,dir,time,date,name);
}

CGPSData::CGPSData(double *lat, double *lon, double *alt, 
				   double *spd, double *dir,
				   INT32 *time, INT32 *date,
				   char *name)
: loc_lat(0), loc_lon(0), loc_alt(0), mov_spd(0), mov_dir(0), gps_time(0), gps_date(0)
, val_lat(0), val_lon(0), val_alt(0), val_spd(0), val_dir(0), val_time(0), val_date(0), val_name(0)
{
	setData(lat,lon,alt,spd,dir,time,date,name);
}

CGPSData::~CGPSData(void)
{
}


void CGPSData::setData(double *lat, double *lon, double *alt, 
				  double *spd, double *dir,
				  INT32 *time, INT32 *date,
				  CString *name)
{
	if (lat != NULL) { loc_lat = *lat; val_lat = true; }
	if (lon != NULL) { loc_lon = *lon; val_lon = true; }
	if (alt != NULL) { loc_alt = *alt; val_alt = true; }

	if (spd != NULL) { mov_spd = *spd; val_spd = true; }
	if (dir != NULL) { mov_dir = *dir; val_dir = true; }

	if (time != NULL) { gps_time = *time; val_time = true; }
	if (date != NULL) { gps_date = *date; val_date = true; }
	if (name != NULL) { gps_name = *name; val_name = true; }
}

void CGPSData::setData(double *lat, double *lon, double *alt, 
				  double *spd, double *dir,
				  INT32 *time, INT32 *date,
				  char *name)
{
	CString cname;
	CString *cptr;

	if (name != NULL) { 
		cname=name;
		// cname.Format(_T("%s"),name);
		cptr=&cname;
	}
	else {
		cptr=NULL;
	}
	setData(lat,lon,alt,spd,dir,time,date,cptr);
}

CString CGPSData::serialize(unsigned char mode)
{
	CString tval(_T(""));

	if (mode & this->outputCSV)
	{
		if (mode & this->outputHDR)
		{
			tval=_T("Latitude [deg],Longitude [deg],Altitude [m],Speed [knots],Direction [deg],Time [s],Date [DDMMYY],GPS Name");
		}
		else 
		{
			if (val_lat) { tval.AppendFormat(_T("%g"), loc_lat); } else { tval.AppendFormat(_T(""));  }
			if (val_lon) { tval.AppendFormat(_T(",%g"),loc_lon); } else { tval.AppendFormat(_T(",")); }
			if (val_alt) { tval.AppendFormat(_T(",%g"),loc_alt); } else { tval.AppendFormat(_T(",")); }

			if (val_spd) { tval.AppendFormat(_T(",%g"),mov_spd); } else { tval.AppendFormat(_T(",")); }
			if (val_dir) { tval.AppendFormat(_T(",%g"),mov_dir); } else { tval.AppendFormat(_T(",")); }

			if (val_time) { tval.AppendFormat(_T(",%d"),gps_time); } else { tval.AppendFormat(_T(",")); }
			if (val_date) { tval.AppendFormat(_T(",%d"),gps_date); } else { tval.AppendFormat(_T(",")); }

			if (val_name) { tval.AppendFormat(_T(",%s"),gps_name.GetString()); } else { tval.AppendFormat(_T(",")); }
		}
	} 
	else if (mode & this->outputXML)
	{
		// currently not supported
	}

	return tval;
}



CRPGarmCom::CRPGarmCom(void)
: comPort(_T(""))
, lastGPRMC(0)
, lastGPGGA(0)
, lastPGRMT(0)
, is_open(false)
, is_valid(false)
{
}

CRPGarmCom::~CRPGarmCom(void)
{
	if (! comPort.IsEmpty())
		this->closeComPort();
}

bool 
CRPGarmCom::getGarminPorts(CStringArray &ports)
{
	CUIntArray portnos;
	CString tstr;
	int ii;

	getComPorts(portnos);

	for (ii=0; ii<portnos.GetSize(); ii++)
	{
		tstr.Format(_T("COM%d"),portnos[ii]);
		ports.Add(tstr);
	}
	return true;
}

bool 
CRPGarmCom::getComPorts(CUIntArray &ports)
{
	return (bool)(CEnumerateSerial::UsingGetDefaultCommConfig(ports));
}

CString CRPGarmCom::getComPort(void)
{
	return comPort;
}

bool CRPGarmCom::setComPort(CString &port)
{
	if (comPort != port)
		closeComPort();

	comPort=port;
	return true;
}

bool CRPGarmCom::openComPort(void)
{
	if (comPort.IsEmpty())
		return false;

	if (comPortCtrl.IsOpen())
		return true;

	comPortCtrl.Open(
		comPort.GetString(),
		4800,
		comPortCtrl.NoParity,
		8,
		comPortCtrl.OneStopBit,
		comPortCtrl.NoFlowControl,
		false);

	if (comPortCtrl.IsOpen()) {
		is_open=true;
		is_valid=false; // @@@ paranoia?
		return true;
	}

	is_open=false;
	is_valid=false;
	return false;
}

void CRPGarmCom::closeComPort(void)
{
	is_open=false;
	is_valid=false;
	comPortCtrl.Close();
}

int CRPGarmCom::readComPort(void)
{
	int res;

	if (! comPortCtrl.IsOpen())
		return -1;
	comPortCtrl.Set0ReadTimeout();
	res=comPortCtrl.Read(iobuf,GARM_SER_BUF);
	
	return res;

}


double CRPGarmCom::garloc_to_realloc(CString* garloc)
{
	return 0;
}

bool CRPGarmCom::check_csum(const char *temptoks, unsigned char *return_cs)
{
	size_t ii=0;

	unsigned char ac=0;
	unsigned char ch,cs;
	char *dummy;

	if (temptoks[ii++] != '$')
		return false;
	
	do {
		ch=(unsigned char)temptoks[ii++];
		if (ch == '*')
			break;
		ac=ac^ch;
	}
	while (ii<strlen(temptoks));

	cs=(unsigned char)strtoul(&(temptoks[ii]),&dummy,16);


	if (cs != ac)
		return false;
	if (return_cs != NULL)
		*return_cs=cs;

	// if we found a valid checksum, we know we have a valid GPS :)
	is_valid=true;
	return true;
}

// Freely based on:
//  strtok_r from libc http://www.koders.com/c/fid3BA8D3CFDF83A080B6FC22683C4EDB5D4CD270FE.aspx
//  strtok from MS https://research.microsoft.com/en-us/um/redmond/projects/invisible/src/crt/strtok.c.htm
char *CRPGarmCom::strstok(char *s, const char *delim) 
{
	static char *lasts = NULL;
	return strstok_r(s,delim,&lasts);
}
char *CRPGarmCom::strstok_r(char *s, const char *delim, char **lasts)
{
	// make sure we have char* in s otherwise return
	if (s == NULL && (s = *lasts) == NULL)
		return (NULL);

	// here we *don't* skip delimiters

	// find first delimiter
	*lasts=strpbrk(s,delim);
	if (*lasts) {
		**lasts='\0';
		(*lasts)++;
	} 
	else {
		*lasts=NULL;
	}
	return s;
}


// GPRMC sentence: http://www.gpsinformation.org/dale/nmea.htm#RMC
//
// $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
// 
// Where:
// 0     RMC          Recommended Minimum sentence C
// 1     123519       Fix taken at 12:35:19 UTC
// 2     A            Status A=active or V=Void.
// 3,4   4807.038,N   Latitude 48 deg 07.038' N
// 5,6    01131.000,E  Longitude 11 deg 31.000' E
// 7     022.4        Speed over the ground in knots
// 8     084.4        Track angle in degrees True
// 9     230394       Date - 23rd of March 1994
// 10,11 003.1,W      Magnetic Variation
// 12    *6A          The checksum data, always begins with *
//
bool CRPGarmCom::parseGPRMC(const char *temptoks,
							INT32 *time,INT32 *date,
							double *loc_lat,double *loc_lon, 
							double *mov_spd,double *mov_dir)
{
	char *token,wbuf[GARM_SER_BUF];
	int ii;

	int tcnt=0;
	int tidx;
	char tbuf[6];

	unsigned char cs1,cs2;
	char *dummy;

	if (strncmp(temptoks,"$GPRMC",6))
		return false;
	if (! check_csum(temptoks,&cs1))
		return false;

	strncpy_s(wbuf,temptoks,GARM_SER_BUF);

	token=strstok(wbuf,",*");
	do {
		switch (tcnt++) {
			case 0: // command ... ignore
				break;

			case 1: // time

				// hours
				tbuf[2]='\0';
				tbuf[0]=token[0];
				tbuf[1]=token[1];
				*time=atol(tbuf)*3600;

				// minutes
				tbuf[0]=token[2];
				tbuf[1]=token[3];
				*time += atol(tbuf)*60;

				// hours
				tbuf[0]=token[4];
				tbuf[1]=token[5];
				*time += atol(tbuf);
				break;

			case 2: // active / void ... should we use this? 
					// http://gpsd.berlios.de/NMEA.txt suggests that this indicates a bad fix
					// so maybe we should just discard the whole sentence?
				break;

			case 3: // latitude ... I am assuming [+-]D{1-3}M{2}.FM{1-4}
				if (strlen(token)) {
					tidx=0;
					while (token[tidx] != '.')
						tidx++;
					if (tidx>2 && tidx<7) {
						for (ii=0; ii<tidx-2; ii++)
							tbuf[ii]=token[ii];
						tbuf[tidx-2]='\0';
						*loc_lat = atof(tbuf);
						*loc_lat += atof(&(token[tidx-2]))/60;
					}
				}
				else
				{
					*loc_lat=CGPS_LAT_ILLEGAL;
				}
				break;

			case 4: // N or S ... just change sign if S
				if (strlen(token)) {
					if (token[0] == 'S')
						*loc_lat *= -1;
				}
				else
				{
					*loc_lat=CGPS_LAT_ILLEGAL;
				}
				break;

			case 5: // longitude ... I am assuming [+-]D{1-3}M{2}.FM{1-4}
				if (strlen(token)) {
					tidx=0;
					while (token[tidx] != '.')
						tidx++;
					if (tidx>2 && tidx<7) {
						for (ii=0; ii<tidx-2; ii++)
							tbuf[ii]=token[ii];
						tbuf[tidx-2]='\0';
						*loc_lon = atof(tbuf);
						*loc_lon += atof(&(token[tidx-2]))/60;
					}
				}
				else
				{
					*loc_lon=CGPS_LON_ILLEGAL;
				}
				break;

			case 6: // E or W ... just change sign if W
				if (strlen(token)) {
					if (token[0] == 'W')
						*loc_lon *= -1;
				}
				else
				{
					*loc_lon=CGPS_LON_ILLEGAL;
				}
				break;

			case 7: // speed in knots
				*mov_spd = atof(token);
				break;

			case 8: // direction of movement
				*mov_dir = atof(token);
				break;

			case 9:	// date
				*date = atol(token);
				break;

			case 10: // magnetic variation ... I don't think we need this
			case 11: // magnetic variation ... I don't think we need this
				break;

			case 12: // checksum ... make sure that we are really on the right token ...
				cs2=(unsigned char)strtoul(token,&dummy,16);
				if (cs1 != cs2)
					return false;
				break;

			default:
				break;
		
		}
		token=strstok(NULL,",*");
	} 
	while (token !=NULL);

	return true;
}

// GPGGA sentence: http://www.gpsinformation.org/dale/nmea.htm#GGA
//
// $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
//
// Where:
// 0     GGA          Global Positioning System Fix Data
// 1     123519       Fix taken at 12:35:19 UTC
// 2,3   4807.038,N   Latitude 48 deg 07.038' N
// 4,5   01131.000,E  Longitude 11 deg 31.000' E
// 6     1            Fix quality: 0 = invalid
//                                 1 = GPS fix (SPS)
//                                 2 = DGPS fix
//                                 3 = PPS fix
//                                 4 = Real Time Kinematic
//                                 5 = Float RTK
//                                 6 = estimated (dead reckoning) (2.3 feature)
//                                 7 = Manual input mode
//                                 8 = Simulation mode
// 7     08           Number of satellites being tracked
// 8     0.9          Horizontal dilution of position
// 9,10  545.4,M      Altitude, Meters, above mean sea level
// 11,12 46.9,M       Height of geoid (mean sea level) above WGS84
//                       ellipsoid
// 13     (empty field) time in seconds since last DGPS update
// 14     (empty field) DGPS station ID number
// 15     *47          the checksum data, always begins with *
//
bool CRPGarmCom::parseGPGGA(const char *temptoks,
							INT32 *time,
							double *loc_lat,double *loc_lon,double *loc_alt)
{
	char *token,wbuf[GARM_SER_BUF];
	int ii;

	int tcnt=0;
	int tidx;
	char tbuf[6];

	unsigned char cs1,cs2;
	char *dummy;

	if (strncmp(temptoks,"$GPGGA",6))
		return false;
	if (! check_csum(temptoks,&cs1))
		return false;

	strncpy(wbuf,temptoks,GARM_SER_BUF);

	token=strstok(wbuf,",*");
	do {
		switch (tcnt++) {
			case 0: // command ... ignore
				break;

			case 1: // time

				// hours
				tbuf[2]='\0';
				tbuf[0]=token[0];
				tbuf[1]=token[1];
				*time=atol(tbuf)*3600;

				// minutes
				tbuf[0]=token[2];
				tbuf[1]=token[3];
				*time += atol(tbuf)*60;

				// hours
				tbuf[0]=token[4];
				tbuf[1]=token[5];
				*time += atol(tbuf);
				break;

			case 2: // latitude ... I am assuming [+-]D{1-3}M{2}.FM{1-4}
				if (strlen(token)) {
					tidx=0;
					while (token[tidx] != '.')
						tidx++;
					if (tidx>2 && tidx<7) {
						for (ii=0; ii<tidx-2; ii++)
							tbuf[ii]=token[ii];
						tbuf[tidx-2]='\0';
						*loc_lat = atof(tbuf);
						*loc_lat += atof(&(token[tidx-2]))/60;
					}
				}
				else
				{
					*loc_lat=CGPS_LAT_ILLEGAL;
				}
				break;

			case 3: // N or S ... just change sign if S
				if (strlen(token)) {
					if (token[0] == 'S')
						*loc_lat *= -1;
				}
				else
				{
					*loc_lat=CGPS_LAT_ILLEGAL;
				}
				break;

			case 4: // longitude ... I am assuming [+-]D{1-3}M{2}.FM{1-4}
				if (strlen(token)) {
					tidx=0;
					while (token[tidx] != '.')
						tidx++;
					if (tidx>2 && tidx<7) {
						for (ii=0; ii<tidx-2; ii++)
							tbuf[ii]=token[ii];
						tbuf[tidx-2]='\0';
						*loc_lon = atof(tbuf);
						*loc_lon += atof(&(token[tidx-2]))/60;
					}
				}
				else
				{
					*loc_lat=CGPS_LON_ILLEGAL;
				}
				break;

			case 5: // E or W ... just change sign if W
				if (strlen(token)) {
					if (token[0] == 'W')
						*loc_lon *= -1;
				}
				else
				{
					*loc_lat=CGPS_LON_ILLEGAL;
				}
				break;

			case 6: // fix quality ... should we suppress results on 0?
				break;

			case 7: // num satellites
				break;

			case 8: // horizontal dilution
				break;

			case 9: // altitude ... assume meters
				if (strlen(token)) {
					*loc_alt = atol(token);
				}
				else
				{
					*loc_alt=CGPS_ALT_ILLEGAL;
				}
				break;

			case 10: // unit of altitude ... ft?
				if (strlen(token)) {
					if (token[0] == 'F')
						*loc_alt *= 0.3;
				}
				else
				{
					*loc_alt=CGPS_ALT_ILLEGAL;
				}
				break;

			case 11: // altitude of geoid ... do we need the accuracy?
				break;

			case 12: // units of geoid altitude
				break;

			case 15: // checksum ... make sure that we are really on the right token ...
				cs2=(unsigned char)strtoul(token,&dummy,16);
				if (cs1 != cs2)
					return false;
				break;

			default:
				break;
		
		}
		token=strstok(NULL,",*");
	} 
	while (token !=NULL);

	return true;

}

// Garmin proprietary: http://www8.garmin.com/support/pdf/NMEA_0183.pdf
bool CRPGarmCom::parsePGRMT(const char *temptoks,char *name)
{
	char *token,wbuf[GARM_SER_BUF];

	int tcnt=0;

	unsigned char cs1,cs2;
	char *dummy;

	if (strncmp(temptoks,"$PGRMT",6))
		return false;
	if (! check_csum(temptoks,&cs1))
		return false;

	strncpy(wbuf,temptoks,GARM_SER_BUF);

	token=strstok(wbuf,",*");
	do {
		switch (tcnt++) {
			case 0: // command ... ignore
				break;

			case 1: // name etc.
				strncpy(name,token,GARM_NAM_BUF);
				break;

			case 2: // ROM checksum test P/F
				break;

			case 3: // Receiver failure P/F
				break;

			case 4: // Stored Data Lost R/L
				break;

			case 5: // Real Time Clock Lost R/L
				break;

			case 6: // Ocillator drift P/F
				break;

			case 7: // Data collection C/null
				break;

			case 8: // GPS sensor temperature
				break;

			case 9: // GPS Sensor Config
				break;

			case 10: // checksum ... make sure that we are really on the right token ...
				cs2=(unsigned char)strtoul(token,&dummy,16);
				if (cs1 != cs2)
					return false;
				break;
			default:
				break;
		
		}
		token=strstok(NULL,",*");
	} 
	while (token !=NULL);

	return true;

}


bool CRPGarmCom::updateInfo(bool invalidate)
{
	int curPos=0;
	char *tokptr,*toksav,wbuf[GARM_SER_BUF];
	INT32 time,date;
	double loc_lat,loc_lon,loc_alt,mov_spd,mov_dir;
	char name[GARM_NAM_BUF];

	if (invalidate)
		invalidateInfo();

	if (readComPort() > 0) 
	{
		strncpy(wbuf,iobuf,GARM_SER_BUF);
		tokptr=strtok_s(wbuf,"\n",&toksav);
		do {
			if (! strncmp(tokptr,"$GPRMC",6))
				if (parseGPRMC(tokptr,&time,&date,&loc_lat,&loc_lon,&mov_spd,&mov_dir))
				{
					lastGPRMC=CTime::GetCurrentTime();
					curData.setData(
						(loc_lat == CGPS_LAT_ILLEGAL)?NULL:&loc_lat,
						(loc_lon == CGPS_LON_ILLEGAL)?NULL:&loc_lon,
						NULL,
						&mov_spd,
						&mov_dir,
						&time,
						&date,
						(char *)0);
				}
			if (! strncmp(tokptr,"$GPGGA",6))
				if (parseGPGGA(tokptr,&time,&loc_lat,&loc_lon,&loc_alt))
				{
					lastGPGGA=CTime::GetCurrentTime();
					curData.setData(
						(loc_lat == CGPS_LAT_ILLEGAL)?NULL:&loc_lat,
						(loc_lon == CGPS_LON_ILLEGAL)?NULL:&loc_lon,
						(loc_alt == CGPS_ALT_ILLEGAL)?NULL:&loc_alt,
						NULL,
						NULL,
						&time,
						NULL,
						(char *)0);
				}
			if (! strncmp(tokptr,"$PGRMT",6))
				if (parsePGRMT(tokptr,name))
				{
					lastPGRMT=CTime::GetCurrentTime();
					curData.setData(NULL,NULL,NULL,NULL,NULL,NULL,NULL,name);
				}
			//if (temptoks.Left(6) == _T("$PGRMT"))
			//	parseGPGGA(temptoks,&gps_name);

			tokptr=strtok_s(NULL,"\n",&toksav);
		}
		while (tokptr != NULL);
		return true;
	}
	
	return false;
}

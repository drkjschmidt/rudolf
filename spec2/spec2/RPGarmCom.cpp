#include "StdAfx.h"
#include "RPGarmCom.h"
#include <serialport.h>

CRPGarmCom::CRPGarmCom(void)
: comPort(_T(""))
, lastGPRMC(0)
, lastGPGGA(0)
, lastPGRMT(0)
{
}

CRPGarmCom::~CRPGarmCom(void)
{
}

CString CRPGarmCom::getComPort(void)
{
	return comPort;
}

bool CRPGarmCom::setComPort(CString port)
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

	if (comPortCtrl.IsOpen())
		return true;

	return false;
}

void CRPGarmCom::closeComPort(void)
{
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

bool CRPGarmCom::check_csum(const char *temptoks)
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
	return true;
}

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

	if (strncmp(temptoks,"$GPRMC",6))
		return false;
	if (! check_csum(temptoks))
		return false;

	strncpy(wbuf,temptoks,GARM_SER_BUF);

	token=strtok(wbuf,",");
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
				break;

			case 3: // latitude ... I am assuming [+-]D{1-3}M{2}.FM{1-4}
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
				break;

			case 4: // N or S ... just change sign if S
				if (token[0] == 'S')
					*loc_lat *= -1;
				break;

			case 5: // longitude ... I am assuming [+-]D{1-3}M{2}.FM{1-4}
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
				break;

			case 6: // E or W ... just change sign if W
				if (token[0] == 'W')
					*loc_lon *= -1;
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
				break;

			default:
				break;
		
		}
		token=strtok(NULL,",");
	} 
	while (token !=NULL);

	return true;
}

bool CRPGarmCom::parseGPGGA(const char *temptoks,
							INT32 *time,
							double *loc_lat,double *loc_lon,double *loc_alt)
{
	char *token,wbuf[GARM_SER_BUF];
	int ii;

	int tcnt=0;
	int tidx;
	char tbuf[6];

	if (strncmp(temptoks,"$GPGGA",6))
		return false;
	if (! check_csum(temptoks))
		return false;

	strncpy(wbuf,temptoks,GARM_SER_BUF);

	token=strtok(wbuf,",");
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
				break;

			case 3: // N or S ... just change sign if S
				if (token[0] == 'S')
					*loc_lat *= -1;
				break;

			case 4: // longitude ... I am assuming [+-]D{1-3}M{2}.FM{1-4}
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
				break;

			case 5: // E or W ... just change sign if W
				if (token[0] == 'W')
					*loc_lon *= -1;
				break;

			case 6: // fix quality ... should we suppress results on 0?
				break;

			case 7: // num satellites
				break;

			case 8: // horizontal dilution
				break;

			case 9: // altitude ... assume meters
				*loc_alt = atol(token);
				break;

			case 10: // unit of altitude ... ft?
				if (token[0] == 'F')
					*loc_alt *= 0.3;
				break;

			case 11: // altitude of geoid ... do we need the accuracy?
				break;

			case 12: // units of geoid altitude
				break;

			default:
				break;
		
		}
		token=strtok(NULL,",");
	} 
	while (token !=NULL);

	return true;

}

bool CRPGarmCom::updateInfo(void)
{
	int curPos=0;
	char *tokptr,*toksav,wbuf[GARM_SER_BUF];

	if (readComPort() > 0) 
	{
		strncpy(wbuf,iobuf,GARM_SER_BUF);
		tokptr=strtok_s(wbuf,"\n\t ",&toksav);
		do {
			if (! strncmp(tokptr,"$GPRMC",6))
				if (parseGPRMC(tokptr,&time,&date,&loc_lat,&loc_lon,&mov_spd,&mov_dir))
					lastGPRMC=CTime::GetCurrentTime();
			if (! strncmp(tokptr,"$GPGGA",6))
				if (parseGPGGA(tokptr,&time,&loc_lat,&loc_lon,&loc_alt))
					lastGPGGA=CTime::GetCurrentTime();
			//if (temptoks.Left(6) == _T("$PGRMT"))
			//	parseGPGGA(temptoks,&gps_name);

			tokptr=strtok_s(NULL,"\n\t ",&toksav);
		}
		while (tokptr != NULL);
	}
	
	return 0;
}

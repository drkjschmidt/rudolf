#include "StdAfx.h"
#include "SpecNameConvert.h"

const int CSpecNameConvert::serial_count = 4;
const char *CSpecNameConvert::serial_manufacturer[] = {
		"USB4000 (USB4F06872)",
		"USB4000 (USB4F05510)",
		"USB4000 (USB4F07478)",
		"USB4000 (USB4F05920)", // Rudolf
	};
const int CSpecNameConvert::serial_wilson[] = {
		1001,
		1002,
		1003,
		0001,
	};

CSpecNameConvert::CSpecNameConvert(void)
{
}

CSpecNameConvert::~CSpecNameConvert(void)
{
}

CString CSpecNameConvert::getManufacturerSerial(CString WilsonSerial) {
	int ii,ts;
	CString tval;

	ts=atoi(WilsonSerial.GetString());
	for (ii=0; ii<serial_count; ii++) 
	{
		if (serial_wilson[ii] == ts)
		{
			tval=serial_manufacturer[ii];
			return tval;
		}
	}
	tval.Format(_T("Unknown: %04d"),ts);
	return tval;
}

CString CSpecNameConvert::getWilsonSerial(CString ManufacturerSerial) {
	int ii;
	CString tval;

	for (ii=0; ii<serial_count; ii++) 
	{
		tval=serial_manufacturer[ii];
		if (! ManufacturerSerial.CompareNoCase(tval)) {
			tval.Format(_T("%04d"),serial_wilson[ii]);
			return tval;
		}
	}
	tval=_T("Unknown: ");
	tval.Append(ManufacturerSerial);
	return tval;
}



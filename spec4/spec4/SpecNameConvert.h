#pragma once

class CSpecNameConvert
{
public:
	CSpecNameConvert(void);
	~CSpecNameConvert(void);

	static CString getManufacturerSerial(CString WilsonSerial);
	static CString getWilsonSerial(CString ManufacturerSerial);

private:
	static const int serial_count;

	static const char *serial_manufacturer[];
	static const int serial_wilson[];
};


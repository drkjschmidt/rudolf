#pragma once

class CCustomerInfoData
{
public:
	CCustomerInfoData(void);
	~CCustomerInfoData(void);

	//@name I/O functions
	//@{
	//! Save to file
	bool Save(CString *filename);

	//! Load from file
	bool Load(CString *filename);

	//! Create serialized data for save function
	CString serializeData(unsigned char modes);

	//! Convert serialized data from load function to calibration curve contents
	bool deserializeData(CString data, int &lpos, int &llpos);
	//@}

	enum SerializeModes {
		outputCSV =   (unsigned char) 0x01,
		outputXML =   (unsigned char) 0x02,
	};

	// @@@ I wonder if GPS information should be mandatory?
	inline bool isValid() { return isvalid; }
	inline void setValid() { isvalid=true; }
	inline void setInValid() { isvalid=false; }

	inline CString getNameCust() { return nameCust; }
	inline CString getNameLoc()  { return nameLoc;  }
	inline CString getNameSamp()  { return nameSamp;  }
	// @@@ We have a generic GPS class ... we should fix this to use it ... 
	inline CString getGpsString() { 
		CString tval; 
		tval = acqGPSInfo.formatLoc() + acqGPSInfo.formatDate() + acqGPSInfo.formatTime();
		// tval.Format(_T("%.6fN %.6fW %.2fm %d %d"),lat,-lon,alt,time,date); 
		return tval; }
	inline double getDataPipeKm() { return dataPipeKm; }
	inline CString getDataPipeKmStr() { CString tval; tval.Format(_T("%g"),dataPipeKm); return tval; }
	inline double getDataDilution() { return dataDilution; }
	inline CString getDataDilutionStr() { CString tval; tval.Format(_T("%g"),dataDilution); return tval; }
	inline CString getDataFF1() { return dataFF1; }
	inline CString getDataFF2() { return dataFF2; }

	inline void setNameCust(CString name) { nameCust=name; }
	inline void setNameLoc(CString name)  { nameLoc=name;  }
	inline void setNameSamp(CString name)  { nameSamp=name;  }
	inline void setGpsLoc(double slat, double slon, double salt) {
		// lat=slat;
		// lon=slon;
		// alt=salt;
		acqGPSInfo.setData(&slat,&slon,&salt,NULL,NULL,NULL,NULL,(char *)NULL);
	}
	inline void setGpsLoc(CGPSData &sGPS) { acqGPSInfo = sGPS; }
	inline void setDataPipeKm(double data) { dataPipeKm=data; }
	inline void setDataPipeKmStr(CString data) { dataPipeKm=atof(data.GetString()); }
	inline void setDataDilution(double data) { dataDilution=data; }
	inline void setDataDilutionStr(CString data) { dataDilution=atof(data.GetString()); }
	inline void setDataFF1(CString data) { dataFF1=data; }
	inline void setDataFF2(CString data) { dataFF2=data; }

protected:
	bool isvalid;
	// double lat,lon,alt;
	// INT32 time,date;
	CGPSData acqGPSInfo;

	CString nameCust,nameLoc,nameSamp;
	double dataPipeKm,dataDilution;
	CString dataFF1,dataFF2;


};

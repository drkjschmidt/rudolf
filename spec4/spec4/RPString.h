#pragma once

class CRPString :
	public CString
{
public:
	CRPString(void);
	CRPString(CString init);
	~CRPString(void);

	enum KeepGroups {
		capitals   = (int)0x0001,
		lowercase  = (int)0x0002,
		numbers    = (int)0x0004,

		spaces     = (int)0x0010,
		newlines   = (int)0x0020,
		tabs       = (int)0x0040,

		dots       = (int)0x0100,
		dashes     = (int)0x0200,
		underscore = (int)0x0400,

		letters    = (int)0x0003,
		whitespace = (int)0x0070,
		punct      = (int)0x0700,
	};

	CRPString GetReplaceWithUnderscores(int keep=(
		CRPString::letters|
		CRPString::numbers|
		CRPString::punct)
		);

	CRPString GetEscaped(int keep=(
		CRPString::letters|
		CRPString::numbers|
		CRPString::spaces|
		CRPString::punct)
		);

	CRPString GetUnEscaped();

	//! Creates a unique identifier composed of system time, system name, and a random number
	CRPString CreateUniqueID();

	CRPString PseudoHexEncode();
	CRPString PseudoHexDecode();

	enum ppmFormat {
		ppmFmt_Vx = (int)0x02,
		ppmFmt_xU = (int)0x01,
		ppmFmt_VU = ppmFmt_Vx|ppmFmt_xU,

		ppmFmt_PPM = (int)0x10,
		ppmFmt_PPX = (int)0x20,

		ppmFmtPPM_VU = ppmFmt_PPM|ppmFmt_VU, // all ppm ... format value and units
		ppmFmtPPM_Vx = ppmFmt_PPM|ppmFmt_Vx, // all ppm ... format value
		ppmFmtPPM_xU = ppmFmt_PPM|ppmFmt_xU, // all ppm ... format units

		ppmFmtPPX_VU = ppmFmt_PPX|ppmFmt_VU, // ppt/ppm/ppb ... format values and units
		ppmFmtPPX_Vx = ppmFmt_PPX|ppmFmt_Vx, // ppt/ppm/ppb ... format values
		ppmFmtPPX_xU = ppmFmt_PPX|ppmFmt_xU, // ppt/ppm/ppb ... format units
	};

	CRPString ppxFormat(double f, int style=ppmFmtPPX_VU);
};

#include "StdAfx.h"
#include "RPString.h"

CRPString::CRPString(void)
: CString()
{
}

CRPString::CRPString(CString init)
: CString(init)
{
}

CRPString::~CRPString(void)
{
}

CRPString CRPString::GetReplaceWithUnderscores(int keep)
{
	int ii;
	CRPString vval;
	char cc;

	vval=*this;

	for (ii=0; ii<vval.GetLength(); ii++) {
		cc=vval.GetAt(ii);
		if (((keep & CRPString::capitals)   && ('A'<=cc) && (cc<='Z')) ||
			((keep & CRPString::lowercase)  && ('a'<=cc) && (cc<='z')) ||
			((keep & CRPString::numbers)    && ('0'<=cc) && (cc<='9')) ||
			((keep & CRPString::dashes)     && ('-'==cc)) ||
			((keep & CRPString::underscore) && ('_'==cc)) ||
			((keep & CRPString::dots)       && ('.'==cc)) ||
			((keep & CRPString::spaces)     && (' '==cc)) ||
			((keep & CRPString::tabs)       && ('\t'==cc)) ||
			((keep & CRPString::newlines)   && ('\n'==cc)) ||
			((keep & CRPString::newlines)   && ('\r'==cc))
		) { 
		}
		else {
			vval.SetAt(ii,'_');
		}
	}
	return vval;
}

CRPString CRPString::GetEscaped(int keep)
{
	int ii;
	CRPString vval;
	char cc;

	for (ii=0; ii<this->GetLength(); ii++) {
		cc=this->GetAt(ii);
		if (((keep & CRPString::capitals)   && ('A'<=cc) && (cc<='Z')) ||
			((keep & CRPString::lowercase)  && ('a'<=cc) && (cc<='z')) ||
			((keep & CRPString::numbers)    && ('0'<=cc) && (cc<='9')) ||
			((keep & CRPString::dashes)     && ('-'==cc)) ||
			((keep & CRPString::underscore) && ('_'==cc)) ||
			((keep & CRPString::dots)       && ('.'==cc)) ||
			((keep & CRPString::spaces)     && (' '==cc)) ||
			((keep & CRPString::tabs)       && ('\t'==cc)) ||
			((keep & CRPString::newlines)   && ('\n'==cc)) ||
			((keep & CRPString::newlines)   && ('\r'==cc))
		) { 
			vval.AppendFormat(_T("%c"),cc);
		}
		else {
			switch ((int)cc) {
				case 8:
					vval.Append(_T("\\t"));
					break;
				case 10:
					vval.Append(_T("\\r"));
					break;
				case 13:
					vval.Append(_T("\\n"));
					break;
				case '\\':
					vval.Append(_T("\\\\"));
					break;
				default:
					vval.AppendFormat(_T("\\%o"),(unsigned char)cc);
					break;
			}

		}
	}
	return vval;
}

CRPString CRPString::GetUnEscaped()
{
	int ii;
	CRPString vval;
	char cc,cd;
	unsigned int ce;

	for (ii=0; ii<this->GetLength(); ii++) {
		cc=this->GetAt(ii);

		if (cc == '\\') {
			cd=this->GetAt(++ii);

			switch (cd) {
				case '\\':
					vval.Append(_T("\\"));
					break;
				case 't':
					vval.AppendFormat(_T("%c"),(unsigned char)8);
					break;
				case 'r':
					vval.AppendFormat(_T("%c"),(unsigned char)10);
					break;
				case 'n':
					vval.AppendFormat(_T("%c"),(unsigned char)13);
					break;

				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					ce=(cd - '0');
					do {
						cd=this->GetAt(ii+1);
						if (! (('0'<=cd) && (cd<='9')))
							break;
						ce=ce*8+(cd - '0');
						ii++;
					} while (ii<this->GetLength());
					vval.AppendFormat(_T("%c"),ce);
					break;
				default:
					vval.AppendFormat(_T("%c"),cc);
					break;
			}
		}
		else {
			vval.AppendFormat(_T("%c"),cc);
		}
	}
	return vval;
}

CRPString CRPString::CreateUniqueID() {
	CRPString vval;
	SYSTEMTIME curtime;
	char tmpname[256];
	DWORD tmplen=sizeof(tmpname);
	int currand;
	CString curname;

	GetSystemTime(&curtime);
	if (GetComputerName(tmpname,&tmplen)) {
		curname=tmpname;
	} else {
		curname="UNK";
	}

	srand((unsigned int)time(NULL));
	currand=rand();

	vval.Format(_T("%04d%02d%02d-%02d%02d%02d%04d-%s-%08d"),
		curtime.wYear,
		curtime.wMonth,
		curtime.wDay,
		curtime.wHour,
		curtime.wMinute,
		curtime.wSecond,
		curtime.wMilliseconds,
		curname,
		currand);
	return vval;
}

// Stupendously lazy way of hexencoding: hex sucks because the characters a not sequential
// and thus we need lookup tables etc. Instead still use 16 value encoding but simply use
// consecutive letters abcdfghijklmnop so we can just use math ... 
CRPString CRPString::PseudoHexEncode() {
	int ii;
	CRPString vval;
	char cc;

	for (ii=0; ii<this->GetLength(); ii++) {
		cc=this->GetAt(ii);

		vval.AppendFormat(_T("%c"), 'a' + (unsigned char)((cc>>4)&0x0F) );
		vval.AppendFormat(_T("%c"), 'a' + (unsigned char)( cc    &0x0F) );
	}
	return vval;
}

CRPString CRPString::PseudoHexDecode() {
	int ii;
	CRPString vval;
	int cc,dd;

	for (ii=0; ii<this->GetLength(); ii++) {
		switch(ii%2) {
			case 0:
				cc = ((int)(this->GetAt(ii)) - (unsigned char)'a');
				// this should never happen ... 
				if ((cc < 0) || (cc > 15))
					return CString();
				break;
			case 1:
				dd = ((int)(this->GetAt(ii)) - (unsigned char)'a');
				if ((dd < 0) || (dd > 15))
					return CString();
				vval.AppendFormat(_T("%c"),(char)((cc<<4) | dd));
				break;
		}
	}
	return vval;
}

CRPString CRPString::ppxFormat(double ppm,int style) {
	CString vval;
	
	// Value ... 
	switch (style&(ppmFmt_PPX|ppmFmt_PPM|ppmFmt_Vx)) {
		case ppmFmt_PPX|ppmFmt_Vx:
			if (ppm >= 1e3  && ppm < 1e6) { // parts per thousand
				vval.Format("%.1f", ppm/1000);
			} else if (ppm >= 1e0  && ppm < 1e3) { // parts per million 
				vval.Format("%.1f", ppm);
			} else if (ppm >= 1e-3 && ppm < 1e0) { // parts per billion
				vval.Format("%.1f", ppm*1000);
			} else if (ppm >= 1e-6 && ppm < 1e-3) { // parts per trillion
				vval.Format("%.1f", ppm*1000);
			} else { // less than 1 part per billion or more than 1000 part per thousand use scientific notation with ppm
				vval.Format("%.1e", ppm);
			}
			break;
		case ppmFmt_PPM|ppmFmt_Vx:
			vval.Format("%.1f", ppm);
			break;
		default:
			break;
	}

	if ((style&ppmFmt_VU) == ppmFmt_VU)
		vval.Append(" ");

	switch (style&(ppmFmt_PPX|ppmFmt_PPM|ppmFmt_xU)) {
		case ppmFmt_PPX|ppmFmt_xU:
			if (ppm >= 1e3  && ppm < 1e6) { // parts per thousand
				vval.Append("ppt");
			} else if (ppm >= 1e0  && ppm < 1e3) { // parts per million 
				vval.Append("ppm");
			} else if (ppm >= 1e-3 && ppm < 1e0) { // parts per billion
				vval.Append("ppb");
			} else if (ppm >= 1e-6 && ppm < 1e-3) { // parts per trillion
				vval.Append("ppb");
			} else { // less than 1 part per billion or more than 1000 part per thousand use scientific notation with ppm
				vval.Append("ppm");
			}
			break;
		case ppmFmt_PPM|ppmFmt_xU:
			vval.Format("ppm");
			break;
		default:
			break;
	}
	return vval;
}





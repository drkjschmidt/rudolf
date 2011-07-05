#include "StdAfx.h"
#include "ptrclass.h"
#include <string.h>

ptrclass::ptrclass(void)
{
	ptrdata=NULL;
}

ptrclass::ptrclass(const char *set)
{
	ptrdata=new char[strlen(set) + 1];
	if (ptrdata != NULL) {
		strncpy(ptrdata,set,strlen(set)+1);
	}
}

ptrclass::~ptrclass(void)
{
	delete ptrdata;
}

#if SMARTCOPY == 1
ptrclass&
ptrclass::operator=(const ptrclass &that)
{
	if (this != &that) {
		delete ptrdata;
		ptrdata=new char[strlen(that.ptrdata) + 1];
		if (ptrdata != NULL) {
			strncpy(ptrdata,that.ptrdata,strlen(that.ptrdata)+1);
		}
	}
	return *this;
}
#endif
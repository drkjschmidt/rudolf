#pragma once

#define REGHELPER_BUF_SIZE 256
#define REGHELPER_BUF_INCR REGHELPER_BUF_SIZE

class RegistryHelper
{
public:
	RegistryHelper(void);
	~RegistryHelper(void);

	bool Init(CString &ClassPath);
	bool ReadStringValue(const CString &name,CString &value);
	bool WriteStringValue(const CString &name,const CString &value);

private:
	CString StoredClassPath;
};

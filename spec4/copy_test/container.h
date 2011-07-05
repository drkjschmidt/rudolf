#pragma once

#include "ptrclass.h"

class container
{
public:
	container(void);
	container(const char *set);
	~container(void);

	inline void showptr() { str1.showptr(); };
	inline void showdata() { str1.showdata(); };

private:
	ptrclass str1;

};

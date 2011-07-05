#pragma once



class ptrclass
{
public:
	ptrclass(void);
	ptrclass(const char *set);
	~ptrclass(void);

#if SMARTCOPY == 1
	// create a copy constructor
	// ptrclass& ptrclass(const ptrclass &that);

	// create an assignment operator
	ptrclass& operator =(const ptrclass &that);

#endif

	inline void showptr() { printf("PTR: %lx\n",(long)ptrdata); };
	inline void showdata() { printf("STR: %s\n",(ptrdata==NULL)?"NULL":ptrdata); };

private:
	char *ptrdata;
};

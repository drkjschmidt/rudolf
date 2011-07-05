// copy_test.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "ptrclass.h"
#include "container.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char dummy;

#if SMARTCOPY == 1
	printf("Overloaded assignment operator ... all should be well\n");
#else
	printf("No overloaded assignent operator ... we expect pointer copies and heap corruption\n");
#endif

	printf("\n--- copy one string to another ---\n\n");

	ptrclass test("hello world");
	ptrclass test2;

	test.showptr();
	test.showdata();

	test2=test; // BAD

	test2.showptr();
	test2.showdata();

	printf("\n--- shallow copy: copy class containing string class ---\n\n");

	container cont("I own you!");
	container cont2;

	cont.showptr();
	cont.showdata();

	cont2=cont; // BAD

	cont2.showptr();
	cont2.showdata();

	scanf("%c",&dummy);
	return 0;
}


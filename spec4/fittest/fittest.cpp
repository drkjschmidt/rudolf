// fittest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fittest.h"
#include <math.h>
#include <gmd.h>
#include <laslv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
#define DEG 1
#define NPTS 20
#define SINGULAR 1
#define RAND 1

		int ii,jj;
		double xv[NPTS],yv[NPTS];
		// rows=npts, columns=degree+1
		LaGenMatDouble XV(NPTS,DEG+1);
		LaGenMatDouble YV(NPTS,1);
		LaGenMatDouble AV(DEG+1,1);

		for (ii=0; ii<NPTS; ii++) {
#ifdef SINGULAR
			xv[ii]=1;
			yv[ii]=1;
#else
#  ifdef RAND
			xv[ii]=ii;
			yv[ii]=pow((double)ii,(double)DEG)+2.0+(rand()-rand())/32000.0;
#  else
			xv[ii]=ii;
			yv[ii]=pow((double)ii,(double)DEG);
#  endif
#endif
		}

		// populate X data matrix
		//
		//                 jj
		//               0 ...   degree
		//       0  x[0]^0 ... x[0]^deg
		//       :  x[1]^0 ... x[1]^deg
		// ii    :
		//       :
		//    npts  x[n]^0 ... x[n]^deg
		//
		double tv;
		for (jj=0; jj<=DEG; jj++)
		{
			for (ii=0; ii<NPTS; ii++)
			{
				tv=pow(xv[ii],jj);
				XV(ii,jj)=tv;
			}
		}
		// populate y vector
		for (ii=0; ii<NPTS; ii++)
		{
			YV(ii,0)=yv[ii];
		}

		for (ii=0; ii<=DEG; ii++)
		{
			AV(ii,0)=0;
		}

		for (ii=0; ii<NPTS; ii++) 
		{
			printf("| ");
			for (jj=0; jj<=DEG; jj++)
				printf("%4.1f ",(const double)XV(ii,jj));
			printf("| ");
			printf("| %4.1f |\n",(const double)YV(ii,0));
		}
		LaLinearSolve(XV,AV,YV);

		for (ii=0; ii<NPTS; ii++)
			printf("%g,%g\n",xv[ii],yv[ii]);

		printf("\n");

		for (ii=0; ii<=DEG; ii++)
			printf("%g,",AV(ii,0));
		printf("\n");
	}

	return nRetCode;
}

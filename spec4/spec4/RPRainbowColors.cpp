#include "StdAfx.h"
#include "RPRainbowColors.h"

RPRainbowColors::RPRainbowColors(void)
{
}

RPRainbowColors::~RPRainbowColors(void)
{
}

// HSV values are 0-1 
// RGB values are 0-1
void RPRainbowColors::HSVtoRGB(double h, double s, double v, double* r, double* g, double* b)
{
	if ( s == 0 )
	{
		*r = v;
		*g = v;
		*b = v;
	}
	else
	{
		double var_h = h * 6;
		double var_i = (double)(int)( var_h ); // hack for "floor"
		double var_1 = v * ( 1 - s );
		double var_2 = v * ( 1 - s * ( var_h - var_i ) );
		double var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );

		if      ( var_i == 0 ) { *r = v     ; *g = var_3 ; *b = var_1; }
		else if ( var_i == 1 ) { *r = var_2 ; *g = v     ; *b = var_1; }
		else if ( var_i == 2 ) { *r = var_1 ; *g = v     ; *b = var_3; }
		else if ( var_i == 3 ) { *r = var_1 ; *g = var_2 ; *b = v;     }
		else if ( var_i == 4 ) { *r = var_3 ; *g = var_1 ; *b = v;     }
		else                   { *r = v     ; *g = var_1 ; *b = var_2; }
	}
}	

void RPRainbowColors::rainbow_rgb(int idx, int num,int *r255, int *g255, int *b255) {
	double r1,g1,b1;

	HSVtoRGB((double)idx/num,1,1,&r1,&g1,&b1);
	*r255=(int)(255*r1);
	*g255=(int)(255*g1);
	*b255=(int)(255*b1);
}

COLORREF RPRainbowColors::rainbow_rgb(int idx, int num) {
		int r255,g255,b255;
		rainbow_rgb(idx, num,&r255,&g255,&b255);
		return RGB(r255,g255,b255);
	}

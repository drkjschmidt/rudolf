#pragma once

class RPRainbowColors
{
public:
	RPRainbowColors(void);
	~RPRainbowColors(void);

	static void HSVtoRGB(
		double h, double s, double v, 
		double* r, double* g, double* b);

	// Split HSV spectrum into num colors and return the one with given index
	// in RGB 0-255 ... note that idx=0 == idx=num ...
	static void rainbow_rgb(int idx, int num,int *r255, int *g255, int *b255);

	static COLORREF rainbow_rgb(int idx, int num);
};

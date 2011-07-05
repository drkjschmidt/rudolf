#pragma once

class CGreatCircle
{
public:
	CGreatCircle(void);
	~CGreatCircle(void);
	static double distGPSHaversine(double p1lat, double p1lon, double p2lat, double p2lon);
};

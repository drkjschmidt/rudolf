#include "StdAfx.h"
#include "GreatCircle.h"
#include <cmath>

CGreatCircle::CGreatCircle(void)
{
}

CGreatCircle::~CGreatCircle(void)
{
}

double CGreatCircle::distGPSHaversine(double lat1, double long1, double lat2, double long2)
{
	// Sample format for latitide and longitudes
	// double lat1=45.54243333333333,lat2=45.53722222,long1=-122.96045277777778,long2=-122.9630556;
	// Below is the main code
	double PI = 4.0*atan(1.0);

	//main code inside the class
	double dlat1=lat1*(PI/180);

	double dlong1=long1*(PI/180);
	double dlat2=lat2*(PI/180);
	double dlong2=long2*(PI/180);

	double dLong=dlong1-dlong2;
	double dLat=dlat1-dlat2;

	double aHarv= pow(sin(dLat/2.0),2.0)+cos(dlat1)*cos(dlat2)*pow(sin(dLong/2),2);
	double cHarv=2*atan2(sqrt(aHarv),sqrt(1.0-aHarv));
	//earth's radius from wikipedia varies between 6,356.750 km � 6,378.135 km (�3,949.901 � 3,963.189 miles)
	//The IUGG value for the equatorial radius of the Earth is 6378.137 km (3963.19 mile)
	// const double earth=3963.19;//I am doing miles, just change this to radius in kilometers to get distances in km
	const double earth=6378.137;// km
	double distance=earth*cHarv;

	return distance;
}

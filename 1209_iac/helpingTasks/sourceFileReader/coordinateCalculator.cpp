#include <cmath>
#include <cstdlib>

#include <iostream>


#include "coordinateCalculator.h"



coordinateCalculator::coordinateCalculator(){
	
}

coordinateCalculator::coordinateCalculator(coordinateCalculator const &other){

}

coordinateCalculator& coordinateCalculator::operator=(coordinateCalculator const &other){
	return *this;
}

coordinateCalculator::~coordinateCalculator(){

}

double coordinateCalculator::latitudeFromGeoCoord(geoCoord point){
	double res =
		point.latitude_deg +
		point.latitude_min * 1.0 / 60.0 +
		point.latitude_sec * 1.0 / 3600.0;

	double pi = 4.0 * atan(1.0);
	double dr = pi / 180.0f;		

	res *= dr;

	return res;
}

double coordinateCalculator::longitudeFromGeoCoord(geoCoord point){
	double res =
		point.longitude_deg +
		point.longitude_min * 1.0 / 60.0 +
		point.longitude_sec * 1.0 / 3600.0;

	double pi = 4.0 * atan(1.0);
	double dr = pi / 180.0f;

	res *= dr;

	return res;
}


void coordinateCalculator::setBasePoint(geoCoord newBasePoint){
	basePoint = newBasePoint;

	basePointLatitude = latitudeFromGeoCoord(basePoint);
	basePointLongitude = longitudeFromGeoCoord(basePoint);
}

void coordinateCalculator::setBasePoint(double latitude, double longitude){
	basePointLatitude = latitude;
	basePointLongitude = longitude;

	basePoint = geoCoordFromLatLong(latitude, longitude);
}

geoCoord coordinateCalculator::geoCoordFromLatLong(double latitude, double longitude){

	double pi = 4.0 * atan(1.0);
	latitude = latitude * 180.0 / pi;
	longitude = longitude * 180.0 / pi;
	
	int latitude_deg = abs(latitude);
	latitude -= abs(latitude);
	latitude *= 60.0;
	int latitude_min = abs(latitude);
	latitude -= abs(latitude);
	latitude *= 60.0;
	double latitude_sec = abs(latitude);

	int longitude_deg = abs(longitude);
	longitude -= abs(longitude);
	longitude *= 60.0;
	int longitude_min = abs(longitude);
	longitude -= abs(longitude);
	longitude *= 60.0;
	double longitude_sec = abs(longitude);

	geoCoord point= {
		latitude_deg,
		latitude_min,
		latitude_sec,
		longitude_deg,
		longitude_min,
		longitude_sec
	};

	return point;
}

position coordinateCalculator::geoToDecart(double latitude, double longitude){

	/*std::cout << "geoToDecart call()" << std::endl;*/

	double distance;
	double bearing;
	
	// pi
	double pi = 4.0f * atan(1.0f);
	// coeff deg->rad
	double dr = pi / 180.0f;
	// coeff rad->deg
	double rd = 180.0f / pi;
	// Earth radius
	float r_earth = 6372.795;


	double latitude1 = latitude;
	double longitude1 = longitude;
	double latitude0 = basePointLatitude;
	double longitude0 = basePointLongitude;
	
	/*
	std::cout << "latitude1=" << latitude1 << std::endl;
	std::cout << "longitude1=" << longitude1 << std::endl;
	std::cout << "latitude0=" << latitude0 << std::endl;
	std::cout << "longitude0=" << longitude0 << std::endl;
	std::cout << "delta longitude=" << longitude1 - longitude0 << std::endl;
	std::cout << "delta latitude=" << latitude1 - latitude0 << std::endl;
	*/

	double cl0 = cos(latitude0);
	double cl1 = cos(latitude1);
	double sl0 = sin(latitude0);
	double sl1 = sin(latitude1);
	double delta = longitude1 - longitude0;
	double cdelta = cos(delta);
	double sdelta = sin(delta);
	// Length of large circle
	double y = sqrt( pow( cl1 * sdelta, 2 ) + pow( cl0 * sl1 - sl0 * cl1 * cdelta, 2 ) );
	double x = sl0 * sl1 + cl0 * cl1 * cdelta;
	double ad = atan2(y,x);
	distance = ad * r_earth;

	// initial azimuth
	x = (cl0 * sl1) - (sl0 * cl1 * cdelta);
	y = sdelta * cl1;
	double z = atan(-y/x);
	z *= rd;
	if (x < 0){
		z += 180;
	}

	double z2 = fmod( ( z + 180. ), 360) - 180;
	z2 = - (z2) * dr;
	double anglerad2 = z2 - ( (2 * pi) * floor( (z2 / ( 2 * pi ) ) ) );
	bearing = anglerad2 * rd;

	/*
	std::cout << "distance=" << distance << std::endl;
	std::cout << "bearing=" << bearing << std::endl;
	std::cout << std::endl << std::endl << std::endl;
	*/

	return position(distance * sin(bearing * dr), distance * cos(bearing * dr));
}

position coordinateCalculator::geoToDecart(geoCoord point){

	double latitude = latitudeFromGeoCoord(point);
	double longitude = longitudeFromGeoCoord(point);

	return geoToDecart(latitude, longitude);
}

geoCoord coordinateCalculator::getBasePoint(){
	return basePoint;
}

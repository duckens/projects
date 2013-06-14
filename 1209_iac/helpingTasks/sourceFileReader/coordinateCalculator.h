	
#ifndef COORDINATECALCULATOR_H
#define COORDINATECALCULATOR_H

#include "classes/position.h"



class coordinateCalculator{
    
	private:

		geoCoord basePoint;

		double basePointLatitude;
		double basePointLongitude;

		double latitudeFromGeoCoord(geoCoord point);
		double longitudeFromGeoCoord(geoCoord point);

	public:

		coordinateCalculator();
		coordinateCalculator(coordinateCalculator const &other);
		coordinateCalculator& operator=(coordinateCalculator const &other);
		~coordinateCalculator();

		void setBasePoint(geoCoord newBasePoint);
		void setBasePoint(double latitude, double longitude);

		geoCoord geoCoordFromLatLong(double latitude, double longitude);

		position geoToDecart(geoCoord geoPosition);
		position geoToDecart(double latitude, double longitude);

		geoCoord getBasePoint();
};

#endif

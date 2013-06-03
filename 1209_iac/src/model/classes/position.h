#ifndef POSITION_H
#define POSITION_H

class position{
	public:
		double x;
		double y;
		double z;
		
		position(double posx = 0, double posy = 0, double posz = 0);
		
		double distance(const position& obj);
		double flatDistance(const position& obj);
		
		position shift(double azimuth, double length);
		position azimuthShift(position center, double pelengStepInDegrees);
};
#endif
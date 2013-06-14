#include <cmath>

#include "position.h"

position::position(double posx, double posy, double posz){
	x = posx;
	y = posy;
	z = posz;
}
double position::distance(const position& obj){
	return sqrt((obj.x - x) * (obj.x - x) + (obj.y - y) * (obj.y - y) + (obj.z - z) * (obj.z - z));
}
double position::flatDistance(const position& obj){
	return sqrt((obj.x - x) * (obj.x - x) + (obj.y - y) * (obj.y - y));
}
position position::shift(double azimuth, double length){
	double azimutInRadians = 2 * 3.1415926536 * azimuth / 180;
	return position(x + length * cos(azimutInRadians), y + length * sin(azimutInRadians), z);
}
position position::azimuthShift(position center, double pelengStepInDegrees){
	double pelengStepInRadians = pelengStepInDegrees * 2 * 3.1415926536 / 180;
	double relativeX = x - center.x;
	double relativeY = y - center.y;
	return position(center.x + (relativeX * cos(pelengStepInRadians) - relativeY * sin(pelengStepInRadians)), center.y + (relativeX * sin(pelengStepInRadians) + relativeY * cos(pelengStepInRadians)), z);
}

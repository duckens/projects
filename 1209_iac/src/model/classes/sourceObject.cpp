#include "model/classes/sourceObject.h"

sourceObject::sourceObject(position sourceObjectPosition, const double freq, const double speed_along_x, const double speed_along_y, const double amp){
	x = sourceObjectPosition.x;
	y = sourceObjectPosition.y;
	z = sourceObjectPosition.z;
	frequency = freq;
	speed_x = speed_along_x;
	speed_y = speed_along_y;
	amplitude = amp;
}
sourceObject::sourceObject(const double pos_x, const double pos_y, const double pos_z, const double freq, const double speed_along_x, const double speed_along_y, const double amp){
	x = pos_x;
	y = pos_y;
	z = pos_z;
	frequency = freq;
	speed_x = speed_along_x;
	speed_y = speed_along_y;
	amplitude = amp;
}
sourceObject& sourceObject::operator =(const sourceObject& obj){
	if(!(&obj == this)){
		pos = obj.pos;
		frequency = obj.frequency;
		speed_x = obj.speed_x;
		speed_y = obj.speed_y;
		amplitude = obj.amplitude;
	}
	
	return *this;
}
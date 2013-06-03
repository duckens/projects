#ifndef SOUCEOBJECT_H
#define SOUCEOBJECT_H

#include "model/classes/position.h"

class sourceObject : public position{
	
	private:
	
		
		double speed_x;
		double speed_y;
		double amplitude;
		position pos;
		
	public:
		
		double frequency;
		
		sourceObject(position sourceObjectPosition, const double freq, const double speed_along_x = 0.0, const double speed_along_y = 0.0, const double amp = 1.0);
		sourceObject(const double pos_x, const double pos_y, const double pos_z, const double freq = 75.0, const double speed_along_x = 0.0, const double speed_along_y = 0.0, const double amp = 1.0);
		sourceObject& operator =(const sourceObject& obj);		
};
#endif
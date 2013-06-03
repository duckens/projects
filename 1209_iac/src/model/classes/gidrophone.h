#ifndef GIDROPHONE_H
#define GIDROPHONE_H

#include "model/classes/position.h"

class gidrophone : public position{
	
	private:

	public:
		gidrophone(position gidrophonePosition);
		
		gidrophone& operator =(const gidrophone& obj);
};
#endif
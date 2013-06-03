#ifndef INLINEANTENA_H
#define INLINEANTENA_H

#include <vector>

#include "model/classes/gidrophone.h"
#include "model/classes/position.h"

class inlineAntena{
	
	private:
	
		std::vector<gidrophone> gidrophones;
		int gidrophonesAmount;
	
	public:

		inlineAntena(const int number = 0, const position firstGidrophonePosition = position(0, 0, 0), const double azimuth = 0, const double step = 0);
		
		inlineAntena(std::vector<gidrophone>& gidrophoneVector);
		
		gidrophone& operator[](int index);
		
		int size();
};
#endif
#ifndef SOURCECIRCLESEQUENCE_H
#define SOURCECIRCLESEQUENCE_H

#include "model/classes/position.h"
#include "model/classes/sourceSequence.h"

class sourceCircleSequence : public sourceSequence{
	private:
	
		position center;
		
	public:
		

		sourceCircleSequence(const int number, const position firstSourcePosition, const position centerOfCircle, const double azimuthStep, const double frequency = 75.03255208);
		
		sourceObject& operator[](int index);
		int size();
};
#endif
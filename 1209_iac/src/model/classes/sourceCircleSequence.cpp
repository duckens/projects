#include "model/classes/sourceCircleSequence.h"

sourceCircleSequence::sourceCircleSequence(const int number, const position firstSourcePosition, const position centerOfCircle, const double azimuthStep, const double frequency){
	center = centerOfCircle;
	
	sourceObjects.clear();
	position newPos = firstSourcePosition;
	sourceObjects.push_back(sourceObject(newPos, frequency));
	
	for(int i = 1; i < number; i++){
		sourceObjects.push_back(sourceObject(newPos.azimuthShift(center, azimuthStep * i), frequency));
	}
	
}
sourceObject& sourceCircleSequence::operator[](int index){
	return sourceObjects[index];
}
int sourceCircleSequence::size(){
	return sourceObjects.size();
}
#include "model/classes/sourceSequence.h"

sourceSequence::sourceSequence(){
}
sourceSequence::sourceSequence(std::vector<sourceObject>& positions){
	sourceObjects = positions;
}
sourceObject& sourceSequence::operator[](int index){
	return sourceObjects[index];
}
int sourceSequence::size(){
	return sourceObjects.size();
}
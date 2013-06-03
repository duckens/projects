#ifndef SOURCESEQUENCE_H
#define SOURCESEQUENCE_H
#include <vector>

#include "model/classes/sourceObject.h"

class sourceSequence{
	private:
	
	public:
		std::vector<sourceObject> sourceObjects;
		
		sourceSequence(std::vector<sourceObject>& positions);
		sourceSequence();
		sourceObject& operator[](int index);
		int size();
};
#endif
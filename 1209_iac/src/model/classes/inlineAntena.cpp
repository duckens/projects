#include<vector>

#include "model/classes/inlineAntena.h"
#include "model/classes/gidrophone.h"


inlineAntena::inlineAntena(const int number, const position firstGidrophonePosition, const double azimuth, const double step){
	gidrophones.clear();
	position newPos = firstGidrophonePosition;
	gidrophones.push_back(gidrophone(newPos));
	for(int i = 1; i < number; i++){
		gidrophones.push_back(gidrophone(newPos.shift(azimuth, step * i)));
	}
	gidrophonesAmount = gidrophones.size();
}		
inlineAntena::inlineAntena(std::vector<gidrophone>& gidrophoneVector){
	gidrophones = gidrophoneVector;
	gidrophonesAmount = gidrophones.size();
}		
gidrophone& inlineAntena::operator[](int index){
	return gidrophones[index];
}		
int inlineAntena::size(){
	return gidrophonesAmount;
}
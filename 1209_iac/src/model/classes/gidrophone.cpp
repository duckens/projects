#include "model/classes/gidrophone.h"

gidrophone::gidrophone(position gidrophonePosition){
	x = gidrophonePosition.x;
	y = gidrophonePosition.y;
	z = gidrophonePosition.z;
}
	
gidrophone& gidrophone::operator =(const gidrophone& obj){
	if(!(&obj == this)){
		x = obj.x;
		y = obj.y;
		z = obj.z;
		return *this;
	}
}
#include "converters.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits>


std::string converters::intToString(int value){
	char str[40];
	sprintf(str, "%d", value);
	return std::string(str);
}

std::string converters::doubleToString(double value){
	char str[40];
	sprintf(str, "%f", value);
	return std::string(str);
}

double converters::stringToDouble(std::string str){
	return atof( str.c_str() );
}

std::string converters::intToStringFourDigits(int value){
	char str[40];
	sprintf(str, "%004d", value);
	return std::string(str);
}
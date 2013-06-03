#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <string>

namespace converters{
	std::string intToString(int value);
	std::string intToStringFourDigits(int value);
	std::string doubleToString(double value);
	double stringToDouble(std::string str);
}
#endif
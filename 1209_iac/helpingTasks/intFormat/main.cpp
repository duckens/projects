#include "stringDecoder.h"
#include <iostream>

int main(){
	for(int i = -10; i <= 10; i++){
		std::cout << i << "\t= " << intToBits(i) << std::endl;
	}
	int i;
	i = 0x7fffffff;
	std::cout << i << "\t= " << intToBits(i) << std::endl;
	i = -0x80000000;
	std::cout << i << "\t= " << intToBits(i) << std::endl;
	for(int i = -10; i <= 10; i++){
		std::cout << "( " << i << " << 8 )" << "\t= " << intToBits(i << 8) << std::endl;
	}

	i = 194 << 8;
	std::cout << "i = " << i << std::endl;
}
#include "decoder.h"

int main(){
	
	//std::cout << "run" << std::endl;
	
	unsigned char* data_input = new unsigned char[4];
	data_input[0] = 0;
	data_input[1] = 0;
	data_input[2] = 226;
	data_input[3] = 66;
	//std::cout << "data_input ready" << std::endl;
	
	int size_input = 4;
	//std::cout << "size_input ready" << std::endl;
	
	std::vector<int>* format_input = new std::vector<int>();
	format_input->push_back(timeSystem::Time);
	//std::cout << "format_input ready" << std::endl;
	
	
	//std::cout << "ALL INPUT data ready" << std::endl;
	timeSystem::dataArray array = timeSystem::dataArray(data_input, size_input, format_input);
	std::cout << array[0].getType() << std::endl;
	std::cout << "cast to float : " << (float)array[0] << std::endl;
	/*
	*/
	
	
	//std::cout << "\n\n\nOutside direct" << std::endl;
	std::vector<timeSystem::dataElement> data2;
	data2.push_back(timeSystem::dataElement(data_input, timeSystem::Time));
	//std::cout << "data2[0].getType() = " << data2[0].getType() << std::endl;

	//00 00 E2 42
	
	return 0;
}

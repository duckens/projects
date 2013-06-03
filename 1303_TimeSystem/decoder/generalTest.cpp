#include "decoder.h"
#include "stringdecoder.h"

int main(){
	
	
	unsigned char* data_input = new unsigned char[4];
	data_input[0] = 0;
	data_input[1] = 0;
	data_input[2] = 226;
	data_input[3] = 66;
	
	int size_input = 4;
	
	std::vector<int>* format_input = new std::vector<int>();
	format_input->push_back(timeSystem::Time);
	
	
	timeSystem::dataArray array = timeSystem::dataArray(data_input, size_input, format_input);
	std::cout << array[0].getType() << std::endl;
	std::cout << "cast to float : " << (float)array[0] << std::endl;
	std::cout << "ID = " << array.getID() << std::endl;
	
	std::string hex = "00 00 E2 42";
	std::cout << "string decoder : " << decode_FP(hex) << std::endl;
	
	
	
	
	return 0;
}

#include <iostream>

#include "signalFileReader.h"
#include "containers/arrays.h"


int main(){
	
	signalFileReader reader;

	std::cout << reader.open("BM_1_DPLA_1_14.11.12.13.44.36_1") << std::endl;
	qint64 startTime = reader.startTime();
	std::cout << startTime << std::endl;
	
	dualVector<int>* data_dual = reader.data(startTime, 15000);
	std::vector<int>* data = reader.data(startTime, 15000, 0);

	std::cout << "data[" << data->size() << "] = [";
	for(int i = 0; i < 10; i++){
		std::cout << (*data)[i] << ", ";
	}
	std::cout << "]" << std::endl;

	return 0;
}
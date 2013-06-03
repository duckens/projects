#include <iostream>

#include "signalFileReader.h"


signalFileReader::signalFileReader(signalFileReader const &other_reader){

}

signalFileReader& signalFileReader::operator= (signalFileReader const &other_reader){
	return *this;
}

signalFileReader::~signalFileReader(){

}




bool signalFileReader::open(std::string filename){
	int last_SeparatorOccurance = filename.find("_", 0);
	this->filename = filename.substr(0, last_SeparatorOccurance);
	std::cout << this->filename << std::endl;
}



bool signalFileReader::fileExists(filename){

}

bool signalFileReader::blockChecksumOk(int blockNumber){
	return 0;
}

int signalFileReader::countBlocksNFiles(){
	return 0;
}

int signalFileReader::findFile(int time){
	return 0;
}

int signalFileReader::findPlaceInFile(int time){
	return 0;
}



ll signalFileReader::startTime(){
	return 0;
}

std::vector<int>* signalFileReader::data(ll start, int length){
	return NULL;
}

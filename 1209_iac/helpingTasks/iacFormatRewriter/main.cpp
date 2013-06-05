#include <QtCore>

#include <iostream>

#include <vector>
#include <string>

#include "rewriteParser.h"

int main(int argc, char *argv[]){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " input_file_name" << std::endl;
		return 0;
	}
	QString inputFilename = argv[1];
	QString outputFilename = inputFilename + "_out";

	rewriteParser parser;

	int gidrophonesAmount;
	int signalLength;
	std::vector<std::string> fileNames;

	parser.parse(inputFilename.toStdString(), outputFilename.toStdString(), gidrophonesAmount, signalLength, &fileNames);
	std::cout << "gidrophonesAmount=" << gidrophonesAmount << std::endl;
	std::cout << "signalLength=" << signalLength << std::endl;

	return 0;
}

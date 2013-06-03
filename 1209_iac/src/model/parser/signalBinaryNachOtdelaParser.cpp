#include "signalBinaryNachOtdelaParser.h"

#include <fstream>
#include <stdlib.h>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#else

#endif



#include "../misc/converters.h"




signalBinaryNachOtdelaParser::signalBinaryNachOtdelaParser(std::string filePath, std::string destinationFolderName){
	log.open("signalBinaryNachOtdelaParser.log");
	log << "constructor(" << filePath << ") call" << std::endl << std::endl;

	path = filePath;
	destFolderName = destinationFolderName;
	
	
	signalFile = fopen(filePath.c_str(), "rb");
	log << "FILE* signalFile = fopen(" << filePath.c_str() << ", \"rb\"); \t done" << std::endl;


	fread(&N_Treal, 4, 1, signalFile);
	log  << std::endl << "fread(&N_Treal, 4, 1, signalFile); \t done" << std::endl;
	log << "N_Treal = " << N_Treal << std::endl;

	fread(&AntNumber, 4, 1, signalFile);
	log  << std::endl << "fread(&AntNumber, 4, 1, signalFile); \t done" << std::endl;
	log << "AntNumber = " << AntNumber << std::endl;

	fread(&N_DA, 4, 1, signalFile);
	log  << std::endl << "fread(&N_DA, 4, 1, signalFile); \t done" << std::endl;
	log << "N_DA = " << N_DA << std::endl;

	
	double* antennaPositionX = new double[N_DA];
	double* antennaPositionY = new double[N_DA];
	double* antennaPositionZ = new double[N_DA];
	log << "antennaPositionX reading..";
	//for(int i = 0; i < N_DA; i++){
		fread(antennaPositionX, sizeof(double), N_DA, signalFile);
	//}
	log << "done" << std::endl;
	log << "antennaPositionY reading..";
	//for(int i = 0; i < N_DA; i++){
		fread(antennaPositionY, sizeof(double), N_DA, signalFile);
	//}
	log << "done" << std::endl;
	log << "antennaPositionZ reading..";
	//for(int i = 0; i < N_DA; i++){
		fread(antennaPositionZ, sizeof(double), N_DA, signalFile);
	//}
	log << "done" << std::endl;
	/*
	double* uselessPtr = (double *)malloc(sizeof(double) * 3 * N_DA);
	fread(uselessPtr, sizeof(double), 3 * N_DA, signalFile);
	log << std::endl << "fread(uselessPtr, sizeof(double), 3 * N_DA, signalFile); \t done" << std::endl;
	log << "sizeof(double) = " << sizeof(double) << std::endl;
	*/
	
	fread(&TimeOfCycle, sizeof(double), 1, signalFile);
	log  << std::endl << "fread(&TimeOfCycle, 4, 1, signalFile); \t done" << std::endl;
	log << "TimeOfCycle = " << TimeOfCycle << std::endl;

	fread(&TimeOfRegistration, sizeof(double), 1, signalFile);
	log  << std::endl << "fread(&TimeOfRegistration, 4, 1, signalFile); \t done" << std::endl;
	log << "TimeOfRegistration = " << TimeOfRegistration << std::endl;

	fread(&FDiscr, sizeof(double), 1, signalFile);
	log  << std::endl << "fread(&FDiscr, 4, 1, signalFile); \t done" << std::endl;
	log << "FDiscr = " << FDiscr << std::endl;

	fread(&SampleLength, 4, 1, signalFile);
	log  << std::endl << "fread(&SampleLength, 4, 1, signalFile); \t done" << std::endl;
	log << "SampleLength = " << SampleLength << std::endl;

	fread(&Reserv, 4, 4, signalFile);
	log  << std::endl << "fread(&Reserv, 4, 1, signalFile); \t done" << std::endl;
	log << "Reserv = " << Reserv << std::endl;


	std::ofstream antennaPositionFile;
	std::string str = escapeFileName(path) + "/" + destFolderName + "/antenna.txt";
	
	antennaPositionFile.open(str.c_str());
	antennaPositionFile << N_DA << std::endl;
	for(int i = 0; i < N_DA; i++){
		antennaPositionFile << antennaPositionX[i] << "\t" << antennaPositionY[i] << " " << std::endl;
	}
	antennaPositionFile.close();
	
	log << "constructor ret" << std::endl;
}
signalBinaryNachOtdelaParser::~signalBinaryNachOtdelaParser(){
	fclose(signalFile);
	log << "destructor call" << std::endl;
	log.close();
}

std::string signalBinaryNachOtdelaParser::escapeFileName(std::string path){
	log << "escapeFileName(" << path << ") call" << std::endl;
	// we drop path behind last '/' symbol
	// siply go from end to front
	
	int pointer = path.size();
	
	while((path[pointer] != '/') && (path[pointer] != '\\')){
		pointer--;
	}
	
	path.resize(pointer);
	log << "escaped variant is: " << path << std::endl;
	return path;
}

bool signalBinaryNachOtdelaParser::parse(std::string filesNameMask, int& gidrophonesAmount, int& signalLength, std::vector<std::string>* fileNames){
	log << std::endl << "parse() call" << std::endl;
	
	// what i have to return i know from the beginning
	gidrophonesAmount = N_DA;
	signalLength = SampleLength;
	
	//std::vector<string>* fileNames = new std::vector<string>;
	fileNames->clear();
	
	std::vector<FILE*> gidrophoneFiles;
	gidrophoneFiles.resize(N_DA);
	log << "gidrophoneFiles->resize(N_DA); \tdone" << std::endl;
	
	//signalFile = fopen(path.c_str(), "rb");
	for(int i = 0; i < N_DA; i++){
		char str[300];
		sprintf(str, "%s/%s/%s%004d", escapeFileName(path).c_str(), destFolderName.c_str(), filesNameMask.c_str(), i);
		gidrophoneFiles[i] = fopen(str, "wb");
		//fileNames->push_back(filesNameMask + converters::intToStringFourDigits(i));
		fileNames->push_back( std::string(str) );
		//log << "pushing : " << filesNameMask + converters::intToStringFourDigits(i) << std::endl;
		log << "pushing : " << std::string(str) << std::endl;
	}
	log << "for(int i = 0; i < N_DA; i++){ char str[20]; ... } \tdone" << std::endl;
	
	float* buffer = new float[N_DA];
	log << "float* buffer = new float[" << N_DA << "]; \tdone" << std::endl;


	#ifdef PLATFORM_WINDOWS
	SYSTEMTIME time_before;
	SYSTEMTIME time_after;
		
	GetSystemTime(&time_before);
	#else
	#endif


	for(int j = 0; j < 10; j++){
		fread(buffer, N_DA * sizeof(float), 1, signalFile);
		
		for(int i = 0; i < N_DA; i++){
			fwrite(&(buffer[i]), sizeof(float), 1, gidrophoneFiles[i]);
			log << "buffer[" << i << "] = " << buffer[i] << std::endl;
		}
		
	}
	for(int j = 10; j < SampleLength; j++){
		fread(buffer, N_DA * sizeof(float), 1, signalFile);
		
		for(int i = 0; i < N_DA; i++){
			fwrite(&(buffer[i]), sizeof(float), 1, gidrophoneFiles[i]);
		}
		
	}
	#ifdef PLATFORM_WINDOWS
	GetSystemTime(&time_after);


	WORD mins = time_after.wMinute - time_before.wMinute;
	WORD millis_before = (time_before.wSecond * 1000) + time_before.wMilliseconds;
	WORD millis_after = (time_after.wSecond * 1000) + time_after.wMilliseconds;
	WORD millis;
	if(millis_after >= millis_before){
		millis = millis_after - millis_before;
	} else {
		millis = 60000 + millis_after - millis_before;
		mins--;
	}
	log << std::endl << std::endl << "TIMING:" << std::endl << "Parsing done in: " << mins << "min, " << millis << " millis!" << std::endl;
	#else
	#endif
	
	
	
	for(int i = 0; i < N_DA; i++){
		fclose(gidrophoneFiles[i]);
	}
	
	log << "for(int i = 0; i < N_DA; i++){\n\tfclose(gidrophoneFiles[i]);\n} \tdone" << std::endl;
	
	//makeConfigFile(escapeFileName(path));
	//pathToConfigFile = escapeFileName(path);
	
	return true;
}

/*
void signalBinaryNachOtdelaParser::makeConfigFile(std::string pathToConfigFile){
	log << "makeConfigFile(" << pathToConfigFile << ")" << std::endl;

	std::string configFilePathWithName = pathToConfigFile;
	configFilePathWithName += "\\config.txt";
	log << "opening " << configFilePathWithName << std::endl;
	std::ofstream configFile(configFilePathWithName.c_str());
	
	//is read like this
	//	configFile >> gidrophonesAmount >> signalLength;

	
	configFile << N_DA << std::endl << SampleLength << std::endl;
	
	for(int i = 0; i < N_DA; i++){
		char str[20];
		sprintf(str, "gidro_%004d", i);
		configFile << str << std::endl;
	}
}
*/
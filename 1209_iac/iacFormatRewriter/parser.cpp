#include "parser.h"

#include <fstream>
#include <iostream>
#include <cstdlib>


rewriteParser::rewriteParser(){}
rewriteParser::~rewriteParser(){}

void rewriteParser::parse(std::string inputFileName, std::string outputFilesName, int& gidrophonesAmount, int& signalLength, std::vector<std::string>* fileNames){
	
	this->inputFileName = inputFileName;
	
	f_in = fopen(inputFileName.c_str(), "rb");

	fread(&N_Treal, 4, 1, f_in);
	fread(&AntNumber, 4, 1, f_in);
	fread(&N_DA, 4, 1, f_in);	
	
	double* antennaPositionX = new double[N_DA];
	double* antennaPositionY = new double[N_DA];
	double* antennaPositionZ = new double[N_DA];
	
	fread(antennaPositionX, sizeof(double), N_DA, f_in);
	fread(antennaPositionY, sizeof(double), N_DA, f_in);
	fread(antennaPositionZ, sizeof(double), N_DA, f_in);
	fread(&TimeOfCycle, sizeof(double), 1, f_in);
	fread(&TimeOfRegistration, sizeof(double), 1, f_in);
	fread(&FDiscr, sizeof(double), 1, f_in);
	fread(&SampleLength, 4, 1, f_in);
	fread(&Reserv, 4, 4, f_in);
	

	/*
	std::ofstream antennaPositionFile;
	std::string str = escapeFileName(path) + "/" + destFolderName + "/antenna.txt";
	
	antennaPositionFile.open(str.c_str());
	antennaPositionFile << N_DA << std::endl;
	for(int i = 0; i < N_DA; i++){
		antennaPositionFile << antennaPositionX[i] << "\t" << antennaPositionY[i] << " " << std::endl;
	}
	antennaPositionFile.close();
	*/

	

	gidrophonesAmount = N_DA;
	signalLength = SampleLength;
	
	float* buffer = new float[N_DA];
	
	qf_out = new QFile(QString(outputFilesName.c_str()));
	qf_out->open(QIODevice::WriteOnly);
	qf_out->seek(0);

	int tripleNumber;

	for(int j = 0; j < SampleLength; j++){
		// qDebug() << "j =" << j;
		fread(buffer, N_DA * sizeof(float), 1, f_in);
		
		// do stuff with buffer - it contains data - all gidrophones, one measurment
		if(j % MEASURMENTS_IN_INNER_BLOCK == 0){
			qDebug() << "block" << j / MEASURMENTS_IN_INNER_BLOCK;
			if(j != 0){
				rewriteInnerHeader(lastInnerHeaderPosition);
				writeOuterChecksum();
				rewriteOuterHeader(lastOuterHeaderPosition);
			}

			lastOuterHeaderPosition = qf_out->pos();
			writeOuterHeader();
			lastInnerHeaderPosition = qf_out->pos();
			writeInnerHeader();

			tripleNumber = 0;
		}
		writeOneTriple(buffer, tripleNumber++);
	}

	fclose(f_in);
	
	delete[] antennaPositionX;
	delete[] antennaPositionY;
	delete[] antennaPositionZ;

	delete[] buffer;
}

void rewriteParser::writeOuterChecksum(){
	qf_out->seek( qf_out->pos() + OUTER_CHECKSUM_SIZE);
}
void rewriteParser::writeOuterHeader(){
	qf_out->seek( qf_out->pos() + OUTER_HEADER_SIZE);
}

void rewriteParser::writeInnerHeader(){
	qf_out->seek( qf_out->pos() + INNER_HEADER_SIZE);
}

void rewriteParser::rewriteOuterHeader(qint64 positionOFHeader){
	qint64 storedPosition = qf_out->pos();
	qf_out->seek(positionOFHeader);
	// write smth
	qf_out->seek(storedPosition);
}

void rewriteParser::rewriteInnerHeader(qint64 positionOFHeader){
	qint64 storedPosition = qf_out->pos();
	qf_out->seek(positionOFHeader);
	// write smth
	qf_out->seek(storedPosition);
}
void rewriteParser::writeOneTriple(float* buffer, int tripleNumber){
	// qDebug() << "writeOneTriple call";
	qint64 tripleStartPosition = qf_out->pos();

	char* triplePacket = new char[TRIPLE_SIZE];
	for(int i = 0; i < GIDROPHONES_AMOUNT; i++){
		
		float number_f = buffer[channelSequently(i)] * ADC_COEFFICIENT;
		int number_i;

		if(abs(number_f) > MAX_24BIT_NUMBER){
			if(number_f > 0){
				number_i = 0xfffffff;
			} else {
				number_i = -1 * 0xfffffff;
			}
		} else {
			int number_i = (int)number_f;
		}

		memcpy(triplePacket + shiftSequently(i), &number_i, ADC_DIMENTION_BYTES);

	}
	qf_out->write(triplePacket, TRIPLE_SIZE);
	qf_out->seek(tripleStartPosition + TRIPLE_SIZE);
}

int rewriteParser::channelSequently(int i){
	if(i < 120){
		if(i < 60){
			return 60 - i;
		} else {
			return 119 + (i - 60);
		}
	} else {
		if(i < 149){
			return 90 + (i - 120);
		} else {
			return 89 - (i - 149);
		}
	}
}

int rewriteParser::shiftSequently(int i){
	int shiftForPacketInTriple;
	if(i < 120){
		if(i < 60){
			shiftForPacketInTriple = 0;
		} else {
			shiftForPacketInTriple = UDP_PACKET_SIZE;
			i -= 60;
		}
	} else {
		shiftForPacketInTriple = 2 * UDP_PACKET_SIZE;
		if(i < 149){
			i -= 119;
		} else {
			i -= 118;
		}
	}
	return UDP_PACKET_NUMBER_SIZE + ( (i / 10) + 1 ) * UDP_PACKET_SYSINFO_SIZE + 3 * i + shiftForPacketInTriple;
}

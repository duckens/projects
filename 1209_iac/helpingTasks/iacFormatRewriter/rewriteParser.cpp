#include "rewriteParser.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>


rewriteParser::rewriteParser() : DIFFERENCE_BETWEEN_INT_AND_ADC_IN_BITS(computeDifferenceBetweenIntAndAndInBits(ADC_DIMENTION_BYTES)){

}

rewriteParser::~rewriteParser(){}

int rewriteParser::computeDifferenceBetweenIntAndAndInBits(int adc_dimension_in_bytes){
	return 8 * (sizeof(int) - adc_dimension_in_bytes);
}

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
	std::cout << "TimeOfRegistration=" << TimeOfRegistration << std::endl;
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
	double currentBlock_t0 = TimeOfRegistration;

	for(int j = 0; j < SampleLength; j++){
		// qDebug() << "j =" << j;
		fread(buffer, N_DA * sizeof(float), 1, f_in);
		
		// do stuff with buffer - it contains data - all gidrophones, one measurment
		if(j % MEASURMENTS_IN_INNER_BLOCK == 0 || j == SampleLength - 1){
			qDebug() << "block" << j / MEASURMENTS_IN_INNER_BLOCK;
			if(j != 0){
				std::cout << "tripleNumber=" << tripleNumber << std::endl;
				qint64 innerBlockSize = INNER_HEADER_SIZE + tripleNumber * TRIPLE_SIZE;
				qint64 outerBlockSize = OUTER_HEADER_SIZE + innerBlockSize + OUTER_CHECKSUM_SIZE;
				rewriteInnerHeader(lastInnerHeaderPosition, currentBlock_t0, innerBlockSize);
				writeOuterChecksum();
				rewriteOuterHeader(lastOuterHeaderPosition, outerBlockSize);
			}

			lastOuterHeaderPosition = qf_out->pos();
			writeOuterHeader();
			lastInnerHeaderPosition = qf_out->pos();
			writeInnerHeader();

			tripleNumber = 0;
			currentBlock_t0 += ONE_BLOCK_LENGTH_IN_SECS;
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
	qf_out->seek( qf_out->pos() + OUTER_CHECKSUM_SIZE );
}
void rewriteParser::writeOuterHeader(){
	qf_out->seek( qf_out->pos() + OUTER_HEADER_SIZE );
}

void rewriteParser::writeInnerHeader(){
	qf_out->seek( qf_out->pos() + INNER_HEADER_SIZE );
}

void rewriteParser::rewriteOuterHeader(qint64 positionOFHeader, qint64 outerBlockSize){
	qint64 storedPosition = qf_out->pos();
	qf_out->seek(positionOFHeader);
	// write smth
	qf_out->write((char*)(&outerBlockSize), OUTER_HEADER_SIZE);
	qf_out->seek(storedPosition);
}

void rewriteParser::rewriteInnerHeader(qint64 positionOFHeader, double t0, qint64 innerBlockSize){
	qint64 storedPosition = qf_out->pos();
	qf_out->seek(positionOFHeader);

	uint nBM = (uint)1;
	uint nAnt = (uint)AntNumber;
	uint TypeAnt = (uint)1;
	uint DataSize = (uint)innerBlockSize;
	std::cout << "DataSize=" << DataSize << std::endl;


	/*
	QDataStream stream(qf_out);
	stream << t0;
	// skipping "Silda" name
	qf_out->seek(qf_out->pos() + 16);
	stream << nBM;
	stream << nAnt;
	*/

	
	// write start time of block
	qf_out->write((char*)&t0, sizeof(double));
	// skipping "Silda" name
	qf_out->seek(qf_out->pos() + 16);
	// write BM number
	qf_out->write((char*)&nBM, sizeof(uint));
	// write antenna number
	qf_out->write((char*)&nAnt, sizeof(uint));
	// write antenna type
	qf_out->write((char*)&TypeAnt, sizeof(uint));
	// write block size
	qf_out->write((char*)&DataSize, sizeof(uint));
	
	qf_out->seek(storedPosition);
}
void rewriteParser::writeOneTriple(float* buffer, int tripleNumber){
	/* DEBUG */
	tripleNumber = 127;

	qint64 tripleStartPosition = qf_out->pos();

	char* triplePacket = new char[TRIPLE_SIZE];
	memset(triplePacket, 0, TRIPLE_SIZE);

	
	for(int i = 0; i < GIDROPHONES_AMOUNT; i++){
		
		float number_f = buffer[channelSequently(i)] * ADC_COEFFICIENT;
		
		/* DEBUG */
		// number_f = 1;

		int number_i;

		if(abs(number_f) > MAX_24BIT_NUMBER){
			if(number_f > 0){
				number_i = MAX_24BIT_NUMBER << DIFFERENCE_BETWEEN_INT_AND_ADC_IN_BITS;
			} else {
				number_i = MIN_24BIT_NUMBER << DIFFERENCE_BETWEEN_INT_AND_ADC_IN_BITS;
			}
			/* DEBUG */
			// std::cout << "overflow" << std::endl;
		} else {
			number_i = (int)(number_f * ADC_COEFFICIENT) << DIFFERENCE_BETWEEN_INT_AND_ADC_IN_BITS;

		}
		
		/* DEBUG */
		// std::cout << std::endl;
		// std::cout << "number_f=" << number_f << std::endl;
		// std::cout << "number_f * " << ADC_COEFFICIENT << "=" << number_f * ADC_COEFFICIENT << std::endl;
		// std::cout << "(int)(number_f * ADC_COEFFICIENT)=" << (int)(number_f * ADC_COEFFICIENT) << std::endl;
		// std::cout << "number_i = " << number_i << std::endl;

		// std::cout << "writing " <<  (int)(number_f * ADC_COEFFICIENT) << " shifted " << number_i << " offset " << shiftSequently(i) << std::endl;
		memcpy(triplePacket + shiftSequently(i), &number_i, ADC_DIMENTION_BYTES);

	}
	

	memcpy(triplePacket, &tripleNumber, sizeof(int));

	qf_out->write(triplePacket, TRIPLE_SIZE);
	qf_out->seek(tripleStartPosition + TRIPLE_SIZE);

	/* DEBUG */
	// qf_out->close();
	// exit(0);
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

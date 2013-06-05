#ifndef SIGNALBINARYNACHOTDELAPARSER_H
#define SIGNALBINARYNACHOTDELAPARSER_H


#include <QtCore>

#include <vector>
#include <string>
#include <fstream>


class rewriteParser{
				
	private:

		const int GIDROPHONES_AMOUNT = 178;
		const int OUTER_CHECKSUM_SIZE = 2;
		const int OUTER_HEADER_SIZE = 8;
		const int INNER_HEADER_SIZE = 40;

		const int UDP_PACKET_NUMBER_SIZE = 4;
		const int UDP_PACKET_SYSINFO_SIZE = 2;
		
		const int UDP_PACKET_SIZE = 260;
		const int UDP_PACKETS_FOR_ONE_MEASURMENT = 3;
		const int TRIPLE_SIZE = UDP_PACKETS_FOR_ONE_MEASURMENT * UDP_PACKET_SIZE;
		const int FREQUENCY_DISCRETESATION = 1000;
		const int MSECS_IN_SEC = 1000;

		const int ADC_DIMENTION_BYTES = 3;
		
		const int MEASURMENTS_IN_INNER_BLOCK = 20480;
		const double ONE_BLOCK_LENGTH_IN_SECS = (double)MEASURMENTS_IN_INNER_BLOCK / FREQUENCY_DISCRETESATION;
		
		const int ADC_COEFFICIENT = 1;
		const int MAX_24BIT_NUMBER = 0x7fffff;
		const int MIN_24BIT_NUMBER = -0x800000;

		 const int DIFFERENCE_BETWEEN_INT_AND_ADC_IN_BITS;

		static int computeDifferenceBetweenIntAndAndInBits(int adc_dimension_in_bytes);

		std::string inputFileName;
		FILE* f_in;
		
		int N_Treal; 				// realisation number
		int AntNumber;				// antena number
		int N_DA;					// amount of gidrophones in antenna
		double TimeOfCycle;			// start time of signal emitting
		double TimeOfRegistration;	// start time of registration on antenna
		double FDiscr;				// discretasation frequency
		int SampleLength;			// number of measurments
		int Reserv[4];				// reserve

		QFile* qf_out;
		qint64 lastOuterHeaderPosition;
		qint64 lastInnerHeaderPosition;

		int channelSequently(int i);
		int shiftSequently(int i);

		void writeOuterHeader();
		void writeInnerHeader();
		
		void writeOuterChecksum();

		void rewriteOuterHeader(qint64 positionOFHeader, qint64 outerBlockSize);
		void rewriteInnerHeader(qint64 positionOFHeader, double t0, qint64 innerBlockSize);

		void writeOneTriple(float* buffer, int tripleNumber);

	public:

		rewriteParser();
		~rewriteParser();

		void parse(std::string inputFileName, std::string outputFilesName, int& gidrophonesAmount, int& signalLength, std::vector<std::string>* fileNames);
};
#endif
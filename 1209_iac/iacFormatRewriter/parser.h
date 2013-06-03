#ifndef SIGNALBINARYNACHOTDELAPARSER_H
#define SIGNALBINARYNACHOTDELAPARSER_H


#include <QtCore>

#include <vector>
#include <string>
#include <fstream>


class rewriteParser{
	public:

		rewriteParser();
		~rewriteParser();

		void parse(std::string inputFileName, std::string outputFilesName, int& gidrophonesAmount, int& signalLength, std::vector<std::string>* fileNames);
		
		
	private:

		static const int GIDROPHONES_AMOUNT = 178;
		static const int OUTER_CHECKSUM_SIZE = 2;
		static const int OUTER_HEADER_SIZE = 8;
		static const int INNER_HEADER_SIZE = 25;

		static const int UDP_PACKET_NUMBER_SIZE = 4;
		static const int UDP_PACKET_SYSINFO_SIZE = 2;

		static const int UDP_PACKET_SIZE = 260;
		static const int UDP_PACKETS_FOR_ONE_MEASURMENT = 3;
		static const int TRIPLE_SIZE = UDP_PACKETS_FOR_ONE_MEASURMENT * UDP_PACKET_SIZE;
		static const int FREQUENCY_DISCRETESATION = 1000;
		static const int MSECS_IN_SEC = 1000;

		static const int ADC_DIMENTION_BYTES = 3;

		static const int MEASURMENTS_IN_INNER_BLOCK = 20480;
		static const int ADC_COEFFICIENT = 8000;
		static const int MAX_24BIT_NUMBER = 8388608;

		std::string inputFileName;
		FILE* f_in;
		
		int N_Treal; 	// realisation number
		int AntNumber;	// antena number
		int N_DA;            // amount of gidrophones in antenna
		double TimeOfCycle;  // start time of signal emitting
		double TimeOfRegistration;  // start time of registration on antenna
		double FDiscr;     		 // discretasation frequency
		int SampleLength;   	// number of measurments
		int Reserv[4];    		// reserve

		QFile* qf_out;
		qint64 lastOuterHeaderPosition;
		qint64 lastInnerHeaderPosition;

		int channelSequently(int i);
		int shiftSequently(int i);

		void writeOuterHeader();
		void writeInnerHeader();
		void writeOuterChecksum();
		void rewriteOuterHeader(qint64 positionOFHeader);
		void rewriteInnerHeader(qint64 positionOFHeader);
		void writeOneTriple(float* buffer, int tripleNumber);
};
#endif
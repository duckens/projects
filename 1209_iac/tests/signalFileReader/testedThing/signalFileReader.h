#include <vector>

#include <QtCore>

#include "containers/arrays.h"


typedef long long int ll;

enum{NOANT, DPLA, VPLA, IVLAR};

struct blockHeader_t{
	double t0;
	char caption[16];
	uint nBM;
	uint aAnt;
	uint TypeAnt;
	uint DataSize;
};

class blockPointer{
	private:
	public:
		int fileNumber;
		qint64 timeStart;
		qint64 positionInFile;
		qint64 sizeInBytes;
		qint64 sizeInMSecs;
		qint64 sizeInMeasurments;
		blockPointer(int fileNumber, qint64 timeStart, qint64 positionInFile, qint64 sizeInBytes, qint64 sizeInMSecs, qint64 sizeInMeasurments);

};

class signalFileReader{

	private:

		static const int GIDROPHONES_AMOUNT = 178;
		static const int OUTER_CHECKSUM_SIZE = 2;
		static const int OUTER_HEADER_SIZE = 8;
		static const int INNER_HEADER_SIZE = 25;

		static const int UDP_PACKET_NUMBER_SIZE = 4;
		static const int UDP_PACKET_SYSINFO_SIZE = 2;

		// static const int BLOCK_HEADER_SIZE = 8;
		// static const int BLOCK_CHECKSUM_SIZE = 2;

		static const int UDP_PACKET_SIZE = 260;
		static const int UDP_PACKETS_FOR_ONE_MEASURMENT = 3;
		static const int TRIPLE_SIZE = UDP_PACKETS_FOR_ONE_MEASURMENT * UDP_PACKET_SIZE;
		static const int FREQUENCY_DISCRETESATION = 1000;
		static const int MSECS_IN_SEC = 1000;

		static const int ADC_DIMENTION_BYTES = 3;

		int filesAmount;

		qint64 startTimeGlobal;

		QString filename_prefix;
		
		std::vector<QFile*> files;
		std::vector<int> filesSizes;
		std::vector<qint64> filesStartTimes;

		QMap< qint64, blockPointer >* blocksAllocationTable; // msecsSinceEpoch -> placeInFile

		bool blockChecksumOk(int blockNumber);

		// int findFile(qint64 time);
		// qint64 findPlaceInFile(qint64 time);
		blockPointer placeInFile(qint64 time);


		bool fileExists(QString filename);
		void openAllFiles();
		void closeAllFiles();
		
		void measureAllFiles();
		void measureFile(int fileNumber);
		bool getStartTimeFromFilename();

		void resizeAllVectors();
		void createBlocksTable();

		bool skipToNextBlock(QFile* f);

		/* 2B wiped */
		/*
		// done))
		blockHeader_t* _readInnerHeader(QFile* f);
		*/

		qint64 readOuterHeader(QFile* f);
		
		int channelSequently(int i);
		int shiftSequently(int i);
		blockPointer readInnerHeader(int fileNumber);



	public:

		signalFileReader();
		signalFileReader(signalFileReader const &other_reader);
		signalFileReader& operator= (signalFileReader const &other_reader);
		~signalFileReader();

		bool open(QString filename);

		qint64 startTime();

		dualVector<int>* data(qint64 start, qint64 length);

};
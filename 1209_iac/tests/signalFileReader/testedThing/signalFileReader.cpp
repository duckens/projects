#include <iostream>

#include "signalFileReader.h"


blockPointer::blockPointer(int fileNumber, qint64 timeStart, qint64 positionInFile, qint64 sizeInBytes, qint64 sizeInMSecs, qint64 sizeInMeasurments){
	this->fileNumber = fileNumber;
	this->timeStart = timeStart;
	this->positionInFile = positionInFile;
	this->sizeInBytes = sizeInBytes;
	this->sizeInMSecs = sizeInMSecs;
	this->sizeInMeasurments = sizeInMeasurments;

}




signalFileReader::signalFileReader(){
	blocksAllocationTable = new QMap< qint64, blockPointer >;
}

signalFileReader::signalFileReader(signalFileReader const &other_reader){
	blocksAllocationTable = new QMap< qint64, blockPointer >;
}

signalFileReader& signalFileReader::operator= (signalFileReader const &other_reader){
	return *this;
}
signalFileReader::~signalFileReader(){}





bool signalFileReader::open(QString filename){
	// qDebug() << ;
	qDebug() << "open called";
	
	int last_SeparatorOccurance = filename.lastIndexOf("_");

	do{
		if( last_SeparatorOccurance == -1 ) break;

		filename_prefix = filename;
		filename_prefix.chop(filename.size() - last_SeparatorOccurance - 1);
		qDebug() << filename_prefix;
		
		filesAmount = 0;
		for(int i = 1;; i++){
			qDebug() << "checking file exists " << i;
			if(fileExists(filename_prefix + QString::number(i))){
				qDebug() << "yes";
				filesAmount++;
			} else {
				qDebug() << "no";
				break;
			}
		}

		if(!filesAmount) break;

		qDebug() << "filesAmount = " << filesAmount;
		if(!getStartTimeFromFilename()){
			qDebug() << "Finally yes!";
			break;
		}
		resizeAllVectors();
		openAllFiles();

		qDebug() << "openAllFiles done";

		measureAllFiles();

		for(int i = 0; i < filesAmount; i++){
			qDebug() << QString("filesSizes[") + QString::number(i) + "] = " + QString::number(filesSizes[i]);
			// qDebug() << QString("filesStartTimes[") + QString::number(i) + "] = " + QString::number(filesStartTimes[i]);
		}

		return true;
		

	} while(0);

	return false;
}

bool signalFileReader::getStartTimeFromFilename(){
	
	QString dateTimeFileName = filename_prefix;
	
	do{
		qDebug() << "check !dateTimeFileName.endsWith(\"_\")";
		if(!dateTimeFileName.endsWith("_")) break;

		int prelast_Occurance = dateTimeFileName.lastIndexOf("_", -2);

		qDebug() << "check prelast_Occurance == -1";
		if(prelast_Occurance == -1) break;

		dateTimeFileName = dateTimeFileName.mid(prelast_Occurance + 1, 17);

		qDebug() << "check dateTimeFileName.size() != 17";
		qDebug() << dateTimeFileName;
		if(dateTimeFileName.size() != 17) break; // should be 17 = 6 * 2 letter per muneric field + 5 separating dots

		dateTimeFileName = dateTimeFileName.left(6) + "20" + dateTimeFileName.right(11);
		startTimeGlobal = (QDateTime::fromString(dateTimeFileName, "dd.MM.yyyy.HH.mm.ss")).toMSecsSinceEpoch();
		qDebug() << startTimeGlobal;


		return true;

	} while(0);

	return false;
}

void signalFileReader::openAllFiles(){
	for(int i = 0; i < filesAmount; i++){
		QFile* f = new QFile( filename_prefix + QString::number(i + 1) );
		files[i] = f;
		files[i]->open(QIODevice::ReadOnly);
		// files[i] = fopen( (filename_prefix + QString::number(i + 1)).toStdString().c_str(), "r" );
	}
}

void signalFileReader::closeAllFiles(){
	for(int i = 0; i < filesAmount; i++){
		files[i]->close();
	}
}

void signalFileReader::measureAllFiles(){
	blocksAllocationTable->clear();
	for(int i = 0; i < filesAmount; i++){
		measureFile(i);
	}
}

void signalFileReader::resizeAllVectors(){
	blocksAllocationTable->clear();
	files.resize(filesAmount);
	// filesStartTimes.resize(filesAmount);
	filesSizes.resize(filesAmount);
}

void signalFileReader::measureFile(int fileNumber){
	int totalSizeInMeasurments = 0;
	int totalSizeInBytes = 0;

	// take the qFile pointer from array of all files
	QFile* f = files[fileNumber];
	// point at the beggining of the file
	f->seek(0);

	bool firstBlockInFile = true;

	// now read the file while there is smth to read
	while(!f->atEnd()){

		// read the outer block size
		qint64 outerBlockSize = readOuterHeader(f);
		// and get the place where the next block starts from
		// and store it
		qint64 nextOuterBlockPosition = f->pos() - OUTER_HEADER_SIZE + outerBlockSize;


		
		blockPointer p = readInnerHeader(fileNumber);

		// insert info about this block into global table of contents
		blocksAllocationTable->insert( p.timeStart, p );
		





		totalSizeInMeasurments += p.sizeInMeasurments;

		// go to next outer block
		f->seek(nextOuterBlockPosition);
	}

	filesSizes[fileNumber] = totalSizeInMeasurments;
}

qint64 signalFileReader::readOuterHeader(QFile* f){
	QDataStream dataStream(f);
	qint64 res;
	dataStream >> res;
	return res;
}
/*
blockHeader_t* signalFileReader::_readInnerHeader(QFile* f){

	blockHeader_t* blockHeader = new blockHeader_t();
	
	do{
		// qint64 oldPosition = f->pos();

		QDataStream dataStream(f);

		char* str_buffer;
		dataStream >> blockHeader->t0;
		dataStream >> str_buffer;
		for(int i = 0; i < 16; i++){
			blockHeader->caption[i] = str_buffer[i];
		}
		delete[] str_buffer;
		dataStream >> blockHeader->nBM;
		dataStream >> blockHeader->aAnt;
		dataStream >> blockHeader->TypeAnt;
		dataStream >> blockHeader->DataSize;

		// f->seek(oldPosition);

		return blockHeader;

	} while(0);

	return NULL;
}
*/

bool signalFileReader::fileExists(QString filename){
	QFile f(filename);
	if(f.exists()){
		return true;
	} else {
		return false;
	}
}

bool signalFileReader::blockChecksumOk(int blockNumber){
	return true;
}

blockPointer signalFileReader::placeInFile(qint64 time_required){
	
	blockPointer p = (--blocksAllocationTable->upperBound(time_required)).value();
	return p;

	// if( time_required <= p.timeStart + p.sizeInMSecs ){
	// 	return p;
	// } else {
	// 	return p;
	// }

}

bool signalFileReader::skipToNextBlock(QFile* f){
	return f->seek( f->pos() + OUTER_CHECKSUM_SIZE + OUTER_HEADER_SIZE + INNER_HEADER_SIZE );
}

qint64 signalFileReader::startTime(){
	return startTimeGlobal;
}

// inline void signalFileReader::skipUdpPacketNumber(qFile* f){
// 	f->seek(f->pos() + UDP_PACKET_NUMBER_SIZE);
// }
// inline void signalFileReader::skipUdpPacketSysInfo(qFile* f){
// 	f->seek(f->pos() + UDP_PACKET_SYSINFO_SIZE);
// }

int signalFileReader::channelSequently(int i){
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

int signalFileReader::shiftSequently(int i){
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

dualVector<int>* signalFileReader::data(qint64 start, qint64 length){

	char* rawData = new char[TRIPLE_SIZE];
	dualVector<int>* res = new dualVector<int>(GIDROPHONES_AMOUNT, length);

	do{
		
		blockPointer p = placeInFile(start);

		int fileNumber = p.fileNumber;
		qint64 currentTripleInFile = p.positionInFile;
		qint64 measurmentsLeftInBlock = p.sizeInMeasurments;

		QFile* f = files[fileNumber];
		f->seek(currentTripleInFile);
		qint64 done = 0;
		
		
		// create buffer for rawData
		
		while(done < length){

			// take raw data from file
			f->read(rawData, TRIPLE_SIZE);

			// copy it into resulting 2-dim vector
			// here we iterate the consequence numbers of places in rawData and then evaluate
			// the shift (i.e. place) in rawData and channel (i.e. godrophone number) to ehich it corespondes.
			for(int i = 0; i < GIDROPHONES_AMOUNT; i++){
				int shift = shiftSequently(i);
				int channel = channelSequently(i);
				memcpy( &((*res)(channel, done)), rawData + shift, ADC_DIMENTION_BYTES );
			}

			measurmentsLeftInBlock--;

			// while there are measurmentsLeftInBlock we can simply fetch them
			// when measurmentsLeftInBlock == 0
			// we shoul pass to next inner block, or maybe even next file
			if(measurmentsLeftInBlock <= 0){
				f->seek(f->pos() + OUTER_CHECKSUM_SIZE);
				while(f->atEnd()){
					fileNumber++;

					if(fileNumber >= files.size()) break;

					f = files[fileNumber];
					f->seek(0);
				}
				f->seek(f->pos() + OUTER_HEADER_SIZE);
				p = readInnerHeader(fileNumber);
				measurmentsLeftInBlock = p.sizeInMeasurments;
			}

			done++;
		}
		delete[] rawData;
		return res;

	} while(0);
	
	// if error occured => delete all things and 
	delete[] rawData;
	delete res;
	return NULL;
}

blockPointer signalFileReader::readInnerHeader(int fileNumber){

	QFile* f = files[fileNumber];

	double t0;
	char caption[16];
	uint nBM;
	uint aAnt;
	uint TypeAnt;
	uint DataSize;


	QDataStream dataStream(f);
	char* str_buffer;

	// read data from header
	dataStream >> t0;
	dataStream >> str_buffer;
	for(int i = 0; i < 16; i++){
		caption[i] = str_buffer[i];
	}
	delete[] str_buffer;
	dataStream >> nBM;
	dataStream >> aAnt;
	dataStream >> TypeAnt;
	dataStream >> DataSize;

	qint64 blockStartTime = (qint64)(MSECS_IN_SEC * t0); // t0 is of type double (OMG!)
	qint64 innerBlockSize = DataSize;
	qint64 innerBlockDataSize = innerBlockSize - INNER_HEADER_SIZE;
	qint64 innerBlockSizeInMSecs = innerBlockDataSize * FREQUENCY_DISCRETESATION / (TRIPLE_SIZE * MSECS_IN_SEC);
	qint64 innerBlockSizeInMeasurments = innerBlockDataSize / TRIPLE_SIZE;

	blockPointer p = blockPointer(fileNumber, blockStartTime, f->pos(), innerBlockDataSize, innerBlockSizeInMSecs, innerBlockSizeInMeasurments);

	return p;
}

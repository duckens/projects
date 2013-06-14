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




signalFileReader::signalFileReader() : DIFFERENCE_BETWEEN_INT_AND_ADC_IN_BITS(computeDifferenceBetweenIntAndAndInBits(ADC_DIMENTION_BYTES)){
	blocksAllocationTable = new QMap< qint64, blockPointer >;
}

signalFileReader::signalFileReader(signalFileReader const &other_reader) : DIFFERENCE_BETWEEN_INT_AND_ADC_IN_BITS(computeDifferenceBetweenIntAndAndInBits(ADC_DIMENTION_BYTES)){
	blocksAllocationTable = new QMap< qint64, blockPointer >;
}

signalFileReader& signalFileReader::operator= (signalFileReader const &other_reader){
	return *this;
}
signalFileReader::~signalFileReader(){}

int signalFileReader::computeDifferenceBetweenIntAndAndInBits(int adc_dimension_in_bytes){
	return 8 * (sizeof(int) - adc_dimension_in_bytes);
}




bool signalFileReader::open(QString filename){
	// qDebug() << ;
	// qDebug() << "open called";
	
	int last_SeparatorOccurance = filename.lastIndexOf("_");

	do{
		if( last_SeparatorOccurance == -1 ) break;

		filename_prefix = filename;
		filename_prefix.chop(filename.size() - last_SeparatorOccurance - 1);
		// qDebug() << filename_prefix;
		
		filesAmount = 0;
		for(int i = 1;; i++){
			/*qDebug() << "checking file exists " << i;*/
			if(fileExists(filename_prefix + QString::number(i))){
				/*qDebug() << "yes";*/
				filesAmount++;
			} else {
				/*qDebug() << "no";*/
				break;
			}
		}

		if(!filesAmount) break;

		/*qDebug() << "filesAmount = " << filesAmount;*/
		if(!getStartTimeFromFilename()){
			/*qDebug() << "Finally yes!";*/
			break;
		}
		resizeAllVectors();
		openAllFiles();

		/*qDebug() << "openAllFiles done";*/

		measureAllFiles();

		for(int i = 0; i < filesAmount; i++){
			// qDebug() << QString("filesSizes[") + QString::number(i) + "] = " + QString::number(filesSizes[i]);
			// qDebug() << QString("filesStartTimes[") + QString::number(i) + "] = " + QString::number(filesStartTimes[i]);
		}

		/*std::cout << "---------------startTimeGlobal IS---------------" << std::endl;
		std::cout << "startTimeGlobal=" << startTimeGlobal << std::endl;
		std::cout << "---------------startTimeGlobal IS---------------" << std::endl;*/

		return true;
		

	} while(0);

	return false;
}

bool signalFileReader::getStartTimeFromFilename(){
	
	QString dateTimeFileName = filename_prefix;
	
	do{
		// qDebug() << "check !dateTimeFileName.endsWith(\"_\")";
		if(!dateTimeFileName.endsWith("_")) break;

		int prelast_Occurance = dateTimeFileName.lastIndexOf("_", -2);

		// qDebug() << "check prelast_Occurance == -1";
		if(prelast_Occurance == -1) break;

		dateTimeFileName = dateTimeFileName.mid(prelast_Occurance + 1, 17);

		// qDebug() << "check dateTimeFileName.size() != 17";
		// qDebug() << dateTimeFileName;
		if(dateTimeFileName.size() != 17) break; // should be 17 = 6 * 2 letter per muneric field + 5 separating dots

		dateTimeFileName = dateTimeFileName.left(6) + "20" + dateTimeFileName.right(11);
		startTimeGlobal = (QDateTime::fromString(dateTimeFileName, "dd.MM.yyyy.HH.mm.ss")).toMSecsSinceEpoch();
		// qDebug() << startTimeGlobal;


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
	bool foundFirstBlock = false;
	for(int i = 0; i < filesAmount; i++){
		measureFile(i, &foundFirstBlock);
	}
}

void signalFileReader::resizeAllVectors(){
	blocksAllocationTable->clear();
	files.resize(filesAmount);
	// filesStartTimes.resize(filesAmount);
	filesSizes.resize(filesAmount);
}

void signalFileReader::measureFile(int fileNumber, bool* foundFirstBlock){
	int totalSizeInMeasurments = 0;
	int totalSizeInBytes = 0;

	// take the qFile pointer from array of all files
	QFile* f = files[fileNumber];
	// point at the beggining of the file
	f->seek(0);

	bool firstBlockInFile = true;

	/* DEBUG */
	int i = 0;

	// now read the file while there is smth to read
	while(!f->atEnd()){

		// read the outer block size
		qint64 outerBlockSize = readOuterHeader(f);
		/*std::cout << "outerBlockSize = " << outerBlockSize << std::endl;*/
		// and get the place where the next block starts from
		// and store it
		qint64 nextOuterBlockPosition = f->pos() - OUTER_HEADER_SIZE + outerBlockSize;
		/*std::cout << "nextOuterBlockPosition = " << nextOuterBlockPosition << std::endl;*/


		
		blockPointer p = readInnerHeader(fileNumber);

		if(!(*foundFirstBlock)){
			/*std::cout << "-----------startTimeGlobal-----------" << std::endl;
			std::cout << "startTimeGlobal=" << (qint64)(p.timeStart) << std::endl;
			std::cout << "-----------startTimeGlobal-----------" << std::endl;*/
			startTimeGlobal = (qint64)(p.timeStart);
			*foundFirstBlock = true;
		}

		/* DEBUG */
		/*std::cout << "-----------INSERT-----------" << std::endl;
		std::cout << "p.fileNumber=" << p.fileNumber << std::endl;
		std::cout << "p.timeStart=" << p.timeStart << std::endl;
		std::cout << "p.positionInFile=" << p.positionInFile << std::endl;
		std::cout << "p.sizeInBytes=" << p.sizeInBytes << std::endl;
		std::cout << "p.sizeInMSecs=" << p.sizeInMSecs << std::endl;
		std::cout << "p.sizeInMeasurments=" << p.sizeInMeasurments << std::endl;
		std::cout << "-----------INSERT-----------" << std::endl;
*/
		// insert info about this block into global table of contents
		blocksAllocationTable->insert( p.timeStart, p );
		





		totalSizeInMeasurments += p.sizeInMeasurments;

		// go to next outer block
		f->seek(nextOuterBlockPosition);

		/* DEBUG */
		// i++;
		// if(i > 10) break;
	}

	filesSizes[fileNumber] = totalSizeInMeasurments;
}

qint64 signalFileReader::readOuterHeader(QFile* f){
	// QDataStream dataStream(f);
	qint64 res;
	// dataStream >> res;
	f->read((char*)(&res), OUTER_HEADER_SIZE);
	return res;
}

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
	
	// std::cout << "placeInFile(" << time_required << ")" << std::endl;

	/* DEBUG */
	/*for(QMap< qint64, blockPointer >::iterator it = blocksAllocationTable->begin(); it != blocksAllocationTable->end(); ++it){
		blockPointer p = *it;
		std::cout << "--------------------" << std::endl;
		std::cout << "p.fileNumber=" << p.fileNumber << std::endl;
		std::cout << "p.timeStart=" << p.timeStart << std::endl;
		std::cout << "p.positionInFile=" << p.positionInFile << std::endl;
		std::cout << "p.sizeInBytes=" << p.sizeInBytes << std::endl;
		std::cout << "p.sizeInMSecs=" << p.sizeInMSecs << std::endl;
		std::cout << "p.sizeInMeasurments=" << p.sizeInMeasurments << std::endl;
		std::cout << "--------------------" << std::endl;
	}*/

	blockPointer p = (--blocksAllocationTable->upperBound(time_required)).value();

	/*std::cout << "--------------------" << std::endl;
	std::cout << "--------------------" << std::endl;
	std::cout << "FOUND!! p:" << std::endl;
	std::cout << "--------------------" << std::endl;
	std::cout << "--------------------" << std::endl;
	std::cout << "p.fileNumber=" << p.fileNumber << std::endl;
	std::cout << "p.timeStart=" << p.timeStart << std::endl;
	std::cout << "p.positionInFile=" << p.positionInFile << std::endl;
	std::cout << "p.sizeInBytes=" << p.sizeInBytes << std::endl;
	std::cout << "p.sizeInMSecs=" << p.sizeInMSecs << std::endl;
	std::cout << "p.sizeInMeasurments=" << p.sizeInMeasurments << std::endl;
	std::cout << "--------------------" << std::endl;*/

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

int signalFileReader::sequentNumberFromGidrophoneNumber(int gidrophoneNumber){
	int shiftForPacketInTriple;
	int res;
	if(gidrophoneNumber <= 59){
		res = 59 - gidrophoneNumber;
	} else if(gidrophoneNumber >= 118){
		res = gidrophoneNumber - 118 + 60;
	} else {
		if(gidrophoneNumber >= 89){
			res = gidrophoneNumber - 89 + 120;
		} else {
			res = 149 - (gidrophoneNumber - 88);
		}
	}
	return res;
}

std::vector<int>* signalFileReader::data(qint64 start, qint64 length, int gidrophoneNumber){

	std::vector<int>* res = new std::vector<int>;
	int i = sequentNumberFromGidrophoneNumber(gidrophoneNumber);
	res->reserve(length);

	do{
		
		blockPointer p = placeInFile(start);

		int fileNumber = p.fileNumber;
		qint64 measurmentsToSkip = (start - p.timeStart) * FREQUENCY_DISCRETESATION / MSECS_IN_SEC;

		QFile* f = files[fileNumber];
		qint64 positionInFile = p.positionInFile + measurmentsToSkip * TRIPLE_SIZE;
		f->seek(positionInFile);

		qint64 measurmentsLeftInBlock = p.sizeInMeasurments - measurmentsToSkip;

		qint64 done = 0;
		
		int shift = shiftSequently(i);
		int channel = channelSequently(i);

		int buffer;

		/* DEBUG */
		/*std::cout << "p.fileNumber=" << p.fileNumber << std::endl;
		std::cout << "p.timeStart=" << p.timeStart << std::endl;
		std::cout << "p.positionInFile=" << p.positionInFile << std::endl;
		std::cout << "p.sizeInBytes=" << p.sizeInBytes << std::endl;
		std::cout << "p.sizeInMSecs=" << p.sizeInMSecs << std::endl;
		std::cout << "p.sizeInMeasurments=" << p.sizeInMeasurments << std::endl;

		std::cout << "measurmentsLeftInBlock=" << measurmentsLeftInBlock << std::endl;*/

		while(done < length){


			f->seek(positionInFile + shift);
			buffer = 0;
			f->read((char*)(&buffer), ADC_DIMENTION_BYTES);
			positionInFile += TRIPLE_SIZE;

			buffer >> DIFFERENCE_BETWEEN_INT_AND_ADC_IN_BITS;
			res->push_back(buffer);

			measurmentsLeftInBlock--;

			// while there are measurmentsLeftInBlock we can simply fetch them
			// when measurmentsLeftInBlock == 0
			// we shoul pass to next inner block, or maybe even next file
			if(measurmentsLeftInBlock <= 0){
				f->seek(positionInFile + OUTER_CHECKSUM_SIZE);
				while(f->atEnd()){
					fileNumber++;
					if(fileNumber >= files.size()) break;
					f = files[fileNumber];
					f->seek(0);
				}
				f->seek(f->pos() + OUTER_HEADER_SIZE);
				p = readInnerHeader(fileNumber);
				measurmentsLeftInBlock = p.sizeInMeasurments;
				positionInFile = p.positionInFile;
			}

			done++;
		}

		/*std::cout << "[";
		for(int i = 0; i < res->size(); i++){
			std::cout << (*res)[i] << ", ";
		}
		std::cout << "]" << std::endl;*/
		return res;

	} while(0);
	
	// if error occured => delete all things and 
	delete res;
	return NULL;
}

dualVector<int>* signalFileReader::data(qint64 start, qint64 length){

	// create buffer for rawData
	char* rawData = new char[TRIPLE_SIZE];

	// create future result
	dualVector<int>* res = new dualVector<int>(GIDROPHONES_AMOUNT, length);

	do{
		
		blockPointer p = placeInFile(start);

		int fileNumber = p.fileNumber;
		qint64 measurmentsToSkip = (start - p.timeStart) * FREQUENCY_DISCRETESATION / MSECS_IN_SEC;

		QFile* f = files[fileNumber];
		qint64 positionInFile = p.positionInFile + measurmentsToSkip * TRIPLE_SIZE;
		f->seek(positionInFile);

		qint64 measurmentsLeftInBlock = p.sizeInMeasurments - measurmentsToSkip;

		qint64 done = 0;
		
		
		
		while(done < length){



			f->seek(positionInFile);
			// take raw data from file
			f->read(rawData, TRIPLE_SIZE);
			positionInFile += TRIPLE_SIZE;

			measurmentsLeftInBlock--;

			// copy it into resulting 2-dim vector
			// here we iterate the consequence numbers of places in rawData and then evaluate
			// the shift (i.e. place) in rawData and channel (i.e. godrophone number) to ehich it corespondes.
			for(int i = 0; i < GIDROPHONES_AMOUNT; i++){
				int shift = shiftSequently(i);
				int channel = channelSequently(i);
				memcpy( &((*res)(channel - 1, done)), rawData + shift, ADC_DIMENTION_BYTES );
				(*res)(channel, done) >> DIFFERENCE_BETWEEN_INT_AND_ADC_IN_BITS;
			}


			// while there are measurmentsLeftInBlock we can simply fetch them
			// when measurmentsLeftInBlock == 0
			// we shoul pass to next inner block, or maybe even next file
			if(measurmentsLeftInBlock <= 0){
				f->seek(positionInFile + OUTER_CHECKSUM_SIZE);
				while(f->atEnd()){
					fileNumber++;
					if(fileNumber >= files.size()) break;
					f = files[fileNumber];
					f->seek(0);
				}
				f->seek(f->pos() + OUTER_HEADER_SIZE);
				p = readInnerHeader(fileNumber);
				measurmentsLeftInBlock = p.sizeInMeasurments;
				positionInFile = p.positionInFile;
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
	char caption[CAPTION_LENGTH];
	uint nBM;
	uint aAnt;
	uint TypeAnt;
	uint DataSize;


	// QDataStream dataStream(f);
	char* str_buffer;


	// read data from header
	f->read((char*)(&t0), sizeof(double));
	f->seek(f->pos() + CAPTION_LENGTH);
	// f->read(str_buffer, CAPTION_LENGTH);
	// std::cout << "read str_buffer=" << str_buffer << std::endl;
	//delete[] str_buffer;
	f->read((char*)(&nBM), sizeof(uint));
	f->read((char*)(&aAnt), sizeof(uint));
	f->read((char*)(&TypeAnt), sizeof(uint));
	f->read((char*)(&DataSize), sizeof(uint));

	

	// dataStream >> t0;
	// dataStream >> str_buffer;
	// for(int i = 0; i < 16; i++){
	// 	caption[i] = str_buffer[i];
	// }
	// delete[] str_buffer;
	// dataStream >> nBM;
	// dataStream >> aAnt;
	// dataStream >> TypeAnt;
	// dataStream >> DataSize;

	qint64 blockStartTime = (qint64)(MSECS_IN_SEC * t0); // t0 is of type double (OMG!)
	qint64 innerBlockSize = DataSize;
	qint64 innerBlockDataSize = innerBlockSize - INNER_HEADER_SIZE;
	qint64 innerBlockSizeInMSecs = innerBlockDataSize * FREQUENCY_DISCRETESATION / (TRIPLE_SIZE * MSECS_IN_SEC);
	qint64 innerBlockSizeInMeasurments = innerBlockDataSize / TRIPLE_SIZE;
	

	blockPointer p = blockPointer(fileNumber, blockStartTime, f->pos(), innerBlockDataSize, innerBlockSizeInMSecs, innerBlockSizeInMeasurments);


	return p;
}

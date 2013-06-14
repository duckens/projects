#include "sourceFileReader.h"
#include <iostream>

sourceFileReader::sourceFileReader(){
	f = NULL;
}

sourceFileReader::sourceFileReader(sourceFileReader const &other){
	f = NULL;
}

sourceFileReader& sourceFileReader::operator= (sourceFileReader const &other){
	f = other.f;
}

sourceFileReader::~sourceFileReader(){

}

void sourceFileReader::setBasePoint(geoCoord newBasePoint){
	coordCalc.setBasePoint(newBasePoint);
}

void sourceFileReader::setBasePoint(double latitude, double longitude){
	coordCalc.setBasePoint(latitude, longitude);
}

position sourceFileReader::coordinatesToBasePoint(double x, double y){
	return coordCalc.geoToDecart(x, y);
}

bool sourceFileReader::open(QString filename){
	do{
		if(f != NULL){
			f->close();
			delete f;
		}
		f = new QFile(filename);
		if(!f->exists()) break;
		f->open(QIODevice::ReadOnly);
		return true;
		
	} while(0);

	return false;
}


QMap<qint64, position>* sourceFileReader::data(){

	QMap<qint64, position>* res = new QMap<qint64, position>();

	f->seek(0);
	
	int year;
	int month;
	int day;

	int hour;
	int min;
	int sec;

	double x;
	double y;
	double z;

	while(!f->atEnd()){
		

		f->read((char*)(&year), INT_SIZE);
		f->read((char*)(&month), INT_SIZE);
		f->read((char*)(&day), INT_SIZE);
		f->read((char*)(&hour), INT_SIZE);
		f->read((char*)(&min), INT_SIZE);
		f->read((char*)(&sec), INT_SIZE);
		
		f->read((char*)(&x), DOUBLE_SIZE);
		f->read((char*)(&y), DOUBLE_SIZE);
		f->read((char*)(&z), DOUBLE_SIZE);

		year += 2000;

		/*
		std::cout << "\tx=" << x << std::endl;
		std::cout << "\ty=" << y << std::endl;
		std::cout << "\tz=" << z << std::endl;
		*/

		geoCoord point = coordCalc.geoCoordFromLatLong(x, y);
		
		/*
		std::cout << "\tthis point:" << std::endl;
		std::cout << "\tlatitude_deg = " << point.latitude_deg << std::endl;
		std::cout << "\tlatitude_min = " << point.latitude_min << std::endl;
		std::cout << "\tlatitude_sec = " << point.latitude_sec << std::endl;
		std::cout << "\tlongitude_deg = " << point.longitude_deg << std::endl;
		std::cout << "\tlongitude_min = " << point.longitude_min << std::endl;
		std::cout << "\tlongitude_sec = " << point.longitude_sec << std::endl;

		std::cout << "\tbase point:" << std::endl;
		std::cout << "\tlatitude_deg = " << coordCalc.getBasePoint().latitude_deg << std::endl;
		std::cout << "\tlatitude_min = " << coordCalc.getBasePoint().latitude_min << std::endl;
		std::cout << "\tlatitude_sec = " << coordCalc.getBasePoint().latitude_sec << std::endl;
		std::cout << "\tlongitude_deg = " << coordCalc.getBasePoint().longitude_deg << std::endl;
		std::cout << "\tlongitude_min = " << coordCalc.getBasePoint().longitude_min << std::endl;
		std::cout << "\tlongitude_sec = " << coordCalc.getBasePoint().longitude_sec << std::endl;
		*/
		
		qint64 timestamp = QDateTime( QDate(year, month, day), QTime(hour, min, sec) ).toMSecsSinceEpoch();

		res->insert(timestamp, coordinatesToBasePoint(x, y));
		

		/*
		std::cout << std::endl;
		std::cout << "\ttimestamp=" << timestamp << std::endl;
		std::cout << "\tres->size()=" << res->size() << std::endl;
		std::cout << std::endl << std::endl << std::endl << std::endl;
		*/
	}

	return res;
}




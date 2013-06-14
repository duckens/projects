#ifndef COURCE_FILE_READER_H
#define COURCE_FILE_READER_H

#include <vector>
#include <QtCore>
#include "classes/position.h"
#include "coordinateCalculator.h"


#define INT_SIZE 4
#define DOUBLE_SIZE 8

class sourceFileReader{
	
	private:

		QFile* f;

		coordinateCalculator coordCalc;

		position coordinatesToBasePoint(double x, double y);

	public:

		sourceFileReader();
		sourceFileReader(sourceFileReader const &other);
		sourceFileReader& operator= (sourceFileReader const &other);
		~sourceFileReader();

		void setBasePoint(geoCoord newBasePoint);
		void setBasePoint(double latitude, double longitude);

		bool open(QString filename);

		QMap<qint64, position>* data();

};

#endif

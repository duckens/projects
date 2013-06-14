#include <iostream>
#include <QtCore>

#include "classes/position.h"
#include "sourceFileReader.h"

int main(){
	
	sourceFileReader reader;

	std::cout << reader.open("stream.out") << std::endl;

	geoCoord SPb = {
		59,
		53,
		39.0,
		30,
		15,
		51.0
	};
	reader.setBasePoint(SPb);
	
	QMap<qint64, position>* data = reader.data();

	
	std::cout << "data[" << data->size() << "] = [" << std::endl;
	for(QMap<qint64, position>::const_iterator it = data->begin(); it != data->end(); ++it){
		std::cout << "\t" << it.key() << "-->\t(" << it.value().x << ", " << it.value().y << ", " << it.value().z << ")" << std::endl;
	}
	std::cout << "]" << std::endl;

	return 0;
}
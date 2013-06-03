#include "signalFileReader.h"

int main(){
	
	signalFileReader reader;

	qDebug() <<	reader.open("BM_1_DPLA_1_14.11.12.13.44.36_1");

	return 0;
}
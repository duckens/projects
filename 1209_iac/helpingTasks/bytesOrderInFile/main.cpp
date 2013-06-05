#include <QtCore>

#include <iostream>
#include <cstdio>

int main(){
	
	double t0 = 1.3 * 1000000000;
	int i = 1;
	uint ui = 1;


	std::cout << "Writing common file..";


	FILE* f = fopen("stdio.out", "w");
	fwrite(&t0, 1, sizeof(double), f);
	fwrite(&i, 1, sizeof(int), f);
	fwrite(&ui, 1, sizeof(uint), f);
	fclose(f);


	std::cout << "done" << std::endl;
	std::cout << "Writing QFile.." << std::endl;


	QFile* qf = new QFile(QString("QFile.out"));
	qf->open(QIODevice::WriteOnly);
	int totalBufferSize = sizeof(double) + sizeof(int) + sizeof(uint);
	char* buffer = new char[totalBufferSize];
	memcpy(buffer, &t0, sizeof(double));
	memcpy(buffer + sizeof(double), &i, sizeof(int));
	memcpy(buffer + sizeof(double) + sizeof(int), &ui, sizeof(uint));
	qf->write(buffer, totalBufferSize);
	qf->close();
	delete qf;
	delete[] buffer;


	std::cout << "done" << std::endl;
	std::cout << "Writing using stream.." << std::endl;


	QFile* qf_stream = new QFile(QString("QDataStream.out"));
	qf_stream->open(QIODevice::WriteOnly);
	QDataStream stream(qf_stream);
	stream << t0;
	stream << i;
	stream << ui;
	qf_stream->close();
	delete qf_stream;

	std::cout << "done" << std::endl;

	return 0;
}
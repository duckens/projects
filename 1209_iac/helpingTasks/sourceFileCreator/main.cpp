#include "decoder.h"
#include <cstdio>
#include <iostream>


int main(){
	

	/*
	Read stream. I'm reading from file
	*/
	std::cout << "Reading \"stream.in\"" << std::endl << std::endl;
	FILE* f;
	FILE* f_out;
	std::vector<unsigned char> stream;
	/*
	// Read from port
	//Необходима настройка порта:
	//	Скорость : 38400
	//	Бит данных : 8
	//	Стоп бит : 1
	//	Бит четности : нет
	//Запускать в этом случае нужно c правами чтения из порта.
	f = fopen("/dev/ttyS0", "rb");
	for(int i = 0; i < 1000; i++){
		unsigned char buffer;
		fread(&buffer, sizeof(unsigned char), 1, f);
		stream.push_back(buffer);
	}
	*/
	// Read from file
	f = fopen("stream.in", "rb");
	f_out = fopen("stream.out", "wb");
	while(!feof(f)){
		unsigned char buffer;
		fread(&buffer, sizeof(unsigned char), 1, f);
		stream.push_back(buffer);
	}


	/*
	Create parser and give data to it
	*/
	std::cout << "Creating parser" << std::endl;
	timeSystem::streamParser parser = timeSystem::streamParser();
	std::cout << "parse()" << std::endl;
	parser.parse(&(stream[0]), stream.size());


	/*
	Create pointer to array for parser answer
	*/
	timeSystem::dataArray* array;


	/*
	Looking for messages with ID = 149.
	*/
	std::cout << "Looking for mesasage with ID = 149:" << std::endl << std::endl;
	for(int i = 0; i < parser.parsedSize(); i++){
		array = parser.getMessage(i);

		std::cout << "Message " << i << "-th by order, ID = " << array->getID() << " .. ";

		if(array->getID() == 149){
			std::cout << "Found!" << std::endl;
		} else {
			std::cout << "don\'t like it" << std::endl;
			continue;
		}


		/*
		Show message with ID=149 on screen
		*/
		std::cout << std::endl << std::endl;
		std::cout << "array.size() = " << array->size() << std::endl;
		std::cout << "array = [ " << std::endl;
		for(int i = 0; i < array->size(); i++){
			//std::cout << "i=" << i << " ";
			switch((*array)[i].getType()){

				case 0:
					std::cout << "\tDon\'t know" << " " << std::endl;
					break;

				case 1:
					std::cout << "\tint:\t" << (int)(unsigned char)(*array)[i] << " " << std::endl;
					break;

				case 2:
					std::cout << "\tword:\t" << (short int)(*array)[i] << " " << std::endl;
					break;

				case 3:
					std::cout << "\ttime:\t" << ((timeSystem::time*)(*array)[i])->toString() << " " << std::endl;
					break;

				case 4:
					std::cout << "\tfloat:\t" << (float)(*array)[i] << " " << std::endl;
					break;

				case 5:
					std::cout << "\tdouble:\t" << (double)(*array)[i] << " " << std::endl;
					break;

			}



		}

		std::cout << "]" << std::endl << std::endl;

		timeSystem::time* date = ((timeSystem::time*)(*array)[7]);
		timeSystem::time* time_of_day = ((timeSystem::time*)(*array)[5]);

		int year = date->data(0);
		int month = date->data(1);
		int day = date->data(2);

		int hour = time_of_day->data(0);
		int min = time_of_day->data(1);
		int sec = time_of_day->data(2);

		std::cout << year << " " << month << " " << day << std::endl;
		std::cout << hour << " " << min << " " << sec << std::endl;

		double x = (double)(*array)[8];
		double y = (double)(*array)[9];
		double z = (double)(*array)[10];
		
		std::cout << x << " " << y << " " << z << std::endl;

		fwrite(&year, 1, sizeof(int), f_out);
		fwrite(&month, 1, sizeof(int), f_out);
		fwrite(&day, 1, sizeof(int), f_out);
		fwrite(&hour, 1, sizeof(int), f_out);
		fwrite(&min, 1, sizeof(int), f_out);
		fwrite(&sec, 1, sizeof(int), f_out);

		fwrite(&x, 1, sizeof(double), f_out);
		fwrite(&y, 1, sizeof(double), f_out);
		fwrite(&z, 1, sizeof(double), f_out);
		
	}
	
	fclose(f);
	fclose(f_out);
	return 0;
}

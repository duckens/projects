#ifndef DECODER_H
#define DECODER_H

#include <cstring> // for memcpy
#include <string>
#include <vector>
#include <map>


namespace timeSystem{


	/*
	Номера типов даных в массиве, задающем формат
	*/
	const int Unknown = 0;
	const int UChar = 1;
	const int Word = 2;
	const int Time = 3;
	const int Float = 4;
	const int Double = 5;
	
	/*
	Длины типов в байтах
	*/
	const int typeSize[6] = {
		0,
		1,
		2,
		4,
		4,
		8
	};
	

	/*
	Класс для хранения времени, дробной части времени и даты. Содержит 4 целых числа.
	Это соответственно чч:мм:сс:резерв, гг:мм:дд:резерв. Что лежит в дробной части времени - см. документацию.
	*/
	class time{
		private:
		
			int _data[4];

		public:

			time(unsigned char* data_input);
			~time();
			std::string toString();
			int data(int i);
	};
	


	/*
	Класс для хранения одного параметра из сообщения.
	Объект этого класса возвращается при обращении к элементу массива разобранного сообщения

	Позволяет хранить параметры типов:
		unsigned char
		short int
		float
		double
		time
	Для этох типов перегружены операторы приведения типа.
	*/
	class dataElement{
		private:

			int type;
			unsigned char* data;
		
		public:
		
			dataElement();
			dataElement(dataElement const & elem);
			dataElement(unsigned char* data_input, int type_input);
			
			void swap(dataElement & elem);
			dataElement& operator=(dataElement const & elem);
			
			int getType();
			
			operator float() const;
			operator double() const;
			operator short int() const;
			operator unsigned char() const;
			operator time*() const;
			
	};



	/*
	Класс хранящий информацию о формате сообщения в зависимости
	от идентификатора ID этого сообщения.
	Создается один экземпляр этого класса. В конструкторе чиатется файл "masks.dat",
	после чего экземпляр класса может отвечать на запросы формата по известному ID.
	*/
	class messageMasks{

		private:

			std::map<int, std::vector<int>*> masks;

		public:
			messageMasks();
			~messageMasks();

			std::vector<int>* find(int ID);

	};


	
	/*
	Класс, хранящий одно сообщение от системы времени.
	Перегружен оператор [], который возвращает dataElement - один параметр.
	Например dataArray[2] вернет параметр, идущий 2-м по счету (начиная с 0) в сообщении, т.е. ID.
	*/
	class dataArray{
	
		private:

			messageMasks* masks;
			
			std::vector<int> format;
			std::vector<dataElement> data;
			int ID;
			
		public:
			
			dataArray(unsigned char* data_input, int size_input, std::vector<int>* format_input, messageMasks* masks_input);
			~dataArray();
			
			int getID();
			int size();
			dataElement operator[](int number);

	};


	/*
	Главный класс. Объект этого класса нужно создать в теле программы, занимающейся обработкой времени.
	После создания, объекту класса можно передавать массив байтов, после чего запрашивать отдельные сообщения из этого массива.
	*/
	class streamParser{
		
		private:

			messageMasks* masks;
			std::vector<dataArray>* messages_parsed;
			std::vector<unsigned char>* dataStream;
			std::vector< std::vector<unsigned char> >* messages;
			int dataStream_size;

		public:

			streamParser();
			~streamParser();
			void parse(unsigned char* data_input, int data_input_size);
			dataArray* getMessage(int i = 0);
			int parsedSize();

	};
}
#endif
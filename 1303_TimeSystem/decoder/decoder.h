#ifndef DECODER_H
#define DECODER_H

#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <ctime>


namespace timeSystem{

	const int Unknown = 0;
	const int UChar = 1;
	const int Word = 2;
	const int Time = 3;
	const int Float = 4;
	const int Double = 5;
	
	const int typeSize[6] = {
		0,
		1, 
		2,
		4,
		4,
		8
	};
	
	
	const std::vector<int> masks[1] = {std::vector<int>()};
	
	
	
	
	class dataElement{
		private:
		
			unsigned char* data;
			int type;
			
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
			operator std::vector<int>*() const;
			
			
			
	};
	
	class dataArray{
	
		private:
		
			
			std::vector<int> format;
			std::vector<dataElement> data;
			int ID;
			
		public:
			
			static std::vector<int> masks[1];
			
			dataArray(unsigned char* data_input, int size_input, std::vector<int>* format_input);
			~dataArray();
			
			int getID();
			dataElement operator[](int number);
	};
	
	
}

#endif
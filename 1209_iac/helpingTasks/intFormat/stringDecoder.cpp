#include <cstdio>
#include <iostream>
#include <string>
#include <math.h>
#include <sstream>

#include "stringDecoder.h"




/* IMPORTANT!! for all such things char MUST BE UNSIGNED!! */
std::string charToBits(unsigned char ch){
	std::string str = "";
	int charater = (int)ch;
	for(int i = 0; i < 8; i++){
		if(charater % 2){
			str += "1";
		} else {
			str += "0";
		}
		charater /= 2;
	}
	std::string res = "";
	for(int i = 7; i >= 0; i--){
		res += str[i];
	}
	return res;
}

unsigned char bitsToChar(std::string bits){
	unsigned char res = 0;
	for(int i = 0; i < 8; i++){
		if(bits[i] == '1'){
			res += 1<<(7 - i);
		}
	}
	return res;
}


std::string floatToBits(float number){
	int size_in_bytes = sizeof(float);
	unsigned char* ch = (unsigned char*)(&number);
	std::string res = "";
	for(int i = size_in_bytes - 1; i >= 0; i--){
		res += charToBits(ch[i]);
	}
	/*
	std::cout << "res = " << res << std::endl;
	*/
	return res;
}

std::string intToBits(int number){
	int size_in_bytes = sizeof(int);
	unsigned char* ch = (unsigned char*)(&number);
	std::string res = "";
	for(int i = size_in_bytes - 1; i >= 0; i--){
		res += charToBits(ch[i]);
	}
	return res;
}

std::string longLongIntToBits(long long int number){
	int size_in_bytes = sizeof(long long int);
	unsigned char* ch = (unsigned char*)(&number);
	std::string res = "";
	for(int i = size_in_bytes - 1; i >= 0; i--){
		res += charToBits(ch[i]);
	}
	return res;
}

std::string doubleToBits(double number){
	int size_in_bytes = sizeof(double);
	unsigned char* ch = (unsigned char*)(&number);
	std::string res = "";
	for(int i = size_in_bytes - 1; i >= 0; i--){
		res += charToBits(ch[i]);
	}
	/*
	std::cout << "res = " << res << std::endl;
	*/
	return res;
}

float bitsToFloat(std::string str){
	int sign;
	if(str[0] == '0'){
		sign = 1;
	} else {
		sign = -1;
	}
	std::string mantisa_str = str.substr(9, 23);
	std::string exp_str = str.substr(1, 8);
	
	double mantisa = 1.0;
	for(int i = 0; i < 23; i++){
		int bit;
		if(mantisa_str[i] == '1'){
			bit = 1;
		} else {
			bit = 0;
		}
		mantisa += bit * pow(2, -(i + 1));
	}
	
	double exp = 0.0;
	for(int i = 0; i < 8; i++){
		int bit;
		if(exp_str[i] == '1'){
			bit = 1;
		} else {
			bit = 0;
		}
		exp += bit * pow(2, 7 - i);
	}
	exp = exp - 127;
	
	double res = sign * mantisa * pow(2, exp);
	
	/*
	std::cout << "INPUT = " << str << std::endl;
	if(sign > 0){
		std::cout << "sign = +" << std::endl;
	} else {
		std::cout << "sign = -1" << std::endl;
	}
	std::cout << "mantisa_str = " << mantisa_str << std::endl;
	std::cout << "exp_str = " << exp_str << std::endl;
	std::cout << "mantisa = " << mantisa << std::endl;
	std::cout << "exp = " << exp << std::endl;
	std::cout << "res = " << res << std::endl;
	*/
	
	return res;
}

float bitsToDouble(std::string str){
	int sign;
	if(str[0] == '0'){
		sign = 1;
	} else {
		sign = -1;
	}
	std::string mantisa_str = str.substr(12, 52);
	std::string exp_str = str.substr(1, 11);
	
	double mantisa = 1.0;
	for(int i = 0; i < 52; i++){
		int bit;
		if(mantisa_str[i] == '1'){
			bit = 1;
		} else {
			bit = 0;
		}
		mantisa += bit * pow(2, -(i + 1));
	}
	
	double exp = 0.0;
	for(int i = 0; i < 11; i++){
		int bit;
		if(exp_str[i] == '1'){
			bit = 1;
		} else {
			bit = 0;
		}
		exp += bit * pow(2, 10 - i);
	}
	exp = exp - 1023;
	
	double res = sign * mantisa * pow(2, exp);
	
	/*
	std::cout << "INPUT = " << str << std::endl;
	if(sign > 0){
		std::cout << "sign = +" << std::endl;
	} else {
		std::cout << "sign = -1" << std::endl;
	}
	std::cout << "mantisa_str = " << mantisa_str << std::endl;
	std::cout << "exp_str = " << exp_str << std::endl;
	std::cout << "mantisa = " << mantisa << std::endl;
	std::cout << "exp = " << exp << std::endl;
	std::cout << "res = " << res << std::endl;
	*/
	
	return res;
}

std::string hexToBits(std::string hexString){
	int hexString_size = hexString.size();
	char buffer[] = "f";
	std::string res = "";
	for(int i = 0; i < hexString_size; i++){
		if(hexString[i] == ' ') continue;
		buffer[0] = hexString[i];
		//std::cout << "buffer = " << buffer << std::endl;
		unsigned char ch;
		
		sscanf(buffer, "%x", &ch);
		
		//std::cout << "ch = " << ch << std::endl;
		res += charToBits(ch).substr(4, 4);
	}
	//std::cout << "res = " << res << std::endl;
	
	return res;
	
	
}

double decode_EFP(std::string hex_str){
	int hex_str_size = hex_str.size();
	std::string str_helper;
	for(int i = 0; i < hex_str_size; i++){
		if(hex_str[i] == ' ') continue;
		str_helper += hex_str[i];
	}
	
	std::string res;
	
	for(int i = str_helper.size() - 2; i >= 0; i -= 2){
		res += str_helper[i];
		res += str_helper[i + 1];
	}

	return bitsToDouble(hexToBits(res));
	
}

float decode_FP(std::string hex_str){
	int hex_str_size = hex_str.size();
	std::string str_helper;
	for(int i = 0; i < hex_str_size; i++){
		if(hex_str[i] == ' ') continue;
		str_helper += hex_str[i];
	}
	
	std::string res;
	
	for(int i = str_helper.size() - 2; i >= 0; i -= 2){
		res += str_helper[i];
		res += str_helper[i + 1];
	}

	return bitsToFloat(hexToBits(res));
	
}

std::string decode_time(std::string hex_str){
	int hex_str_size = hex_str.size();
	std::string str_helper;
	for(int i = 0; i < hex_str_size; i++){
		if(hex_str[i] == ' ') continue;
		str_helper += hex_str[i];
	}
	std::string res = "";
	for(int i = 0; i < str_helper.size(); i += 2){
		std::string buffer = "";
		buffer += str_helper[i];
		buffer += str_helper[i + 1];
		int n;
		sscanf(buffer.c_str(), "%x", &n);
		std::stringstream ss;
		ss << n;
		ss >> buffer;
		if(i > 0){
			res += ":";
		}
		res += buffer;
	}
	return res;
}

int decode_word(std::string hex_str){
	int hex_str_size = hex_str.size();
	std::string str_helper;
	for(int i = 0; i < hex_str_size; i++){
		if(hex_str[i] == ' ') continue;
		str_helper += hex_str[i];
	}
	int res = 0;
	for(int i = 0; i < str_helper.size(); i += 2){
		std::string buffer = "";
		buffer += str_helper[i];
		buffer += str_helper[i + 1];
		int n;
		sscanf(buffer.c_str(), "%x", &n);
		res += n << (i * 4);
	}
	return res;
}

int decode_char(std::string hex_str){
	int hex_str_size = hex_str.size();
	std::string str_helper;
	for(int i = 0; i < hex_str_size; i++){
		if(hex_str[i] == ' ') continue;
		str_helper += hex_str[i];
	}
	int n;
	sscanf(str_helper.c_str(), "%x", &n);
	
	return n;
}

void parseFormat(std::string data, std::string format){
	int data_size = data.size();
	std::string hex_str = "";
	
	for(int i = 0; i < data_size; i++){
		if(data[i] == ' ') continue;
		hex_str += data[i];
	}
	/*
	format keys:
	
	c = unsugned char
	w = word
	i = int
	l = long long int
	f = float
	d = double
	*/
	int format_size = format.size();
	std::string buffer;
	for(int i = 0; i < format_size; i++){
		
		switch(format[i]){
			case 'f':
				buffer = hex_str.substr(0, 8);
				hex_str = hex_str.substr(8, hex_str.size() - 8);
				std::cout << "float = " << decode_FP(buffer) << std::endl;
				break;
			case 'd':
				buffer = hex_str.substr(0, 16);
				hex_str = hex_str.substr(16, hex_str.size() - 16);
				std::cout << "double = " << decode_EFP(buffer) << std::endl;
				break;
			case 'c':
				buffer = hex_str.substr(0, 2);
				hex_str = hex_str.substr(2, hex_str.size() - 2);
				std::cout << "char = " << decode_char(buffer) << std::endl;
				break;
			case 'w':
				buffer = hex_str.substr(0, 4);
				hex_str = hex_str.substr(4, hex_str.size() - 4);
				std::cout << "word = " << decode_word(buffer) << std::endl;
				break;
			case 't':
				buffer = hex_str.substr(0, 8);
				hex_str = hex_str.substr(8, hex_str.size() - 8);
				std::cout << "time = " << decode_time(buffer) << std::endl;
				break;
			
		}
	}
	
}
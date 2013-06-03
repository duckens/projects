#include <cstdio>
#include <iostream>
#include <string>



std::string bitsOfChar(char ch){
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

void printBitsOfNumber(double number){
	int size_in_bytes = sizeof(double);
	char* ch = (char*)(&number);
	std::cout << number << " =\t";
	for(int i = size_in_bytes - 1; i >= 0; i--){
		std::cout << bitsOfChar(ch[i]) << " ";
	}
	std::cout << std::endl;
}

void printBitsOfNumber(float number){
	int size_in_bytes = sizeof(float);
	char* ch = (char*)(&number);
	std::cout << number << " =\t";
	for(int i = size_in_bytes - 1; i >= 0; i--){
		std::cout << bitsOfChar(ch[i]) << " ";
	}
	std::cout << std::endl;
}

float bitsToFloat(std::string num){
	

}


int main(){
	std::cout << "sizeof(double) = " << sizeof(double) << std::endl;
	std::cout << "sizeof(char) = " << sizeof(char) << std::endl;
	
	/*
	for(double i = -10.0; i <= 10.5; i += 1.0){
		printBitsOfNumber(i);
	}
	*/
	for(float i = -10.0; i <= 10.5; i += 1.0){
		printBitsOfNumber(i);
	}
	return 0;
}
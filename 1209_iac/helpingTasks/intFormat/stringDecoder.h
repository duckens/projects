#ifndef STRINGDECODER_H
#define STRINGDECODER_H

#include <cstdio>
#include <iostream>
#include <string>
#include <math.h>
#include <sstream>


/* IMPORTANT!! for all such things char MUST BE UNSIGNED!! */
std::string charToBits(unsigned char ch);
unsigned char bitsToChar(std::string bits);
std::string floatToBits(float number);
std::string doubleToBits(double number);
std::string intToBits(int number);
std::string longLongIntToBits(long long int number);
float bitsToFloat(std::string str);
float bitsToDouble(std::string str);
std::string hexToBits(std::string hexString);
double decode_EFP(std::string hex_str);
float decode_FP(std::string hex_str);
std::string decode_time(std::string hex_str);
int decode_word(std::string hex_str);
int decode_char(std::string hex_str);
void parseFormat(std::string data, std::string format);

#endif
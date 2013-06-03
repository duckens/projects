#include "configParser.h"
#include <iostream>
#include <stdlib.h>
#include"../misc/converters.h"

configParser::configParser(std::string filePath){
	log.open("configParser.log");
	log << "constructor call" << std::endl;
	configFilePath = filePath;
	configMap.clear();
}

configParser::~configParser(){
	configFile.close();
}

void configParser::setNewFilePath(std::string filePath){
	configFilePath = filePath;
	configMap.clear();
}

std::pair<std::string, std::string>* configParser::parseString(char* str){
	log << std::endl << "parseString(" << str << ") call" << std::endl;
	int i = 0;
	while((str[i] == ' ') || (str[i] == '\t')){
		if((str[i] == '\n') || (str[i] == '\0')) return NULL;
		i++;
	}
	std::string key;
	std::string val;
	while((str[i] != ' ') && (str[i] != '\t')){
		if((str[i] == '\n') || (str[i] == '\0')) return NULL;
		key.push_back(str[i]);
		i++;
	}
	while((str[i] == ' ') || (str[i] == '\t')){
		if((str[i] == '\n') || (str[i] == '\0')) return NULL;
		i++;
	}
	if((str[i] == '\n') || (str[i] == '\0')) return NULL;
	while((str[i] != ' ') && (str[i] != '\t') && (str[i] != '\n') && (str[i] != '\0')){
		val.push_back(str[i]);
		i++;
	}
	
	std::pair<std::string, std::string>* pair = new std::pair<std::string, std::string>(key, val);
	log << "parseString ret" << std::endl;
	return pair;
}

bool configParser::writeConfigFile(){
	log << std::endl << "writeConfigFile call" << std::endl;
	if (configFile.is_open()){
		configFile.close();
	}
	
	configFile.open(configFilePath.c_str(), std::fstream::out);
	if (!configFile.is_open()){
		log << "can't open file! ret false" << std::endl;
		return false;
	}
	
	log << "Writing!" << std::endl;
	for(std::map<std::string, std::string>::iterator it = configMap.begin(); it != configMap.end(); ++it){
		log << it->first << "\t\t-->\t\t" << it->second << std::endl;
		configFile << it->first << " " << it->second << std::endl;
	}
	configFile.close();
	log << "ret true" << std::endl;
	return true;
}

bool configParser::readConfigFile(){
	log << std::endl << "readConfigFile call" << std::endl;
	
	if (configFile.is_open()){
		configFile.close();
	}
	
	configFile.open(configFilePath.c_str(), std::fstream::in);
	if (!configFile.is_open()){
		return false;
	}
	
	configMap.clear();
	while(!configFile.eof()){
		char* lineBuffer = new char[100];
		
		// important!! clear buffer
		// will not work if you don't do this!
		for(int i = 0; i < 100; i++){
			lineBuffer[i] = ' ';
		}
		
		configFile.getline (lineBuffer, 100);
			
		std::pair<std::string, std::string>* newPair = parseString(lineBuffer);
		delete[] lineBuffer;
		if(newPair != NULL){
			configMap.insert( *newPair );
		}
	}
	configFile.close();
	return true;
}

std::string configParser::getValue(std::string desiredKey){
	log << std::endl << "getValue(" << desiredKey << ") call" << std::endl;
	
	if(configMap.find(desiredKey) != configMap.end()){
		log << "answer = " << configMap.find(desiredKey)->second << std::endl;
		log << "getValue ret" << std::endl;
		return configMap.find(desiredKey)->second;
	} else {
		log << "getValue ret" << std::endl;
		return "";
	}
}

void configParser::removeValue(std::string key){
	log << std::endl << "removeValue(" << key << ") call" << std::endl;
	
	configMap.erase(key);
}

int configParser::getIntValue(std::string desiredKey){
	log << std::endl << "getIntValue(" << desiredKey << ") call" << std::endl;
	if(configMap.find(desiredKey) != configMap.end()){
		log << "answer = " << atoi(configMap.find(desiredKey)->second.c_str()) << std::endl;
		log << "getIntValue ret" << std::endl;
		return atoi(configMap.find(desiredKey)->second.c_str());
	} else {
		log << "getIntValue = 0 ret" << std::endl;
		return 0;
	}
	log << "getIntValue ret" << std::endl;
}

double configParser::getDoubleValue(std::string desiredKey){
	log << std::endl << "getDoubleValue(" << desiredKey << ") call" << std::endl;
	if(configMap.find(desiredKey) != configMap.end()){
		log << "answer = " << converters::stringToDouble(configMap.find(desiredKey)->second) << std::endl;
		log << "getDoubleValue ret" << std::endl;
		return converters::stringToDouble(configMap.find(desiredKey)->second);
	} else {
		log << "getDoubleValue = 0 ret" << std::endl;
		return 0;
	}
	log << "getDoubleValue ret" << std::endl;
}

bool configParser::hasValue(std::string desiredKey){
	log << std::endl << "hasValue(" << desiredKey << ") call" << std::endl;

	if(configMap.find(desiredKey) != configMap.end()){
		return true;
	} else {
		return false;
	}
}

void configParser::setValue(std::string key, std::string val){
	log << std::endl << "setValue((string)" << key << ", (string)" << val << ") call" << std::endl;
	std::map<std::string, std::string>::iterator it = configMap.find(key);
	if  ( it != configMap.end() ){
		it->second = val;
	} else {
		configMap.insert ( std::pair<std::string, std::string>(key, val) );
	}
	log << "setValue ret" << std::endl;
}

void configParser::setValue(std::string key, double val){
	log << std::endl << "setValue((string)" << key << ", (int)" << val << ") call" << std::endl;
	char string_val[40];
	sprintf(string_val, "%f", val);
	std::map<std::string, std::string>::iterator it = configMap.find(key);

	if  ( it != configMap.end() ){
		it->second = string_val;
	} else {
		configMap.insert ( std::pair<std::string, std::string>(key, std::string(string_val)) );
	}
	log << "setValue ret" << std::endl;
}

void configParser::setValue(std::string key, int val){
	log << std::endl << "setValue((string)" << key << ", (int)" << val << ") call" << std::endl;
	char string_val[40];
	sprintf(string_val, "%d", val);
	std::map<std::string, std::string>::iterator it = configMap.find(key);

	if  ( it != configMap.end() ){
		it->second = string_val;
	} else {
		configMap.insert ( std::pair<std::string, std::string>(key, std::string(string_val)) );
	}
	log << "setValue ret" << std::endl;
}

void configParser::clearConfig(){
	configMap.clear();
}

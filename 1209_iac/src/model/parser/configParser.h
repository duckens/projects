#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include<fstream>
#include<map>
#include<string>


class configParser{
	public:
		configParser(std::string filePath = std::string());
		~configParser();
		
		void setNewFilePath(std::string filePath);
		bool readConfigFile();
		bool writeConfigFile();
		std::string getValue(std::string desiredKey);
		int getIntValue(std::string desiredKey);
		double getDoubleValue(std::string desiredKey);
		bool hasValue(std::string desiredKey);
		void setValue(std::string key, std::string val);
		void setValue(std::string key, int val);
		void setValue(std::string key, double val);
		void removeValue(std::string key);
		void clearConfig();
		
	private:
		std::ofstream log;
		std::map<std::string, std::string> configMap;

		std::pair<std::string, std::string>* parseString(char* str);
		std::string configFilePath;
		std::fstream configFile;
};

#endif
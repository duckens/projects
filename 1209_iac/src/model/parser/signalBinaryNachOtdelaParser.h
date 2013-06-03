#ifndef SIGNALBINARYNACHOTDELAPARSER_H
#define SIGNALBINARYNACHOTDELAPARSER_H

#include <vector>
#include <string>
#include <fstream>


class signalBinaryNachOtdelaParser{
	public:
		signalBinaryNachOtdelaParser(std::string filePath, std::string destinationFolderName);
		~signalBinaryNachOtdelaParser();

		bool parse(std::string filesNameMask, int& gidrophonesAmount, int& signalLength, std::vector<std::string>* fileNames);
		
		std::string escapeFileName(std::string path);
		
	private:
		void makeConfigFile(std::string pathToConfigFile);

		std::ofstream log;
	
		std::string path;
		std::string destFolderName;
		
		FILE* signalFile;
		
		int N_Treal; 	//номер реализации
		int AntNumber;	//номер антенны; 
		int N_DA;            // число элементов антенны;
		//double x[N_DA];     // относительные декартовы координаты элементов антенны 		 
		//double y[N_DA];     // (относительно опорной приемной позиции), м 
		//double z[N_DA];                                     
		double TimeOfCycle;  //начальный момент излучения пачки зондирующего сигнала, 
								// тип для  значения "дата/время" (здесь и далее обозначается
								 // double) в зависимости от среды разработки имитатора:
								// CDateTime (VC), либо TDateTime (C++ Builder, Delphi),
								 // либо time_t (Unix), (double);
		double TimeOfRegistration;  //время начала регистрации на данной антенне;
		double FDiscr;     		 // частота дискретизации;
		int SampleLength;   	// число отсчётов по времени;
		int Reserv[4];    		// резерв
		//float Data [N_DA] [SampleLength]
};
#endif
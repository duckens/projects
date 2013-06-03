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
		
		int N_Treal; 	//����� ����������
		int AntNumber;	//����� �������; 
		int N_DA;            // ����� ��������� �������;
		//double x[N_DA];     // ������������� ��������� ���������� ��������� ������� 		 
		//double y[N_DA];     // (������������ ������� �������� �������), � 
		//double z[N_DA];                                     
		double TimeOfCycle;  //��������� ������ ��������� ����� ������������ �������, 
								// ��� ���  �������� "����/�����" (����� � ����� ������������
								 // double) � ����������� �� ����� ���������� ���������:
								// CDateTime (VC), ���� TDateTime (C++ Builder, Delphi),
								 // ���� time_t (Unix), (double);
		double TimeOfRegistration;  //����� ������ ����������� �� ������ �������;
		double FDiscr;     		 // ������� �������������;
		int SampleLength;   	// ����� �������� �� �������;
		int Reserv[4];    		// ������
		//float Data [N_DA] [SampleLength]
};
#endif
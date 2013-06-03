#ifndef CALIBRATIONPROCESS_H
#define CALIBRATIONPROCESS_H

#include "model/classes/inlineAntena.h"
#include "model/classes/sourceSequence.h"

#include "model/misc/physicalConstants.h"

#include "model/alglib/fasttransforms.h"
#include "model/alglib/ap.h"
#include "model/alglib/alglibmisc.h"
#include "model/alglib/alglibinternal.h"

class calibrationProcess{
	private:
	
		std::ofstream log;
		// �������, ������� �� ���������. � �������� ���������� ���� ������ �����������
		inlineAntena antena;
		// ��������� ������� (�������� �����, ���������� ������� � ������� � �.�.)
		sourceSequence sources;
		// �������� �������� ����� i-� � i+1-� ����������.
		dualVector<double> mutualDelays;
		// ������������� �������� ����� i-� � 0-� ���������� - �� �� ������� ����� ��������� ������� mutualDelays
		dualVector<double> relativeDelays;
		tripleVector<double> gidrophonesCalibrationData;
		tripleVector<alglib::complex> gidrophonesCalibrationDataAfterFFT;

		std::vector<double>* errorX;
		std::vector<double>* errorY;
		
		int dftDimention;
		double soundSpeed;
		
		// ����������� �������������� �����
		static const int frequencyDiscretization = ph_const_frequencyDiscretization;//(Hz)
		// ������� ������������� ������� � ���������
		//static double timeIntervalDiscretization = 1 / (double)frequencyDiscretization; // secounds
		// ������ �������������
		
		int findMaximalDFTBean(tripleVector<alglib::complex>* dataAfterFFT, int gidrophoneNumber, int sourceNumber);
		
		double fik2(double Re, double Im);
		double fik(double Re, double Im);		

	public:
	
		static const double pi = 3.14159265;
		
		calibrationProcess();
		calibrationProcess(inlineAntena& givenAntena, sourceSequence& givenSources, tripleVector<double>& givenGidrophonesCalibrationData, int DFTDim, double soundSpeedGiven);
		~calibrationProcess();
		
		void operator=(const calibrationProcess& newProcess);
		void makeFFT();
		void makeMutualDelaysSmart();
		void makeRelativeDelays();
		
		//void honestCalibrateOneGidrophone(int gidrophoneNumber);
		void smartCalibrateOneGidrophone(int gidrophoneNumber);
		void calibrate3dOneGidrophone(int gidrophoneNumber);
		
		void smartCalibrate();
		//void honestCalibrate();
		
		void giveFFT(std::vector<double>& vect, int gidrophoneNumber, int sourceNumber);
		void giveAbsFFT(std::vector<double>& vect, int gidrophoneNumber, int sourceNumber);
		void correctMutualDelays();
		inlineAntena giveAntena();
		sourceSequence giveSources();
		
		std::vector<double>* giveErrorX();
		std::vector<double>* giveErrorY();

};
#endif
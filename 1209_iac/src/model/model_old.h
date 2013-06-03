#ifndef MODEL_H
#define MODEL_H

#include <fstream>

#include <QtGui>
#include "containers/arrays.h"
#include "containers/modelData.h"
#include "containers/modelShowData.h"
#include "model/parser/configParser.h"
#include "model/parser/signalBinaryNachOtdelaParser.h"
#include "model/classes/inlineAntena.h"
#include "model/classes/calibrationProcess.h"
#include "model/classes/sourceSequence.h"
#include "model/classes/sourceCircleSequence.h"



#include<string>

class model : public QObject{

	Q_OBJECT
	
	private:
		std::ofstream log;
		
		configParser* project;
		std::string projectFilePath;
		std::string projectFolderPath;
		
		std::string escapeFileName(std::string path);
		std::string escapePath(std::string path);
		
		sourceSequence* makeSources();
		
		tripleVector<double>* giveCalibrationData();
		tripleVector<double>* giveCalibrationData_cached;
		bool giveCalibrationData_isCached;
		bool giveCalibrationData_forcedCalculate;
		
		std::vector<int>* findCycles();
		std::vector<int>* findCycles_cached;
		bool findCycles_isCached;
		bool findCycles_forcedCalculate;
		
		dualVector<double>* givePeakSignal_cached;
		bool givePeakSignal_isCached;
		bool givePeakSignal_forcedCalculate;
		
		std::vector<std::string>* findSingleGidrophoneFiles();
		std::vector<std::string>* findSingleGidrophoneFiles_cached;
		bool findSingleGidrophoneFiles_isCached;
		bool findSingleGidrophoneFiles_forcedCalculate;
		
		dualVector<double>* giveSignal_cached;
		bool giveSignal_isCached;
		
		
		std::string signalFilePath;
		calibrationProcess* calibrator;
		
		double giveMeNoise();
		
		bool manuallyFindCycles;
		
		void quickSort(std::vector<int>* A, int low, int high);
		
		modelShowData whatModelCanShow;
		
	public:
		model(void);
		~model(void);
		
		
		
		bool openProjectFile(std::string path);
		bool newProjectFile(std::string path);
		void parseSignalFile();
		void takeParameters(modelData parameters);
		void sliceAndMakeFFT(int DFTDimention, int overlay, int threshold);
		void calibrate();
		
		inlineAntena giveAntena();
		sourceSequence giveSources();
		dualVector<double>* giveSignal();
		dualVector<double>* GiveFFTSignal(int cycleNumber);
		dualVector<double>* givePeakSignal(bool forced = false);
		
		std::vector<int>* giveCycles();
		std::vector<double>* giveFrequencies();
		
		
		
		void takeCycles(std::vector<int>* arrayOfMarkers);
		
		
		


		
	//public Q_SLOTS:
	
	
	Q_SIGNALS:
	
		void modelStarted(std::string);
		void modelFinished(std::string, std::string);
		void modelSays(std::string);
	
	
		void modelNewParameters(modelData);
		void modelReadyToTakeCycles(dualVector<double>*);

	
		void modelHasSignalFile(bool);
		void modelFinishedParsing(bool);
		void modelFinishedPeakSignalCreating(bool);
		void modelFinishedChoosingCycles(bool);
		void modelFinishedAll(bool);
		
		void calibrationFinished(void);
		
		void modelCanShow(modelShowData);
		
		


};
#endif
#include <QtGui>

#include <string>
#include <fstream>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "model/model.h"


#include "model/misc/converters.h"
#include "model/misc/physicalConstants.h"


#include "model/alglib/fasttransforms.h"
#include "model/alglib/fasttransforms.cpp"
#include "model/alglib/ap.h"
#include "model/alglib/ap.cpp"
#include "model/alglib/alglibmisc.h"
#include "model/alglib/alglibmisc.cpp"
#include "model/alglib/alglibinternal.h"
#include "model/alglib/alglibinternal.cpp"




model::model(){
	log.open("model.log");
	log << "constructor call" << std::endl;
	project = new configParser();

	whatModelCanShow.signal = false;
	whatModelCanShow.peakSignal = false;
	whatModelCanShow.cycles = false;
	whatModelCanShow.antena = false;
	
	
	findCycles_cached = new std::vector<int>;
	findSingleGidrophoneFiles_cached = new std::vector<std::string>;
	givePeakSignal_cached = new dualVector<double>;
	giveCalibrationData_cached = new tripleVector<double>;
	giveSignal_cached = new dualVector<double>;
	
	giveSignal_isCached = false;
	giveCalibrationData_isCached = false;
	findCycles_isCached = false;
	givePeakSignal_isCached = false;
	findSingleGidrophoneFiles_isCached = false;
	
	giveCalibrationData_forcedCalculate = false;
	findCycles_forcedCalculate = false;
	givePeakSignal_forcedCalculate = false;
	
	manuallyFindCycles = false;
	
}
model::~model(){
	log << "destructor call" << std::endl;
	
	delete[] findCycles_cached;
	delete[] findSingleGidrophoneFiles_cached;
	log.close();
}
void model::parseSignalFile(){
	findSingleGidrophoneFiles_forcedCalculate = true;
	findSingleGidrophoneFiles();
}

void model::takeParameters(modelData parameters){
	Q_EMIT modelStarted(std::string("takeParameters"));

	if(parameters.DFTDimention != project->getIntValue("DFTDimention")){
		project->setValue("DFTDimention", parameters.DFTDimention);
		
		giveCalibrationData_isCached = false;
		findCycles_isCached = false;
		givePeakSignal_isCached = false;
		
		whatModelCanShow.peakSignal = false;
		whatModelCanShow.cycles = false;
		whatModelCanShow.antena = false;
		
		giveCalibrationData_forcedCalculate = true;
		findCycles_forcedCalculate = true;
		givePeakSignal_forcedCalculate = true;
		//findSingleGidrophoneFiles_isCached;
	}
	if(parameters.overlay != project->getIntValue("overlay")){
		project->setValue("overlay", parameters.overlay);
	
		giveCalibrationData_isCached = false;
		findCycles_isCached = false;
		givePeakSignal_isCached = false;
		
		whatModelCanShow.peakSignal = false;
		whatModelCanShow.cycles = false;
		whatModelCanShow.antena = false;
		
		giveCalibrationData_forcedCalculate = true;
		findCycles_forcedCalculate = true;
		givePeakSignal_forcedCalculate = true;
	}
	if(parameters.threshold != project->getIntValue("threshold")){
		project->setValue("threshold", parameters.threshold);
		
		giveCalibrationData_isCached = false;
		findCycles_isCached = false;
		
		whatModelCanShow.cycles = false;
		whatModelCanShow.antena = false;
		
		giveCalibrationData_forcedCalculate = true;
		findCycles_forcedCalculate = true;
	}
	if( (parameters.bandHigh != project->getIntValue("bandHigh")) || (parameters.bandLow != project->getIntValue("bandLow"))){
		project->setValue("bandHigh", parameters.bandHigh);
		project->setValue("bandLow", parameters.bandLow);
		
		giveCalibrationData_isCached = false;
		findCycles_isCached = false;
		givePeakSignal_isCached = false;
		
		whatModelCanShow.peakSignal = false;
		whatModelCanShow.cycles = false;
		whatModelCanShow.antena = false;
		
		giveCalibrationData_forcedCalculate = true;
		findCycles_forcedCalculate = true;
		givePeakSignal_forcedCalculate = true;
	}
	manuallyFindCycles = parameters.manuallyFindCycles;
	project->setValue("soundSpeed", parameters.soundSpeed);
	/*
	if(parameters.farawayZone){
		project->setValue("farawayZone", "yes");
	} else {
		project->setValue("farawayZone", "no");
	} 
	*/
	Q_EMIT modelNewParameters(parameters);
	Q_EMIT modelCanShow(whatModelCanShow);
	
	Q_EMIT modelFinished(std::string("takeParameters"), std::string("DFTDimention = ") + converters::intToString(project->getIntValue("DFTDimention")) + ", overlay : " + converters::intToString(project->getIntValue("overlay")) + ", manuallyFindCycles : " + converters::intToString(manuallyFindCycles));	//Q_EMIT modelNewParameters(parameters);
}

dualVector<double>* model::giveSignal(){
	log << std::endl << "giveSignal call" << std::endl;
	Q_EMIT modelStarted(std::string("giveSignal"));
	
	if(giveSignal_isCached){
		return giveSignal_cached;
	} else {		
		std::vector<std::string>* gidrophoneFilePaths = findSingleGidrophoneFiles();
		

		
		int gidrophonesAmount = project->getIntValue("gidrophonesAmount");
		int signalLength = project->getIntValue("signalLength");
		
		log << "gidrophonesAmount = " << gidrophonesAmount << std::endl;
		log << "signalLength = " << signalLength << std::endl;
		
		giveSignal_cached->resize(gidrophonesAmount, signalLength);
		log << "resized!" << std::endl;
		
		for(int i = 0; i < gidrophonesAmount; i++){
			FILE* gidrophoneSignalFile = fopen((*gidrophoneFilePaths)[i].c_str(), "rb");
			log << "just opened " << (*gidrophoneFilePaths)[i].c_str() << std::endl;
			float buffer;
			for(int n = 0; n < signalLength; n++){
				fread(&buffer, sizeof(float), 1, gidrophoneSignalFile);
				(*giveSignal_cached)(i, n) = (double)buffer;
			}
		}
		
		Q_EMIT modelFinished(std::string("giveSignal"), std::string("OK"));
		giveSignal_isCached = true;
		return giveSignal_cached;
	}
}
dualVector<double>* model::GiveFFTSignal(int cycleNumber){
	log << "GiveFFTSignal call" << std::endl;
	Q_EMIT modelStarted(std::string("GiveFFTSignal"));

	int gidrophonesAmount = project->getIntValue("gidrophonesAmount");
	int DFTDimention = project->getIntValue("DFTDimention");
	int antiOverlayInPoints = DFTDimention * (100 - project->getIntValue("overlay")) / 100;
	dualVector<double>* result = new dualVector<double>(gidrophonesAmount, DFTDimention);
	
	log << "gidrophonesAmount = " << gidrophonesAmount << std::endl;
	log << "overlay = " << project->getIntValue("overlay")  << std::endl;
	log << "antiOverlayInPoints = " << antiOverlayInPoints << std::endl;
	
	std::vector<std::string>* gidrophoneFilePaths = findSingleGidrophoneFiles();
	
	for(int i = 0; i < gidrophonesAmount; i++){
		log << "i = " << i << "\t\t";
		std::vector<double> signalInOneCycle(DFTDimention);
		
		FILE* gidrophoneSignalFile = fopen((*gidrophoneFilePaths)[i].c_str(), "rb");
		log << "just opened " << (*gidrophoneFilePaths)[i].c_str() << std::endl;
		
		float buffer;
		fseek(gidrophoneSignalFile, antiOverlayInPoints * cycleNumber * sizeof(float), SEEK_SET);
		for(int n = 0; n < DFTDimention; n++){
			fread(&buffer, sizeof(float), 1, gidrophoneSignalFile);
			signalInOneCycle[n] = (double)buffer;
		}
		
		
		alglib::real_1d_array signalBeforeFFT;
		signalBeforeFFT.setcontent(signalInOneCycle.size(),&signalInOneCycle[0]);
		alglib::complex_1d_array signalAfterFFT;
		alglib::fftr1d(signalBeforeFFT, signalAfterFFT);
		
		for(int k = 0; k < DFTDimention; k++){
			(*result)(i,k) = (signalAfterFFT[k].x * signalAfterFFT[k].x + signalAfterFFT[k].y * signalAfterFFT[k].y);
		}
		
		fclose(gidrophoneSignalFile);
	}
	Q_EMIT modelFinished(std::string("GiveFFTSignal"), std::string("OK"));
	log << "GiveFFTSignal ret" << std::endl;
	return result;
}
dualVector<double>* model::givePeakSignal(/*int DFTDimention, int overlay, int threshold,*/ bool forced){
	log << std::endl << "givePeakSignal call" << std::endl;
	Q_EMIT modelStarted(std::string("givePeakSignal"));
	if (givePeakSignal_isCached && !givePeakSignal_forcedCalculate) {
		log << "givePeakSignal_isCached == true" << std::endl;
		log << "givePeakSignal ret OK [cache]" << std::endl;
		whatModelCanShow.peakSignal = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		Q_EMIT modelFinished(std::string("givePeakSignal"), std::string("OK [cache]"));
		return givePeakSignal_cached;
	} else if( (project->getValue("peakSignalCreated") == "yes") && !givePeakSignal_forcedCalculate ) {
		log << "givePeakSignal_isCached == false, but project->getValue(\"peakSignalCreated\") == \"yes\"" << std::endl;
	
		int gidrophonesAmount = project->getIntValue("gidrophonesAmount");
		log << "project->getValue(\"peakSignalCreated\") == yes" << std::endl;
		log << "project->getIntValue(\"peakSignalSize\") = " << project->getIntValue("peakSignalSize") << std::endl;
		log << "project->getValue(\"peakSignal\") = " << project->getValue("peakSignalFile") << std::endl;
		log << "gidrophonesAmount = " << gidrophonesAmount << std::endl;
		
		
		(*givePeakSignal_cached).resize(gidrophonesAmount, project->getIntValue("peakSignalSize"));
		std::string peakSignalFileName = projectFolderPath + "/" + project->getValue("peakSignalFile");
		FILE* peakSignalFile = fopen(peakSignalFileName.c_str(), "rb");

		log << "peakSignal(i, j)" << std::endl;
		for(int i = 0; i < gidrophonesAmount; i++){
			for(int j = 0; j < project->getIntValue("peakSignalSize"); j++){
				fread(&((*givePeakSignal_cached)(i, j)), sizeof(double), 1, peakSignalFile);
				log << (*givePeakSignal_cached)(i, j) << std::endl;
			}
		}
		fclose(peakSignalFile);
		
		givePeakSignal_isCached = true;
		Q_EMIT modelFinished(std::string("givePeakSignal"), std::string("OK [preloaded]"));
		log << "givePeakSignal ret OK [preloaded]" << std::endl;
		whatModelCanShow.peakSignal = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		return givePeakSignal_cached;
	} else {
		log << "givePeakSignal evaluating..." << std::endl;
		std::vector<std::string>* gidrophoneFilePaths = findSingleGidrophoneFiles();
		
		
		int gidrophonesAmount = project->getIntValue("gidrophonesAmount");
		int antiOverlayInPoints = project->getIntValue("DFTDimention") * (100 - project->getIntValue("overlay")) / 100;
		int signalLength = project->getIntValue("signalLength");
		int peakSignalSize = (signalLength - project->getIntValue("DFTDimention") + antiOverlayInPoints - 1) / antiOverlayInPoints;
		int DFTDimention = project->getIntValue("DFTDimention");
		
		int bandLow = project->getIntValue("bandLow");
		int bandHigh = project->getIntValue("bandHigh");

		int lowestBean = ( DFTDimention * bandLow ) / ph_const_frequencyDiscretization;
		int highestBean = ( DFTDimention * bandHigh ) / ph_const_frequencyDiscretization;
		if(highestBean >= DFTDimention){
			highestBean = DFTDimention - 1;
		}
		
		log << "project->getIntValue(\"DFTDimention\") = " << project->getIntValue("DFTDimention") << std::endl;
		log << "overlay = " << project->getIntValue("overlay") << std::endl;
		log << std::endl;
		log << "gidrophonesAmount = " << gidrophonesAmount << std::endl;
		log << "signalLength = " << signalLength << std::endl;
		log << "antiOverlayInPoints = " << antiOverlayInPoints << std::endl;
		log << "peakSignalSize = " << peakSignalSize << std::endl;

		givePeakSignal_cached->resize(gidrophonesAmount, peakSignalSize);
		std::vector<double> singleGidrophoneSignal;

		singleGidrophoneSignal.resize(signalLength);
		
		for(int i = 0; i < gidrophonesAmount; i++){
			log << (*gidrophoneFilePaths)[i] << std::endl;
		}
		
		//double maxPeakValue = 0.0;
		for(int i = 0; i < gidrophonesAmount; i++){
			
			FILE* gidrophoneSignalFile = fopen((*gidrophoneFilePaths)[i].c_str(), "rb");
			log << "sliceAndMakeFFT just opened " << (*gidrophoneFilePaths)[i].c_str() << std::endl;
			
			float buffer;
			
			for(int n = 0; n < signalLength; n++){
				fread(&buffer, sizeof(float), 1, gidrophoneSignalFile);
				singleGidrophoneSignal[n] = (double)buffer;
			}
			
			std::vector<double> signalInOneCycle(DFTDimention);
			for(int j = 0; j < peakSignalSize; j++){
				for(int k = 0; k < DFTDimention; k++){
					signalInOneCycle[k] = singleGidrophoneSignal[antiOverlayInPoints * j + k];
				}
				alglib::real_1d_array signalBeforeFFT;
				signalBeforeFFT.setcontent(signalInOneCycle.size(),&signalInOneCycle[0]);
				alglib::complex_1d_array signalAfterFFT;
				alglib::fftr1d(signalBeforeFFT, signalAfterFFT);
				
				double peakValue = 0;
				for(int k = lowestBean; k < highestBean; k++){
					double thisValue = (signalAfterFFT[k].x * signalAfterFFT[k].x + signalAfterFFT[k].y * signalAfterFFT[k].y);
					if(thisValue > peakValue){
						peakValue = thisValue;
					}
				}
				
				(*givePeakSignal_cached)(i, j) = peakValue;
				
			}
			fclose(gidrophoneSignalFile);
			
			log << "done creating givePeakSignal_cached with " << i << "-th gidrophone" << std::endl;
		}
		
		
		
		std::string peakSignalFileName = projectFolderPath + "/peakSignal.dat";
		

		FILE* peakSignalFile = fopen(peakSignalFileName.c_str(), "wb");
		log << "writing peakSignalFile " << peakSignalFileName << " " << gidrophonesAmount << " x " << peakSignalSize << " numbers" << std::endl;
		log << "givePeakSignal_cached(i, j)" << std::endl;
		for(int i = 0; i < gidrophonesAmount; i++){
			for(int j = 0; j < peakSignalSize; j++){
				fwrite(&((*givePeakSignal_cached)(i, j)), sizeof(double), 1, peakSignalFile);
				log << (*givePeakSignal_cached)(i, j) << std::endl;

			}
		}
		fclose(peakSignalFile);
		
		project->setValue("peakSignalCreated", "yes");
		project->setValue("peakSignalFile", "peakSignal.dat");
		project->setValue("gidrophonesAmount", converters::intToString(gidrophonesAmount));
		project->setValue("peakSignalSize", converters::intToString(peakSignalSize));
		project->setValue("peakSignalCreated", "yes");
		
		project->writeConfigFile();
		
		givePeakSignal_isCached = true;
		givePeakSignal_forcedCalculate = false;
		
		Q_EMIT modelFinished(std::string("givePeakSignal"), std::string("OK"));
		//Q_EMIT modelFinishedPeakSignalCreating(true);
		log << "givePeakSignal ret OK" << std::endl;
		whatModelCanShow.peakSignal = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		return givePeakSignal_cached;
	}
}
std::vector<int>* model::findCycles(){
	log << std::endl << "findCycles call" << std::endl;
	Q_EMIT modelStarted(std::string("findCycles"));
	
	if(findCycles_isCached && !findCycles_forcedCalculate){
		log << "findCycles ret from cache" << std::endl;
		Q_EMIT modelFinished(std::string("findCycles"), std::string(std::string("from cache : ") + converters::intToString(findCycles_cached->size()) + " cycles"));
		whatModelCanShow.cycles = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		return findCycles_cached;
	} else if( (project->getValue("cyclesFound") == "yes") && !findCycles_forcedCalculate ){
		log << "project->getValue(\"cyclesFound\") == \"yes\"" << std::endl;
		
		findCycles_cached->clear();
		
		log << "findCycles_cached->clear();\tdone" << std::endl;
		for(int i = 0; i < project->getIntValue("cyclesFoundAmount"); i++){
			log << "for(int i = 0; i < project->getIntValue(\"cyclesFoundAmount\"); i++){\tdone" << std::endl;
			(*findCycles_cached).push_back( project->getIntValue("cyclesFound_" + converters::intToString(i)) );
		}
		log << "preloadedfrom disk to cache" << std::endl;
		log << "findCycles ret" << std::endl;
		
		findCycles_isCached = true;
		Q_EMIT modelFinished(std::string("findCycles"), std::string(std::string("from disk : ") + converters::intToString(findCycles_cached->size()) + " cycles"));
		whatModelCanShow.cycles = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		return findCycles_cached;
	} else{
		dualVector<double>* peakSignal = givePeakSignal();
		
		int lastDetectedPeakPlace = 0;
		bool walkingInGapBetweenPeaks = true;
		double localMax = (*peakSignal)(0, 0);
		double maxPeakValue = 0;
		
		for(int i = 0; i < project->getIntValue("peakSignalSize"); i++){
			if((*peakSignal)(0, i) > maxPeakValue){
				maxPeakValue = (*peakSignal)(0, i);
			}
		}
		
		double thresholdValue = project->getIntValue("threshold") * maxPeakValue / 100;
		findCycles_cached->clear();

		for(int i = 0; i < project->getIntValue("peakSignalSize"); i++){
			log << (*peakSignal)(0, i) << " < " << thresholdValue << "? .. ";
			if((*peakSignal)(0, i) < thresholdValue){
				log << "yes" << std::endl;
				if(walkingInGapBetweenPeaks == false){
					log << "found cycle n = " << lastDetectedPeakPlace << ", value = " << (*peakSignal)(0, lastDetectedPeakPlace) << std::endl;
					(*findCycles_cached).push_back(lastDetectedPeakPlace);
					walkingInGapBetweenPeaks = true;
				}
				continue;
			}
			log << "no" << std::endl;
			if(walkingInGapBetweenPeaks){
				walkingInGapBetweenPeaks = false;
				//walkingInGapBetweenPeaks == false
				log << "localMax = peakSignal(0, " << i << ") = " << (*peakSignal)(0, i) << std::endl;
				localMax = (*peakSignal)(0, i);
				lastDetectedPeakPlace = i;
			} else {
				if(localMax < (*peakSignal)(0, i)){
					lastDetectedPeakPlace = i;
					localMax = (*peakSignal)(0, i);
				}
			}
		}
		
		
		
		//now remove what we had there
		if (project->hasValue("cyclesFoundAmount")){
			for(int i = 0; i < project->getIntValue("cyclesFoundAmount"); i++){
				project->removeValue("cyclesFound_" + converters::intToString(i));
			}
		}
		
		
		project->setValue("cyclesFoundAmount", (int)findCycles_cached->size());
		for(int i = 0; i < project->getIntValue("cyclesFoundAmount"); i++){
			project->setValue("cyclesFound_" + converters::intToString(i), (*findCycles_cached)[i]);
		}
		project->setValue("cyclesFound", "yes");
		
		findCycles_isCached = true;
		findCycles_forcedCalculate = false;
		
		Q_EMIT modelFinished(std::string("findCycles"), std::string(std::string("found ") + converters::intToString(findCycles_cached->size()) + " cycles"));
		log << "findCycles ret" << std::endl;
		whatModelCanShow.cycles = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		return findCycles_cached;
	}
}
tripleVector<double>* model::giveCalibrationData(){
	log << std::endl << "giveCalibrationData() call" << std::endl;
	Q_EMIT modelStarted(std::string("GiveCalibrationData"));
	
	if (giveCalibrationData_isCached && !giveCalibrationData_forcedCalculate){
		log << "giveCalibrationData ret from cache" << std::endl;
		Q_EMIT modelFinished(std::string("GiveCalibrationData"), std::string("OK [cache] "));
		return giveCalibrationData_cached;
	} else {
		std::vector<int>* cycleNumbers = findCycles();

		
		for(int i = 0; i < cycleNumbers->size(); i++){
			log << "(*cycleNumbers)[i] = " << (*cycleNumbers)[i] << std::endl;
		}
		
		
		// format: calibrationData(gidrophoneNumber, sourceNumber, i);
		int DFTDimention = project->getIntValue("DFTDimention");
		int sourcesAmount = cycleNumbers->size();
		int antiOverlayInPoints = DFTDimention * (100 - project->getIntValue("overlay")) / 100;
		int signalLength = project->getIntValue("signalLength");
		
		
		log << "DFTDimention = " << DFTDimention << std::endl;
		log << "sourcesAmount = " << sourcesAmount << std::endl;
		log << "antiOverlayInPoints = " << antiOverlayInPoints << std::endl;

		int gidrophonesAmount = project->getIntValue("gidrophonesAmount");
		giveCalibrationData_cached->resize(gidrophonesAmount, sourcesAmount, DFTDimention);
		
		std::vector<double> singleGidrophoneSignal;
		singleGidrophoneSignal.resize(signalLength);
		
		std::vector<std::string>* gidrophoneFilePaths = findSingleGidrophoneFiles();

		log << "(*gidrophoneFilePaths).size() = " << (*gidrophoneFilePaths).size() << std::endl;
		for(int i = 0; i < gidrophonesAmount; i++){
			log << "gidrophone = " << i << std::endl;

			log << (*gidrophoneFilePaths)[i].c_str() << std::endl;
			FILE* gidrophoneSignalFile = fopen((*gidrophoneFilePaths)[i].c_str(), "rb");
			float buffer;
			for(int n = 0; n < signalLength; n++){
				fread(&buffer, sizeof(float), 1, gidrophoneSignalFile);
				singleGidrophoneSignal[n] = (double)buffer;
			}
			
			for(int j = 0; j < sourcesAmount; j++){
				log << "*** source (cycle) = ***" << (*cycleNumbers)[j] << std::endl;
				for(int k = 0; k < DFTDimention; k++){
					(*giveCalibrationData_cached)(i,j,k) = singleGidrophoneSignal[antiOverlayInPoints * ((*cycleNumbers)[j]) + k];
					log << (*giveCalibrationData_cached)(i,j,k) << " ";
				}
				log << std::endl;
			}
			log << std::endl;
		}
		
		//project->setValue("cyclesChoosed", "yes");
		project->writeConfigFile();
		
		giveCalibrationData_isCached = true;
		giveCalibrationData_forcedCalculate = false;
		
		Q_EMIT modelFinished(std::string("GiveCalibrationData"), std::string("OK"));
		
		log << "giveCalibrationData ret" << std::endl;
		return giveCalibrationData_cached;
	}
}
double model::giveMeNoise(){
	double noise = 0;
	
	for(int i =0; i < 12; i++){
		noise += alglib::randomreal();
	}
	return (noise - 6) / 5.8;
}
sourceSequence* model::makeSources(/*std::vector<int>* foundCycles*/){
	log << std::endl << "makeSources call" << std::endl;
	Q_EMIT modelStarted(std::string("makeSources"));

	
	std::vector<int>* foundCycles = findCycles();
	
	if(foundCycles->size() != project->getIntValue("sourcesAmount")){
		log << "wrong sources amount! foundCycles.size() = " << foundCycles->size() << ", project->getIntValue(\"sourcesAmount\") = " << project->getIntValue("sourcesAmount") << std::endl;
		log << "makeSources ret NULL!!" << std::endl;
		Q_EMIT modelFinished(std::string("makeSources"), std::string("FAULT! wrong amount"));
		return NULL;
	}
	int DFTDimention = project->getIntValue("DFTDimention");
	std::vector<double> souceFrequencies;
	log << "foundCycles->size() = " << foundCycles->size() << std::endl;
	for(int i = 0; i < foundCycles->size(); i++){
		dualVector<double>* fftSignal = GiveFFTSignal((*foundCycles)[i]);
		double maximum = 0;
		int maxBean = 0;
		for(int j = 0; j < DFTDimention / 2; j++){
			if( (*fftSignal)(0, j) > maximum ){
				maxBean = j;
				maximum = (*fftSignal)(0, j);
			}
		}
		double maxInFrequency = (double)(maxBean * ph_const_frequencyDiscretization) / DFTDimention;
		log << "for " << i << "-th foud cycle maxInFrequency is : " << maxInFrequency << ", max bean = " << maxBean << std::endl;
		souceFrequencies.push_back(maxInFrequency);
		//delete[] fftSignal;
	}
	std::string sourcesFilePathWithName;
	sourcesFilePathWithName = projectFolderPath + "/../" + project->getValue("sourcesFile");
	
	log << "opening " << sourcesFilePathWithName << std::endl;
	std::ifstream sourcesFile(sourcesFilePathWithName.c_str());
	
	std::vector<sourceObject>* sourcesVect = new std::vector<sourceObject>;
		
		
		
	if(/*(!project->hasValue("frequency")) ||*/ (!project->hasValue("sourcesAmount")) || (!project->hasValue("sourcesFile")) || ( project->getIntValue("sourcesAmount") < 2 )){
		log << "makeSources dousn't know where are the sources" <<std::endl;
		log << "makeSources ret" <<std::endl;
		Q_EMIT modelFinished(std::string("makeSources"), std::string("FAULT! no freq specified"));
		return NULL;
	}
	int sourcesNumber = project->getIntValue("sourcesAmount");
	//sourcesFile >> sourcesNumber >> freq;
	for(int i = 0; i < sourcesNumber; i++){
		double x;
		double y;
		double z;
		sourcesFile >> x >> y >> z;
		log << "source at (" << x << ", " << y << ", " << z <<"), \t freq = " << souceFrequencies[i] << std::endl;


		sourcesVect->push_back(sourceObject(x, y, z, souceFrequencies[i]));	
	}
	sourceSequence* sources = new sourceSequence(*sourcesVect);
	
	Q_EMIT modelFinished(std::string("makeSources"), std::string("OK"));
	log << std::endl << "makeSources ret" << std::endl;
	return sources;
}
void model::calibrate(){
	log << std::endl << "calibrate call" << std::endl;
	Q_EMIT modelStarted(std::string("calibrate"));

	if(manuallyFindCycles){
		log << std::endl << "manuallyFindCycles = " << manuallyFindCycles << std::endl;
		dualVector<double>* peakSignal = givePeakSignal();
		manuallyFindCycles = false;
		
		modelData data;
		data.DFTDimention = project->getIntValue("DFTDimention");
		data.overlay = project->getIntValue("overlay");
		data.threshold = project->getIntValue("threshold");
		data.bandHigh = project->getIntValue("bandHigh");
		data.bandLow = project->getIntValue("bandLow");
		data.soundSpeed = project->getDoubleValue("soundSpeed");
		/*
		if(project->getValue("farawayZone") == "yes"){
			data.farawayZone = true;
		} else {
			data.farawayZone = false;
		}
		*/
		data.manuallyFindCycles = false;
		
		Q_EMIT modelNewParameters(data);
		Q_EMIT modelFinished(std::string("calibrate"), std::string("Redy for cycles"));
		Q_EMIT modelReadyToTakeCycles(peakSignal);
	} else {
		log << std::endl << "manuallyFindCycles = " << manuallyFindCycles << std::endl;
		sourceSequence* sources = makeSources();
		tripleVector<double>* calibrationData = giveCalibrationData();

		int gidrophonesAmount = project->getIntValue("gidrophonesAmount");
		
		std::ifstream firstGidroFile;
		log << "calibrate opens ifstream file " << (std::string(projectFolderPath) + "/../firstGidro.txt") << std::endl;
		firstGidroFile.open( (std::string(projectFolderPath) + "/../firstGidro.txt").c_str() );
		double posX = 0;
		double posY = 0;
		double posZ = 0;
		if(firstGidroFile.is_open()){
			firstGidroFile >> posX >> posY >> posZ;
		} else {
			log << "ERROR! no firstGidroFile! Set to 0, 0, 0" << std::endl;
		}
		firstGidroFile.close();
		
		inlineAntena antena(gidrophonesAmount, position(posX, posY, posZ), 0, 1);
		log << "made antena with length = " << gidrophonesAmount << std::endl;
		log << "extern call calibrationProcess(antena, *sources, calibrationData, " << project->getIntValue("DFTDimention") << ")" << std::endl;
		calibrator = new calibrationProcess(antena, *sources, *calibrationData, project->getIntValue("DFTDimention"), project->getDoubleValue("soundSpeed"));
		
		log << "extern call calibrator->smartCalibrate()" << std::endl;
		calibrator->smartCalibrate();
		//calibrator->calibrate();
		log << "extern call calibrator->giveAntena()" << std::endl;
		inlineAntena antenaResult = calibrator->giveAntena();
		
		
		
		
		log << "extern call calibrator->giveErrorX()" << std::endl;
		std::vector<double>* errorX = calibrator->giveErrorX();
		log << "extern call calibrator->giveErrorY()" << std::endl;
		std::vector<double>* errorY = calibrator->giveErrorY();
		log << "errorX->size() = " << errorX->size() << std::endl;
		double maxErrorX = 0;
		double maxErrorY = 0;
		double averageErrorX = 0;
		double averageErrorY = 0;
		for(int i = 0; i < errorX->size(); i++){
			log << "== " << i << "==   (" << (*errorX)[i] << ", " << (*errorY)[i] << ")" << std::endl;
			if( (*errorX)[i] > maxErrorX ){
				maxErrorX = (*errorX)[i];
			}
			if( (*errorY)[i] > maxErrorY ){
				maxErrorY = (*errorY)[i];
			}
			averageErrorX += (*errorX)[i];
			averageErrorY += (*errorY)[i];
			log << "Q_EMIT modelSays(..." << std::endl;
			Q_EMIT modelSays( std::string("== ") + converters::intToString(i) + "==   (" + converters::doubleToString( (*errorX)[i] ) + ", " + converters::doubleToString( (*errorY)[i] ) + ")" );
		}
		log << "cycle done" << std::endl;
		averageErrorX /= errorX->size();
		averageErrorY /= errorX->size();
		
		log << "Q_EMIT modelSays(..." << std::endl;
		Q_EMIT modelSays( std::string("== maximal error ==   (") + converters::doubleToString( maxErrorX ) + ", " + converters::doubleToString( maxErrorY ) + ")" );
		log << "Q_EMIT modelSays(..." << std::endl;
		Q_EMIT modelSays( std::string("== average error ==   (") + converters::doubleToString( averageErrorX ) + ", " + converters::doubleToString( averageErrorY ) + ")" );
		
		
		
		
		
		std::ifstream antennaRealPositionsFile;
		std::ofstream antennaCalibratedFile;
		log << "calibrate opens ifstream file " << (std::string(projectFolderPath) + "/antenna.txt") << std::endl;
		antennaRealPositionsFile.open( (std::string(projectFolderPath) + "/antenna.txt").c_str() );
		log << "calibrate opens ofstream file " << (std::string(projectFolderPath) + "/antennaCalib.txt") << std::endl;
		antennaCalibratedFile.open( (std::string(projectFolderPath) + "/antennaCalib.txt").c_str() );
		int N_DA;
		log << "N_DA init" << std::endl;
		antennaRealPositionsFile >> N_DA;
		log << "N_DA = " << N_DA << std::endl;
		
		double x;
		double y;
		double maxDifferX = 0;
		double maxDifferY = 0;
		double averageDifferX = 0;
		double averageDifferY = 0;
		log << "for cycle is ready" << std::endl;
		for(int i = 0; i < antenaResult.size(); i++){
			antennaRealPositionsFile >> x >> y;
			log << "== " << i << " ==\tx = " << x << ", y = " << y << std::endl;
			antennaCalibratedFile << "== " << i << " ==" << std::endl << "(" << x << ", " << y << ")\t(" << antenaResult[i].x << ", " << antenaResult[i].y << ")\tdiffer : " << fabs(antenaResult[i].x - x) << ", " << fabs(antenaResult[i].y - y) << std::endl;
			if(fabs(antenaResult[i].x - x) > maxDifferX){
				maxDifferX = fabs(antenaResult[i].x - x);
			}
			averageDifferX += fabs(antenaResult[i].x - x);
			if(fabs(antenaResult[i].y - y) > maxDifferY){
				maxDifferY = fabs(antenaResult[i].y - y);
			}
			averageDifferY += fabs(antenaResult[i].y - y);
		
			Q_EMIT modelSays(std::string("== ") + converters::intToString(i) + "==   (" + converters::doubleToString(antenaResult[i].x) + ", " + converters::doubleToString(antenaResult[i].y) + ")     diff.: (" + converters::doubleToString(fabs(antenaResult[i].x - x)) + ", " + converters::doubleToString(fabs(antenaResult[i].y - y)) + ")" );
		}
		if( antenaResult.size() != 0 ){
			averageDifferX /= antenaResult.size();
			averageDifferY /= antenaResult.size();
		} else {
			log << "ERROR! antenaResult.size() == 0" << std::endl;
		}
		
		antennaCalibratedFile << "max error in x : " << maxDifferX << std::endl;
		antennaCalibratedFile << "max error in y : " << maxDifferY << std::endl;
		
		antennaCalibratedFile << "average error in x : " << averageDifferX << std::endl;
		antennaCalibratedFile << "average error in y : " << averageDifferY << std::endl;
		
		Q_EMIT modelSays(std::string("max error :     (") + converters::doubleToString(maxDifferX) + ", " + converters::doubleToString(maxDifferY) + ")");
		Q_EMIT modelSays(std::string("aver error :     (") + converters::doubleToString(averageDifferX) + ", " + converters::doubleToString(averageDifferY) + ")");
		
		
		antennaRealPositionsFile.close();
		antennaCalibratedFile.close();
		
		
		
		
		
		
		
		
		
		
		
		project->setValue("calibrated", "yes");
		project->writeConfigFile();
		
		Q_EMIT modelFinished(std::string("calibrate"), std::string("OK"));
		Q_EMIT calibrationFinished();
		whatModelCanShow.antena = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		log << std::endl << "calibrate ret" << std::endl;
	}
}
std::vector<std::string>* model::findSingleGidrophoneFiles(){
	log << std::endl << "findSingleGidrophoneFiles call" << std::endl;
	Q_EMIT modelStarted(std::string("findSingleGidrophoneFiles"));

	if(findSingleGidrophoneFiles_isCached && !findSingleGidrophoneFiles_forcedCalculate){
		log << std::endl << "findSingleGidrophoneFiles \t(from cache)\tret" << std::endl;
		Q_EMIT modelFinished(std::string("findSingleGidrophoneFiles"), std::string("OK [cache]"));
		
		whatModelCanShow.signal = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		return findSingleGidrophoneFiles_cached;
	} else if(project->hasValue("parsed") && (project->getValue("parsed") == "yes") && project->hasValue("singleFilesNameMask") && !findSingleGidrophoneFiles_forcedCalculate){
	
		std::string singleFilesNameMask = project->getValue("singleFilesNameMask");
		
		std::vector<std::string>* gidrophoneFilePaths = new std::vector<std::string>;

		findSingleGidrophoneFiles_cached->clear();
		int gidrophonesAmount = project->getIntValue("gidrophonesAmount");
		for(int i = 0; i < gidrophonesAmount; i++){
			// make path from name
			std::string singleGidrophoneFilePath = projectFolderPath + "/" + singleFilesNameMask + converters::intToStringFourDigits(i);
			
			// add pushing path to file to vector
			log << "pushing singleGidrophoneFilePath" << std::endl;
			(*findSingleGidrophoneFiles_cached).push_back(singleGidrophoneFilePath);
			
			// test if the file can be opened
			std::ifstream testGidro(singleGidrophoneFilePath.c_str());
			if(!testGidro.is_open()){
				log << "can't open file " << singleGidrophoneFilePath << std::endl;
				whatModelCanShow.signal = false;
				Q_EMIT modelCanShow(whatModelCanShow);
				return NULL;
			} else {
				log << "checked file " << singleGidrophoneFilePath << " it's OK" << std::endl;
			}
			testGidro.close();
		}
		log << std::endl << "findSingleGidrophoneFiles ret OK [preloaded]" << std::endl;
		
		findSingleGidrophoneFiles_isCached = true;
		whatModelCanShow.signal = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		Q_EMIT modelFinished(std::string("findSingleGidrophoneFiles"), std::string("OK [preloaded]"));
		return findSingleGidrophoneFiles_cached;
		
	} else {

		//std::string singleFilesNameMask = project->getValue("singleFilesNameMask");
		std::string singleFilesNameMask = "gidro_";
		std::string path = escapeFileName(projectFilePath) + "/../" + project->getValue("signalFile");
	
		signalBinaryNachOtdelaParser* RAParser = new signalBinaryNachOtdelaParser( path , std::string("IACFiles") );
		
		int gidrophonesAmount;
		int signalLength;
		
		log << "extern RAParser->parse(" << singleFilesNameMask << ", output, output) call" << std::endl;
		bool parsedOK = RAParser->parse(singleFilesNameMask, gidrophonesAmount, signalLength, findSingleGidrophoneFiles_cached);
		log << "extern RAParser finished" << std::endl;
		
		project->setValue("gidrophonesAmount", gidrophonesAmount);
		//project->setValue("signalLength", project->getIntValue("signalLength"));
		project->setValue("signalLength", signalLength);
		project->setValue("singleFilesNameMask", singleFilesNameMask);
		project->setValue("parsed", "yes");
		
		project->writeConfigFile();

		Q_EMIT modelFinished(std::string("findSingleGidrophoneFiles"), std::string("OK"));
		Q_EMIT modelFinishedParsing(true);
		findSingleGidrophoneFiles_isCached = true;
		findSingleGidrophoneFiles_forcedCalculate = false;
		whatModelCanShow.signal = true;
		Q_EMIT modelCanShow(whatModelCanShow);
		log << std::endl << "findSingleGidrophoneFiles ret OK" << std::endl;
		return findSingleGidrophoneFiles_cached;
		
	}
}
std::string model::escapeFileName(std::string path){
	log << "escapeFileName(" << path << ") call" << std::endl;
	// we drop path behind last '/' symbol
	// siply go from end to front
	
	int pointer = path.size();
	
	while((path[pointer] != '/') && (path[pointer] != '\\')){
		pointer--;
	}
	
	path.resize(pointer);
	log << "escaped variant is: " << path << std::endl;
	return path;
}
std::string model::escapePath(std::string path){
	log << "escapePath(" << path << ") call" << std::endl;
	// we drop path behind last '/' symbol
	// siply go from end to front
	
	int pointer = path.size();
	int len = 0;
	
	while((path[pointer] != '/') && (path[pointer] != '\\')){
		pointer--;
		len++;
	}
	
	path = path.substr(pointer + 1, len);
	log << "escaped variant is: " << path << std::endl;
	return path;
}
bool model::newProjectFile(std::string path){
	log << "newProjectFile(" << path << ") call" << std::endl;

	Q_EMIT modelStarted(std::string("newProjectFile"));
	projectFolderPath = escapeFileName(path) + "/IACFiles";
	projectFilePath = projectFolderPath + "/project.txt";
	
	//mkdir(projectFolderPath.c_str());
	#ifdef PLATFORM_WINDOWS  
	CreateDirectory( (wchar_t*)( &((projectFolderPath.c_str())[0]) ), NULL );
	#else
	mkdir(projectFolderPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	#endif

	project->setNewFilePath(projectFilePath);
	project->setValue("projectFile", "IACProjectFile");
	
	
	whatModelCanShow.signal = false;
	whatModelCanShow.peakSignal = false;
	whatModelCanShow.cycles = false;
	whatModelCanShow.antena = false;
	Q_EMIT modelCanShow(whatModelCanShow);
	
	project->setValue("signalFile", escapePath(path));
	project->setValue("parsed", "no");
	project->setValue("peakSignalCreated", "no");
	project->setValue("cyclesChoosed", "no");
	project->setValue("calibrated", "no");
	
	modelData data;
	
	project->setValue("DFTDimention", 4096);
	project->setValue("overlay", 75);
	project->setValue("threshold", 40);
	project->setValue("bandLow", 1);
	project->setValue("bandHigh", 1000);
	project->setValue("soundSpeed", 1460.0);
	
	project->setValue("sourcesFile", "sources.txt");
	std::string sourcesFilePath = projectFolderPath + "/../sources.txt";
	log << "looking for sources records in file : " << sourcesFilePath << std::endl;
	std::ifstream sourcesCountFile(sourcesFilePath.c_str());

	std::string line;
	int count = 0;
	while(!sourcesCountFile.eof()){
		getline(sourcesCountFile,line);
		if(line != "") count++;
	}
	sourcesCountFile.close();
	project->setValue("sourcesAmount", count);
	log << "I found " << count << "sources" << std::endl;

	
	
	
	project->writeConfigFile();

	data.DFTDimention = project->getIntValue("DFTDimention");
	data.overlay = project->getIntValue("overlay");
	data.threshold = project->getIntValue("threshold");
	data.bandLow = project->getIntValue("bandLow");
	data.bandHigh = project->getIntValue("bandHigh");
	data.soundSpeed = project->getDoubleValue("soundSpeed");;
	data.manuallyFindCycles = false;
	
	Q_EMIT modelNewParameters(data);
	Q_EMIT modelFinished(std::string("newProjectFile"), std::string("OK"));
	log << "newProjectFile ret true" << std::endl;
	return true;
}	
bool model::openProjectFile(std::string path){
	Q_EMIT modelStarted(std::string("openProjectFile"));

	log << "openProjectFile(" << path << ") call" << std::endl;
	Q_EMIT modelStarted(std::string("openProjectFile"));
	projectFilePath = path;
	projectFolderPath = escapeFileName(projectFilePath);
	
	log << "projectFolderPath = " << projectFolderPath << std::endl;
	
	project->setNewFilePath(projectFilePath);
	if (!project->readConfigFile()) return false;
	if (!project->hasValue("projectFile")) return false;
	if (project->getValue("projectFile") != "IACProjectFile") return false;
	
	/*
	signalFile DA_1_1_15_081012.dat
	parsed no
	peakSignalCreated no
	cyclesChoosed no
	calibrated no
	*/
	log << "project->getValue(\"parsed\") = " << project->getValue("parsed") << std::endl;
	
	
	whatModelCanShow.signal = false;
	whatModelCanShow.peakSignal = false;
	whatModelCanShow.cycles = false;
	whatModelCanShow.antena = false;
	
	if(project->getValue("parsed") == "yes"){
		whatModelCanShow.signal = true;
	}
	if(project->getValue("peakSignalCreated") == "yes"){
		whatModelCanShow.peakSignal = true;
	}
	if(project->getValue("cyclesChoosed") == "yes"){
		whatModelCanShow.cycles = true;
	}
	if(project->getValue("calibrated") == "yes"){
		whatModelCanShow.antena = true;
	}
	
	Q_EMIT modelCanShow(whatModelCanShow);
	
	
	modelData data;
	
	if(project->hasValue("DFTDimention")){
		data.DFTDimention = project->getIntValue("DFTDimention");
	} else {
		data.DFTDimention = -1;
	}

	if(project->hasValue("overlay")){
		data.overlay = project->getIntValue("overlay");
	} else {
		data.overlay = -1;
	}

	if(project->hasValue("threshold")){
		data.threshold = project->getIntValue("threshold");
	} else {
		data.threshold = -1;
	}
	
	if(project->hasValue("bandLow")){
		data.bandLow = project->getIntValue("bandLow");
	} else {
		data.bandLow = 0;
	}
	if(project->hasValue("bandHigh")){
		data.bandHigh = project->getIntValue("bandHigh");
	} else {
		data.bandHigh = 1000;
	}
	
	if(project->hasValue("soundSpeed")){
		data.soundSpeed = project->getDoubleValue("soundSpeed");
	} else {
		data.soundSpeed = 1460.0;
	}
	
	/*
	if(project->hasValue("farawayZone")){
		if( project->getValue("farawayZone") == "yes" ){
			data.farawayZone = true;
		} else {
			data.farawayZone = false;
		}
	} else {
		data.farawayZone = true;
	}
	*/
	data.manuallyFindCycles = false;
	Q_EMIT modelNewParameters(data);
	
	log << "openProjectFile ret true" << std::endl;
	Q_EMIT modelFinished(std::string("openProjectFile"), std::string("OK"));

	return true;
}
inlineAntena model::giveAntena(){
	log << std::endl << "giveAntena call" << std::endl;
	//calibrate();
	return calibrator->giveAntena();
}
sourceSequence model::giveSources(){
	log << std::endl << "giveSources call" << std::endl;
	return calibrator->giveSources();
}
void model::sliceAndMakeFFT(int DFTDimention, int overlay, int threshold){
	
}
void model::takeCycles(std::vector<int>* arrayOfMarkers){
	log << std::endl << "takeCycles call" << std::endl;
	findCycles_cached = arrayOfMarkers;
	quickSort(findCycles_cached, 0, findCycles_cached->size() - 1);
	findCycles_isCached = true;
	log << std::endl << "takeCycles ret" << std::endl;
}
std::vector<int>* model::giveCycles(){
	if(findCycles_isCached){
		return findCycles_cached;
	} else {
		return NULL;
	}
}
std::vector<double>* model::giveFrequencies(){
	sourceSequence* sources = makeSources();
	std::vector<double>* frequencies = new std::vector<double>;
	int sourcesSize = sources->size();
	for(int i = 0; i < sourcesSize; i++){
		frequencies->push_back((*sources)[i].frequency);
	}
	return frequencies;
}

void model::quickSort(std::vector<int>* A, int low, int high){
	int i = low;
	int j = high;
	int x = (*A)[(low+high)/2];
	do {
		while((*A)[i] < x) ++i;
		while((*A)[j] > x) --j;
		if(i <= j){
			int temp = (*A)[i];
			(*A)[i] = (*A)[j];
			(*A)[j] = temp;
			i++;
			j--;
		}
	} while(i <= j);

	if(low < j) quickSort(A, low, j);
	if(i < high) quickSort(A, i, high);
}

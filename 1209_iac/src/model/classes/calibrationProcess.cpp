#include <vector>
#include <fstream>
#include <math.h>

#include "containers/arrays.h"

#include "model/classes/calibrationProcess.h"

#include "model/classes/inlineAntena.h"
#include "model/classes/sourceSequence.h"

#include "model/alglib/fasttransforms.h"
#include "model/alglib/ap.h"
#include "model/alglib/alglibmisc.h"
#include "model/alglib/alglibinternal.h"


#include "model/classes/commonMath.h"





double calibrationProcess::fik(double Re, double Im){
	
	double result = atan( fabs( Im / Re ) );
	
	
	
	if(Re >= 0){
		if(Im >= 0){
			return result;
		} else {
			return 2 * pi - result;
		}
	} else {
		if(Im >= 0){
			return pi - result;
		} else {
			return pi + result;
		}
	}
}
calibrationProcess::calibrationProcess(){
	
}
calibrationProcess::calibrationProcess(inlineAntena& givenAntena, sourceSequence& givenSources, tripleVector<double>& givenGidrophonesCalibrationData, int DFTDim, double soundSpeedGiven){
	log.open("calibrationProcess.log", std::ios::out);
	log << std::endl << "constructor call" << std::endl;
	
	soundSpeed = soundSpeedGiven;
	dftDimention = DFTDim;
	antena = givenAntena;
	sources = givenSources;
	gidrophonesCalibrationData = givenGidrophonesCalibrationData;
	gidrophonesCalibrationDataAfterFFT.resize(gidrophonesCalibrationData.sizex(), gidrophonesCalibrationData.sizey(), gidrophonesCalibrationData.sizez());
	log << "sources.size() =" << sources.size() << std::endl;
	log << "antena.size() =" << antena.size() << std::endl;
	for(int i = 0; i < sources.size(); i++){
		log << "for " << i << "-th source frequency is : " << sources[i].frequency << std::endl;
	}
	
	
	errorX = new std::vector<double>(antena.size());
	errorY = new std::vector<double>(antena.size());
}
void calibrationProcess::operator=(const calibrationProcess& newProcess){
	log << "operator= call" << std::endl;
	
	dftDimention = newProcess.dftDimention;
	antena = newProcess.antena;
	sources = newProcess.sources;
	gidrophonesCalibrationData = newProcess.gidrophonesCalibrationData;
	gidrophonesCalibrationDataAfterFFT.resize(gidrophonesCalibrationData.sizex(), gidrophonesCalibrationData.sizey(), gidrophonesCalibrationData.sizez());
}
calibrationProcess::~calibrationProcess(){
	log << std::endl << "destructor call" << std::endl;
	log.close();
}	
void calibrationProcess::makeFFT(){
	log << std::endl << "makeFFT call" << std::endl;
	log << "sources.size() =" << sources.size() << std::endl;
	for(int sourceNumber = 0; sourceNumber < sources.size(); sourceNumber++){
		log << "antena.size() =" << antena.size() << std::endl;
		for(int gidrophoneNumber = 0; gidrophoneNumber < antena.size() ; gidrophoneNumber++){
			std::vector<double> signalBeforeFFTHelpVector;
			log << "dftDimention =" << dftDimention << std::endl;
			for(int i = 0; i < dftDimention; i++){
				signalBeforeFFTHelpVector.push_back( gidrophonesCalibrationData(gidrophoneNumber, sourceNumber, i) );
			}
			alglib::real_1d_array signalBeforeFFT;
			signalBeforeFFT.setcontent(signalBeforeFFTHelpVector.size(),&signalBeforeFFTHelpVector[0]);
			
			alglib::complex_1d_array signalAfterFFT;
			alglib::fftr1d(signalBeforeFFT, signalAfterFFT);
			for(int i = 0; i < dftDimention; i++){
				gidrophonesCalibrationDataAfterFFT(gidrophoneNumber, sourceNumber, i) = signalAfterFFT[i];
			}
		}
	}
	log << std::endl << "makeFFT ret" << std::endl;
}
void calibrationProcess::makeMutualDelaysSmart(){
	log << std::endl << "makeMutualDelaysSmart call" << std::endl;
	mutualDelays.resize(antena.size() - 1, sources.size());

	double someFrequency;
	double phaseDifference;
	
	for(int sourceNumber = 0; sourceNumber < sources.size(); sourceNumber++){
		someFrequency = ((double)(sources[sourceNumber].frequency * dftDimention)) / frequencyDiscretization;
		
		int nearestIntegerFrequency = floor(someFrequency);
		if (someFrequency - nearestIntegerFrequency > 0.5){
			nearestIntegerFrequency++;
		}
		int nf = nearestIntegerFrequency;
		for(int gidrophoneNumber = 0; gidrophoneNumber < antena.size() - 1; gidrophoneNumber++){
			//log << "(" << gidrophoneNumber << ", " << sourceNumber << ") nearestIntegerFrequency was : " << nf << ", now it's : ";
			nearestIntegerFrequency = findMaximalDFTBean(&gidrophonesCalibrationDataAfterFFT, gidrophoneNumber, sourceNumber);
			//log << nearestIntegerFrequency << std::endl;
			phaseDifference =  -fik(gidrophonesCalibrationDataAfterFFT(gidrophoneNumber + 1, sourceNumber, nearestIntegerFrequency).x, gidrophonesCalibrationDataAfterFFT(gidrophoneNumber + 1, sourceNumber, nearestIntegerFrequency).y)
							+ fik(gidrophonesCalibrationDataAfterFFT(gidrophoneNumber, sourceNumber, nearestIntegerFrequency).x, gidrophonesCalibrationDataAfterFFT(gidrophoneNumber, sourceNumber, nearestIntegerFrequency).y);
			log << "mutualDelays(" << gidrophoneNumber << ", " << sourceNumber << ") ~ phaseDifference is :\t" << phaseDifference << "\t, corrected is : \t";
			if(fabs(phaseDifference) > pi){
				if(phaseDifference > 0){
					phaseDifference -= 2 * pi;
				} else {
					phaseDifference += 2 * pi;
				}
			}
			log << phaseDifference << std::endl;
			//log << "soundSpeed = " << soundSpeed << std::endl;
			mutualDelays(gidrophoneNumber, sourceNumber) = phaseDifference * soundSpeed / (2 * pi * sources[sourceNumber].frequency);
		}
	}
	log << std::endl << "makeMutualDelaysSmart ret" << std::endl;
}	
int calibrationProcess::findMaximalDFTBean(tripleVector<alglib::complex>* dataAfterFFT, int gidrophoneNumber, int sourceNumber){
	int size = dataAfterFFT->sizez() / 2;
	int maximalBean = 0;
	double max = sqr((*dataAfterFFT)(gidrophoneNumber, sourceNumber, 0).x) + sqr((*dataAfterFFT)(gidrophoneNumber, sourceNumber, 0).y);
	for(int i = 1; i < size; i++){
		if(max < sqr((*dataAfterFFT)(gidrophoneNumber, sourceNumber, i).x) + sqr((*dataAfterFFT)(gidrophoneNumber, sourceNumber, i).y)){
			max = sqr((*dataAfterFFT)(gidrophoneNumber, sourceNumber, i).x) + sqr((*dataAfterFFT)(gidrophoneNumber, sourceNumber, i).y);
			maximalBean = i;
		}
	}
	return maximalBean;
}
void calibrationProcess::makeRelativeDelays(){
	log << std::endl << "makeRelativeDelays call" << std::endl;
	relativeDelays.resize(antena.size(), sources.size());
	for(int i = 0; i < relativeDelays.sizex(); i++){
		for(int j = 0; j < relativeDelays.sizey(); j++){
			relativeDelays(i, j) = 0;
		}
	}
	for(int j = 0; j < relativeDelays.sizey(); j++){
		for(int i = 1; i < relativeDelays.sizex(); i++){
			relativeDelays(i, j) = relativeDelays(i - 1, j) + mutualDelays(i - 1, j);
		}
	}
	log << std::endl << "makeRelativeDelays ret" << std::endl;
}		

void calibrationProcess::smartCalibrateOneGidrophone(int gidrophoneNumber){
	log << std::endl << "smartCalibrateOneGidrophone(" << gidrophoneNumber << ") call" << std::endl;

	// метод калибровки одного гидрофона (калибровка каждого независима, поэтому нет смысла пихать их в одну процедуру)
	// зафодим переменную с его координатами, пишем туда нули чтобы усреднение сделать
	position evaluatedPosition(0,0,0);
	std::vector<position> evaluatedPositionForSource;
	evaluatedPositionForSource.clear();
	
	for(int j = 0; j < sources.size() / 2; j++){
		int j_other = j + (sources.size() / 2);
		// пишем в previousIteration то, где гидрофон стоит в исходной поданной на вход антенне
		// вообще сюда можно записать что угодно
		position previousIteration(antena[gidrophoneNumber].x, antena[gidrophoneNumber].y, antena[gidrophoneNumber].z);

		for(int iteration = 1; iteration < 10; iteration++){
			
			// считаем матрицу
			double a = -2 * ( sources[j].x  - previousIteration.x );
			double b = -2 * ( sources[j_other].x  - previousIteration.x );
			double c = -2 * ( sources[j].y  - previousIteration.y );
			double d = -2 * ( sources[j_other].y  - previousIteration.y );
			
			// ситаем невязку
			double discrepancy_j_1 = sqr( sources[j].distance(previousIteration) ) - sqr( sources[j].distance(antena[gidrophoneNumber - 1]) + mutualDelays(gidrophoneNumber - 1, j));
			double discrepancy_j = sqr( sources[j_other].distance(previousIteration) ) - sqr( sources[j_other].distance(antena[gidrophoneNumber - 1]) + mutualDelays(gidrophoneNumber - 1, j_other));
			
			// определитель матрицы
			double detOfBlock = a * d - b * c;
			
			// обратная транспонированная матрица
			double inv_a = d / detOfBlock;
			double inv_c = -1 * b / detOfBlock;
			double inv_b = -1 * c / detOfBlock;
			double inv_d = a / detOfBlock;
			
			// насколько надо подвинуть положение
			double shiftX = inv_a * discrepancy_j_1 + inv_b * discrepancy_j;
			double shiftY = inv_c * discrepancy_j_1 + inv_d * discrepancy_j;
			
			// исправили
			previousIteration.x -= shiftX;
			previousIteration.y -= shiftY;
		}
		// после итерирования - пишем это все в evaluatedPosition
		evaluatedPosition.x += previousIteration.x;
		evaluatedPosition.y += previousIteration.y;
		
		evaluatedPositionForSource.push_back( position(previousIteration.x, previousIteration.y, 0) );
		log << "== sources " << j << " -- " << j_other << " ==\t(" << previousIteration.x << ", " << previousIteration.y << ")" << std::endl;
	}
	// после проходпа по всем источникам - усредняем
	// тут (sources.size() - 1), потому что идем по парам соседних источников, и если есть
	// sources.size() источников, то есть (sources.size() - 1) соседей
	evaluatedPosition.x = evaluatedPosition.x / (sources.size() / 2);
	evaluatedPosition.y = evaluatedPosition.y / (sources.size() / 2);
	log << "== average ==\t(" << evaluatedPosition.x << ", " << evaluatedPosition.y << ")" << std::endl;
	
	(*errorX)[gidrophoneNumber] = 0;
	(*errorY)[gidrophoneNumber] = 0;
	for(int j = 1; j < sources.size(); j++){
		(*errorX)[gidrophoneNumber] += sqr(evaluatedPosition.x - evaluatedPositionForSource[j - 1].x);
		(*errorY)[gidrophoneNumber] += sqr(evaluatedPosition.y - evaluatedPositionForSource[j - 1].y);
	}
	(*errorX)[gidrophoneNumber] /= (sources.size() / 2);
	(*errorY)[gidrophoneNumber] /= (sources.size() / 2);
	(*errorX)[gidrophoneNumber] = sqrt((*errorX)[gidrophoneNumber]);
	(*errorY)[gidrophoneNumber] = sqrt((*errorY)[gidrophoneNumber]);
	log << "== square error ==\t(" << (*errorX)[gidrophoneNumber] << ", " << (*errorY)[gidrophoneNumber] << ")" << std::endl;
	
	
	antena[gidrophoneNumber].x = evaluatedPosition.x;
	antena[gidrophoneNumber].y = evaluatedPosition.y;
	log << std::endl << "smartCalibrateOneGidrophone ret" << std::endl;
}	
void calibrationProcess::calibrate3dOneGidrophone(int gidrophoneNumber){
	log << std::endl << "calibrate3dOneGidrophone call" << std::endl;

	// метод калибровки одного гидрофона (калибровка каждого независима, поэтому нет смысла пихать их в одну процедуру)
	// зафодим переменную с его координатами, пишем туда нули чтобы усреднение сделать
	position evaluatedPosition(0,0,0);
	
	for(int j = 1; j < sources.size() - 1; j++){

		// пишем в previousIteration то, где гидрофон стоит в исходной поданной на вход антенне
		// вообще сюда можно записать что угодно
		position previousIteration(antena[gidrophoneNumber].x, antena[gidrophoneNumber].y, antena[gidrophoneNumber].z);

		for(int iteration = 1; iteration < 10; iteration++){
			
			// считаем матрицу
			double a = -2 * ( sources[j - 1].x  - previousIteration.x );
			double b = -2 * ( sources[j].x  - previousIteration.x );
			double c = -2 * ( sources[j + 1].x  - previousIteration.x );
			
			double d = -2 * ( sources[j - 1].y  - previousIteration.y );
			double e = -2 * ( sources[j].y  - previousIteration.y );
			double f = -2 * ( sources[j + 1].y  - previousIteration.y );
			
			double g = -2 * ( sources[j - 1].z  - previousIteration.z );
			double h = -2 * ( sources[j].z  - previousIteration.z );
			double i = -2 * ( sources[j + 1].z  - previousIteration.z );
			
			// ситаем невязку
			double discrepancy_j_minus = sqr( sources[j - 1].distance(previousIteration) ) - sqr( sources[j - 1].distance(antena[0]) + relativeDelays(gidrophoneNumber, j - 1));
			double discrepancy_j = sqr( sources[j].distance(previousIteration) ) - sqr( sources[j].distance(antena[0]) + relativeDelays(gidrophoneNumber, j));
			double discrepancy_j_plus = sqr( sources[j + 1].distance(previousIteration) ) - sqr( sources[j + 1].distance(antena[0]) + relativeDelays(gidrophoneNumber, j + 1));
			
			// определитель матрицы
			double detOfBlock = a * e * i + b * f * g + c * h * d - c * e * g - a * f * h - b * d * i;
			
			// обратная транспонированная матрица
			double inv_a = (e * i - f * h) / detOfBlock;
			double inv_b = (f * g - d * i) / detOfBlock;
			double inv_c = (d * h - e * g) / detOfBlock;
			
			double inv_d = (c * h - b * i) / detOfBlock;
			double inv_e = (a * i - c * g) / detOfBlock;
			double inv_f = (b * g - a * h) / detOfBlock;
			
			double inv_g = (b * f - e * c) / detOfBlock;
			double inv_h = (c * d - a * f) / detOfBlock;
			double inv_i = (a * e - b * d) / detOfBlock;
			
			
			// насколько надо подвинуть положение
			double shiftX = inv_a * discrepancy_j_minus + inv_b * discrepancy_j + inv_c * discrepancy_j_plus;
			double shiftY = inv_d * discrepancy_j_minus + inv_e * discrepancy_j + inv_f * discrepancy_j_plus;
			double shiftZ = inv_g * discrepancy_j_minus + inv_h * discrepancy_j + inv_i * discrepancy_j_plus;
			
			// исправили
			previousIteration.x -= shiftX;
			previousIteration.y -= shiftY;
			previousIteration.z -= shiftZ;
		}
		// после итерирования - пишем это все в evaluatedPosition
		evaluatedPosition.x += previousIteration.x;
		evaluatedPosition.y += previousIteration.y;
		evaluatedPosition.z += previousIteration.z;
	}
	// после проходпа по всем источникам - усредняем
	// тут (sources.size() - 2), потому что идем по тройкам соседних источников, и если есть
	// sources.size() источников, то есть (sources.size() - 2) троек соседей
	evaluatedPosition.x = evaluatedPosition.x / (sources.size() - 2);
	evaluatedPosition.y = evaluatedPosition.y / (sources.size() - 2);
	evaluatedPosition.z = evaluatedPosition.z / (sources.size() - 2);
	
	antena[gidrophoneNumber].x = evaluatedPosition.x;
	antena[gidrophoneNumber].y = evaluatedPosition.y;
	antena[gidrophoneNumber].z = evaluatedPosition.z;
}		

void calibrationProcess::correctMutualDelays(){
	log << std::endl << "correctMutualDelays call" << std::endl;
	int antenaSize = antena.size();
	int sourcesSize = sources.size();
	log << "antenaSize = " << antenaSize << std::endl;
	log << "sourcesSize = " << sourcesSize << std::endl;
	for(int gidrophoneNumber = 1; gidrophoneNumber < antenaSize; gidrophoneNumber++){
		log << "gidrophoneNumber = " << gidrophoneNumber << std::endl;
		for(int sourceNumber = 0; sourceNumber < sourcesSize; sourceNumber++){
			log << "sourceNumber = " << sourceNumber << std::endl;
			double c = antena[gidrophoneNumber].distance(antena[0]);
			double a = sources[sourceNumber].distance(antena[0]);
			double b = sources[sourceNumber].distance(antena[gidrophoneNumber]);
			
			double correctingCoefficient = (a * a + b * b - c * c) / (2 * a * b);
			log << "correctingCoefficient = " << correctingCoefficient << std::endl;
			mutualDelays(gidrophoneNumber - 1, sourceNumber) = mutualDelays(gidrophoneNumber - 1, sourceNumber) * correctingCoefficient;
		}
	}
	log << std::endl << "correctMutualDelays ret" << std::endl;
}
void calibrationProcess::smartCalibrate(){
	log << std::endl << "smartCalibrate call" << std::endl;
	makeFFT();
	makeMutualDelaysSmart();
	makeRelativeDelays();
	for(int i = 1; i < antena.size(); i++){
		smartCalibrateOneGidrophone(i);
	}
	
	
	log << std::endl << "smartCalibrate ret" << std::endl;
}
inlineAntena calibrationProcess::giveAntena(){
	log << std::endl << "giveAntena call" << std::endl;

	log << std::endl << "giveAntena ret" << std::endl;
	return antena;
}		
sourceSequence calibrationProcess::giveSources(){
	log << std::endl << "giveSources call" << std::endl;
	
	log << std::endl << "giveSources ret" << std::endl;
	return sources;
}
std::vector<double>* calibrationProcess::giveErrorX(){
	return errorX;
}
std::vector<double>* calibrationProcess::giveErrorY(){
	return errorY;
}
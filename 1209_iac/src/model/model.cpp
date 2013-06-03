#include <QtGui>

/* DEBUG */
// #include <iostream>


#ifdef PLATFORM_WINDOWS
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif


#include "model/model.h"


#include "model/misc/converters.h"
#include "model/misc/physicalConstants.h"



model::model(){
	
}

model::~model(){

}

void model::run(){
	exec();
}

void model::setParameters(calibrationParameters parameters){
	qDebug() << "setParameters";
	Q_EMIT avaliableResources(avaliable_resources);
}

void model::calibrate(){

}

inlineAntena model::antena(){

}

sourceSequence model::sources(){

}

dualVector<double>* model::signal(){

}

dualVector<double>* model::FFTSignal(int cycleNumber){

}

dualVector<double>* model::peakSignal(){

}

std::vector<int>* model::cycles(){

}

#ifndef MODEL_H
#define MODEL_H

#include <QtCore>

#include "containers/arrays.h"
#include "containers/calibrationParameters.h"
#include "containers/modelAvaliableResources.h"

#include "model/parser/configParser.h"
#include "model/parser/signalBinaryNachOtdelaParser.h"
#include "model/classes/inlineAntena.h"
#include "model/classes/calibrationProcess.h"
#include "model/classes/sourceSequence.h"
#include "model/classes/sourceCircleSequence.h"

#include <string>


class model : public QThread{ // QObject{

	Q_OBJECT
	
	private:
		
		std::string projectFolderPath;
		modelAvaliableResources avaliable_resources;

		void run();
		
	public:

		model(void);
		~model(void);
		
		void calibrate();
		
		inlineAntena antena();
		sourceSequence sources();
		dualVector<double>* signal();
		dualVector<double>* FFTSignal(int cycleNumber);
		dualVector<double>* peakSignal();
		std::vector<int>* cycles();
		
	public Q_SLOTS:

		void setParameters(calibrationParameters parameters);
	
	
	Q_SIGNALS:

		//void modelAvaliableResources(int avaliable_resources);
		void avaliableResources(modelAvaliableResources avaliable_resources);

};
#endif
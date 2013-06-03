#ifndef MAINVIEW_H
#define MAINVIEW_H


#include <QtGui>

#include "model/model.h"
//#include "model/modelThread.h"
#include "model/parser/configParser.h"

#include "../release/ui/ui_IACMainWindow.h"

class mainView : public QWidget, private Ui::IacMainWindow {
	
    Q_OBJECT
	
	private:
		QString signalFilePath;
		int DFTDimention;
		int overlay;

		int historyIndention;

		// QThread* model_thread;
		model* main_model;
		
	
	public:

		//myModel* mainModel;
		mainView(QWidget *parent=0);

	public Q_SLOTS:

		//void findCycles();
		void parseSignalFile();
		void showPeakAndFFTSignal();
		void showSignal();
		//void sliceAndMakeFFT();
		void calibrate();
		void openProjectFile();
		void newProjectFile();
		

		
		void takeParametersFromModel(calibrationParameters data);
		void setParametersClicked();

		void chooseCyclesManually(dualVector<double>*);

		void modelStarted(std::string);
		void modelFinished(std::string, std::string);
		void modelSays(std::string);
		
		void calibrationFinished();
		void showAntena();
		void showCycles();

		void model_avaliableResources(modelAvaliableResources whatModelCanShow);

	
	Q_SIGNALS:

		void signalFileOpen(QString path);
		void giveSignal();

		void model_setParameters(calibrationParameters);
	
};
#endif
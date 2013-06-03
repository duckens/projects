#include <QtGui>


#include "containers/arrays.h"
#include "view/mainView.h"


//#include "view/signalForm.h"
//#include "view/FFTSignalForm.h"
//#include "view/chooseCyclesForm.h"
//#include "view/calibratedAntForm.h"


mainView::mainView(QWidget *parent){
	
	setupUi(this);
    

    // model_thread = new QThread;
	main_model = new model;
	// main_model->moveToThread(model_thread);

	connect( pushButton_newProjectFile, SIGNAL( clicked() ), this, SLOT( newProjectFile() ) );
	connect( pushButton_openProjectFile, SIGNAL( clicked() ), this, SLOT( openProjectFile() ) );

	connect( pushButton_showSignal, SIGNAL( clicked() ), this, SLOT( showSignal() ) );
	connect( pushButton_showPeakAndFFTSignal, SIGNAL( clicked() ), this, SLOT( showPeakAndFFTSignal() ) );
	connect( pushButton_showCycles, SIGNAL( clicked() ), this, SLOT( showCycles() ) );
	connect( pushButton_showAntenna, SIGNAL( clicked() ), this, SLOT( showAntena() ) );
	
	connect( pushButton_setParameters, SIGNAL( clicked() ), this, SLOT( setParametersClicked() ) );
	connect( pushButton_calibrate, SIGNAL( clicked() ), this, SLOT( calibrate() ) );
	
	qRegisterMetaType<calibrationParameters>("calibrationParameters");
	qRegisterMetaType<modelAvaliableResources>("modelAvaliableResources");
	connect( main_model, SIGNAL(avaliableResources(modelAvaliableResources)), this, SLOT(model_avaliableResources(modelAvaliableResources)), Qt::QueuedConnection);
	connect( this, SIGNAL(model_setParameters(calibrationParameters)), main_model, SLOT(setParameters(calibrationParameters)), Qt::QueuedConnection);
}

void mainView::setParametersClicked(){
	calibrationParameters calibration_parameters;
	
	calibration_parameters.DFTDimention = spinBox_DFTDimention->value();
	calibration_parameters.overlay = spinBox_overlay->value();
	calibration_parameters.threshold = spinBox_threshold->value();
	calibration_parameters.bandHigh = spinBox_bandHigh->value();
	calibration_parameters.bandLow = spinBox_bandLow->value();
	calibration_parameters.soundSpeed = doubleSpinBox_soundSpeed->value();
	calibration_parameters.manuallyFindCycles = checkBox_manuallyFindCycles->checkState();

	Q_EMIT model_setParameters(calibration_parameters);
	// QMetaObject::invokeMethod(main_model, "setParameters",  Qt::QueuedConnection,  Q_ARG(calibrationParameters, calibration_parameters));
}

void mainView::showAntena(){
	/*
	calibratedAntForm *form = new calibratedAntForm(mainModel);
	form->show();
	*/
}
void mainView::model_avaliableResources(modelAvaliableResources whatModelCanShow){
	qDebug() << "model_avaliableResources came";
	pushButton_showSignal->setEnabled(whatModelCanShow.signal);
	pushButton_showPeakAndFFTSignal->setEnabled(whatModelCanShow.peakSignal);
	pushButton_showCycles->setEnabled(whatModelCanShow.cycles);
	pushButton_showAntenna->setEnabled(whatModelCanShow.antena);

	pushButton_setParameters->setEnabled(true);
	pushButton_calibrate->setEnabled(true);
}
void mainView::takeParametersFromModel(calibrationParameters data){
	spinBox_DFTDimention->setValue( data.DFTDimention );
	spinBox_overlay->setValue( data.overlay );
	spinBox_threshold->setValue( data.threshold );
	spinBox_bandHigh->setValue( data.bandHigh );
	spinBox_bandLow->setValue( data.bandLow );
	doubleSpinBox_soundSpeed->setValue( data.soundSpeed );
	

	if(data.manuallyFindCycles){
		checkBox_manuallyFindCycles->setCheckState( Qt::Checked );
		label_manuallyFindCycles->setText("yes");
	} else {
		checkBox_manuallyFindCycles->setCheckState( Qt::Unchecked );
		label_manuallyFindCycles->setText("no");
	}

	label_soundSpeed->setText(QString::number(data.soundSpeed));
	label_DFTDimention->setText(QString::number(data.DFTDimention));
	label_overlay->setText(QString::number(data.overlay));
	label_threshold->setText(QString::number(data.threshold));
	label_bandHigh->setText(QString::number(data.bandHigh));
	label_bandLow->setText(QString::number(data.bandLow));
}
void mainView::modelSays(std::string replica){
	
	listWidget_history->addItem( QString( replica.c_str() ) );
}
void mainView::modelStarted(std::string method){
	
	listWidget_history->addItem( QString( method.c_str() ) );
}
void mainView::modelFinished(std::string method, std::string result){
	
	listWidget_history->addItem( QString( (method + "\t:\t" + result).c_str() ) );
}


void mainView::newProjectFile(){
	QString path;

	path = QFileDialog::getOpenFileName(
		this,
		"Choose a file to open",
		QString::null,
		QString::null);
		
	if(path != ""){
		
		/*
		if(mainModel->newProjectFile(path.toStdString())){
			label_fileProjectPath->setText(path);
		} else {
			label_fileProjectPath->setText("Error opening project file");
			pushButton_calibrate->setEnabled(false);
			pushButton_takeParameters->setEnabled(false);

			pushButton_displaySignal->setEnabled(false);
			pushButton_showPeakAndFFTSignal->setEnabled(false);
			pushButton_showCycles->setEnabled(false);
			pushButton_showAntena->setEnabled(false);
			
			label_DFTDimention->setText("-");
			label_overlay->setText("-");
			label_threshold->setText("-");
			label_bandHigh->setText("-");
			label_bandLow->setText("-");
			label_soundSpeed->setText("-");
			label_manuallyFindCycles->setText("-");
		}
		*/
	}
}
void mainView::openProjectFile(){
	QString path;

	path = QFileDialog::getOpenFileName(
		this,
		"Choose a file to open",
		QString::null,
		QString::null);
		
	if(path != ""){
		/*
		if(mainModel->openProjectFile(path.toStdString())){
			label_fileProjectPath->setText(path);
		} else {
			label_fileProjectPath->setText("Error opening project file");
			//pushButton_parseSignalSignal->setEnabled(false);
			pushButton_calibrate->setEnabled(false);
			pushButton_takeParameters->setEnabled(false);

			pushButton_displaySignal->setEnabled(false);
			pushButton_showPeakAndFFTSignal->setEnabled(false);
			pushButton_showCycles->setEnabled(false);
			pushButton_showAntena->setEnabled(false);
			
			label_DFTDimention->setText("-");
			label_overlay->setText("-");
			label_threshold->setText("-");
			label_bandHigh->setText("-");
			label_bandLow->setText("-");
			label_soundSpeed->setText("-");
			//label_farawayZone->setText("-");
			label_manuallyFindCycles->setText("-");
		}
		*/
	}
}

void mainView::parseSignalFile(){
	//mainModel->parseSignalFile();
}
void mainView::showSignal(){
	/*
	dualVector<double>* signal = mainModel->giveSignal();
	
	signalForm *form = new signalForm(signal);
	form->show();
	*/
}


void mainView::chooseCyclesManually(dualVector<double>* peakSignal){
	/*
	chooseCyclesForm* form = new chooseCyclesForm(mainModel);
	form->show();
	log << "chooseCyclesManually ret" << std::endl;
	*/
}

void mainView::showPeakAndFFTSignal(){
	/*
	FFTSignalForm *form = new FFTSignalForm(mainModel);
	form->show();
	*/
}
void mainView::showCycles(){
	/*
	chooseCyclesForm* form = new chooseCyclesForm(mainModel, true);
	form->show();
	*/
}


void mainView::calibrationFinished(){
	showAntena();
}
	
void mainView::calibrate(){
	/*
	mainModel->calibrate();
	*/
}

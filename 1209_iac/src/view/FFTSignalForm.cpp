#include <vector>

#include <QtGui>
#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>

#include <qwt_counter.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>

#include "classes/scrollZoomer.h"

#include "pixmaps/pixmaps.h"
#include "classes/markerList.h"

#include "containers/arrays.h"
#include "FFTSignalForm.h"

FFTSignalForm::FFTSignalForm(model* mainModelGiven){

	// open log file
	log.open("FFTSignalForm.log");
	log << "constructor call" << std::endl;
	FFTSignal = NULL;
	justDeletedMaker = NULL;



	// attach ui form
	setupUi(this);


	//overlay = spinBox_overlay->value();
	//DFTDimention = spinBox_DFTDimention->value();
	// create model (all math is in there)
	mainModel = mainModelGiven;
	// ask to evaluate peak signal
	//askMainModelToEvaluate();
	// and take it
	peakSignal = mainModel->givePeakSignal();




	// peak signal plot
	qwtPlot_peakSignal->setAutoReplot(false);

	qwtPlot_peakSignal->setCanvasBackground(QColor(Qt::darkBlue));
	qwtPlot_peakSignal->setTitle("peak signal");
	qwtPlot_peakSignal->setAxisTitle(QwtPlot::xBottom, "FFT cycles");
	qwtPlot_peakSignal->setAxisTitle(QwtPlot::yLeft, "Amplitude");

	QwtLegend *legend = new QwtLegend;
    qwtPlot_peakSignal->insertLegend(legend, QwtPlot::BottomLegend);

	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->enableXMin(true);
	grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
	grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
	grid->attach(qwtPlot_peakSignal);

	peakSignalCurve1 = new QwtPlotCurve("Gidrophone 0 peakSignal");
	peakSignalCurve1->setPen(QPen(Qt::cyan));
	peakSignalCurve1->attach(qwtPlot_peakSignal);

	peakSignalCurve2 = new QwtPlotCurve("Gidrophone 0 peakSignal");
	peakSignalCurve2->setPen(QPen(Qt::yellow));
	peakSignalCurve2->attach(qwtPlot_peakSignal);

	showPeakSignal1(0);
	showPeakSignal2(0);

	qwtPlot_peakSignal->setAutoReplot(true);
	// end of peak signal plot




	//FFTSignal
	qwtPlot_FFTCycle->setAutoReplot(false);

	qwtPlot_FFTCycle->setCanvasBackground(QColor(Qt::darkBlue));
	qwtPlot_FFTCycle->setTitle("FFT signal");
	qwtPlot_FFTCycle->setAxisTitle(QwtPlot::xBottom, "FFT beans");
	qwtPlot_FFTCycle->setAxisTitle(QwtPlot::yLeft, "Amplitude");

	QwtLegend *legendFFTSignal = new QwtLegend;
    qwtPlot_FFTCycle->insertLegend(legendFFTSignal, QwtPlot::BottomLegend);

	QwtPlotGrid *gridFFTSignal = new QwtPlotGrid;
	gridFFTSignal->enableXMin(true);
	gridFFTSignal->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
	gridFFTSignal->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
	gridFFTSignal->attach(qwtPlot_FFTCycle);

	FFTSignalCurve1 = new QwtPlotCurve("Gidrophone 0 peakSignal");
	FFTSignalCurve1->setPen(QPen(Qt::cyan));
	FFTSignalCurve1->attach(qwtPlot_FFTCycle);

	FFTSignalCurve2 = new QwtPlotCurve("Gidrophone 0 peakSignal");
	FFTSignalCurve2->setPen(QPen(Qt::yellow));
	FFTSignalCurve2->attach(qwtPlot_FFTCycle);

	qwtPlot_FFTCycle->setAutoReplot(true);
	// end of FFT signal plot



	// zoomer and picker and smth else
	// recreare zoomer each time we get new data in the plot
	reCreateZoomer();
	// end of zoomer and picker and smth else




	// toolbar
	QToolBar *toolBar = new QToolBar(this);




	// zoom button
    btnZoom = new QToolButton(toolBar);
    btnZoom->setText("Zoom");
    btnZoom->setIcon(QIcon(zoom_xpm));
    btnZoom->setCheckable(true);
    btnZoom->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(btnZoom);
    connect(btnZoom, SIGNAL(toggled(bool)), SLOT(enableZoomMode(bool)));
	// end of zoom button




	// marker
	peakSignalMarker = new QwtPlotMarker();
	peakSignalMarker->setValue(0.0, 0.0);
	peakSignalMarker->setLineStyle(QwtPlotMarker::VLine);
	peakSignalMarker->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
	peakSignalMarker->setLinePen(QPen(Qt::green, 0, Qt::DashDotLine));
	peakSignalMarker->attach(qwtPlot_peakSignal);
	// end of marker




	// marker list
	//listOfMarkers = new markerList(listWidget_FFTCycles, qwtPlot_peakSignal, peakSignalMarker);
	// end of marker list




	// spin boxes
	spinBox_gidrophone1->setMinimum(0);
	spinBox_gidrophone1->setMaximum(peakSignal->sizex() - 1);
	spinBox_gidrophone2->setMinimum(0);
	spinBox_gidrophone2->setMaximum(peakSignal->sizex() - 1);

	connect(spinBox_gidrophone1, SIGNAL( valueChanged(int) ), this, SLOT( showPeakSignal1(int) ) );
	connect(spinBox_gidrophone2, SIGNAL( valueChanged(int) ), this, SLOT( showPeakSignal2(int) ) );
	// end of spin boxes




	// spin boxes 2
	spinBox_FFTGidrophone1->setMinimum(0);
	spinBox_FFTGidrophone1->setMaximum(0);
	spinBox_FFTGidrophone2->setMinimum(0);
	spinBox_FFTGidrophone2->setMaximum(0);
	connect(spinBox_FFTGidrophone1, SIGNAL( valueChanged(int) ), this, SLOT( showFFTSignal1(int) ) );
	connect(spinBox_FFTGidrophone2, SIGNAL( valueChanged(int) ), this, SLOT( showFFTSignal2(int) ) );
	// end of spin boxes 2




	// evaluate button
	// (defined in ui form)
	//connect( pushButton_evaluate, SIGNAL( clicked() ), this, SLOT( evaluatePeakSignal() ) );
	// end of evaluate button




	// signal picker
	// (defined in function reCreateZoomer())
	connect(peakSignalPicker, SIGNAL(moved(const QPoint &)), this, SLOT(showCurrentMarker(const QPoint &)));
	connect(peakSignalPicker, SIGNAL(appended(const QPoint &)), this, SLOT(showCurrentMarker(const QPoint &)));
	// end of signal picker

	//connect(pushButton_addFFTCycle, SIGNAL( clicked() ), listOfMarkers, SLOT( addMarker() ) );
	//connect(pushButton_removeCycle, SIGNAL( clicked() ), listOfMarkers, SLOT( removeMarker() ) );
	//connect(pushButton_FFTCycleShiftLeft, SIGNAL( clicked() ), listOfMarkers, SLOT( shiftCurrentMarkerLeft() ) );
	//connect(pushButton_FFTCycleShiftRight, SIGNAL( clicked() ), listOfMarkers, SLOT( shiftCurrentMarkerRight() ) );
	//connect(pushButton_writeSlicedSignal, SIGNAL( clicked() ), this, SLOT( writeSlicedSignal() ) );


	//connect(listWidget_FFTCycles, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ), this, SLOT( chooseCycleItem( QListWidgetItem*, QListWidgetItem* ) ) );
	//connect(listWidget_FFTCycles, SIGNAL( 	itemActivated( QListWidgetItem* ) ), listOfMarkers, SLOT( markerSelected( QListWidgetItem* ) ) );
}
FFTSignalForm::~FFTSignalForm(){
	log.close();
}
void FFTSignalForm::reCreateZoomer(){
	// zoomer
	peakSignalZoomer = new ScrollZoomer( /*QwtPlot::xBottom, QwtPlot::yLeft, */
		qwtPlot_peakSignal->canvas());
	peakSignalZoomer->setRubberBand(QwtPicker::RectRubberBand);
	peakSignalZoomer->setRubberBandPen(QColor(Qt::green));
	peakSignalZoomer->setTrackerMode(QwtPicker::ActiveOnly);
	peakSignalZoomer->setTrackerPen(QColor(Qt::white));

    // panner
	peakSignalPanner = new QwtPlotPanner(qwtPlot_peakSignal->canvas());
	peakSignalPanner->setMouseButton(Qt::MidButton);

	// picker
	peakSignalPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
		qwtPlot_peakSignal->canvas());
	peakSignalPicker->setStateMachine(new QwtPickerDragPointMachine());
	peakSignalPicker->setRubberBandPen(QColor(Qt::green));
	peakSignalPicker->setRubberBand(QwtPicker::CrossRubberBand);
	peakSignalPicker->setTrackerPen(QColor(Qt::white));

	enableZoomMode(false);
}
void FFTSignalForm::showPeakSignal1(int gidrophoneNumber){

	const bool doReplot = qwtPlot_peakSignal->autoReplot();

	double* x = new double[peakSignal->sizey()];
	double* y = new double[peakSignal->sizey()];

	for(int i = 0; i < peakSignal->sizey(); i++){
		x[i] = i;
		y[i] = (*peakSignal)(gidrophoneNumber, i);
	}

	peakSignalCurve1->setSamples(x, y, peakSignal->sizey());

	char str2[10];
	//itoa(gidrophoneNumber, str2, 10);
	sprintf(str2, "%d", gidrophoneNumber);
	char str[80];
	strcpy (str,"Gidrophone ");
	strcat (str, str2);
	strcat (str," peak signal");

	peakSignalCurve1->setTitle(str);
	qwtPlot_peakSignal->setAutoReplot(doReplot);
}
void FFTSignalForm::showPeakSignal2(int gidrophoneNumber){

	const bool doReplot = qwtPlot_peakSignal->autoReplot();

	double* x = new double[peakSignal->sizey()];
	double* y = new double[peakSignal->sizey()];

	for(int i = 0; i < peakSignal->sizey(); i++){
		x[i] = i;
		y[i] = (*peakSignal)(gidrophoneNumber, i);
	}

	peakSignalCurve2->setSamples(x, y, peakSignal->sizey());

	char str2[10];
	//itoa(gidrophoneNumber, str2, 10);
	sprintf(str2, "%d", gidrophoneNumber);
	char str[80];
	strcpy (str,"Gidrophone ");
	strcat (str, str2);
	strcat (str," peak signal");

	peakSignalCurve2->setTitle(str);
	qwtPlot_peakSignal->setAutoReplot(doReplot);
}
/*
void FFTSignalForm::askMainModelToEvaluate(){
	log << "askMainModelToEvaluate call" << std::endl;
	log << "going to call mainModel->sliceAndMakeFFT(" << DFTDimention << ", " << overlay << ")" << std::endl;

	mainModel->sliceAndMakeFFT(DFTDimention, overlay, spinBox_threshold->value());
}
*/

void FFTSignalForm::emptyFFTPlot(){
	if(FFTSignal != NULL){
		log << "delete  FFTSignal" << std::endl;
		delete FFTSignal;
		FFTSignal = NULL;
	}
	FFTSignal = new dualVector<double>(0,0);
	showFFTSignal1(0);
	showFFTSignal2(0);
	spinBox_FFTGidrophone1->setValue(0);
	spinBox_FFTGidrophone2->setValue(0);
	spinBox_FFTGidrophone1->setMaximum(0);
	spinBox_FFTGidrophone2->setMaximum(0);
}
void FFTSignalForm::enableZoomMode(bool on){
    peakSignalPanner->setEnabled(on);

    peakSignalZoomer->setEnabled(on);
    //peakSignalZoomer->zoom(0);

    peakSignalPicker->setEnabled(!on);
}
void FFTSignalForm::showCurrentMarker(const QPoint & pos){
	log << std::endl << "showCurrentMarker call" << std::endl;
	if(!peakSignalZoomer->isEnabled()){

		//listOfMarkers->deselectAllMarkers();

		peakSignalMarker->setLinePen(QPen(Qt::green, 0, Qt::DashDotLine));

		int cycleNumber = (int)qwtPlot_peakSignal->invTransform(QwtPlot::xBottom, pos.x());
		if(cycleNumber < 0) cycleNumber = 0;
		if(cycleNumber > peakSignal->sizey() - 1) cycleNumber = peakSignal->sizey() - 1;

		QString str;
		str.sprintf("FFT signal, cycle number = %d",
			cycleNumber
		);
		qwtPlot_FFTCycle->setTitle(str);
		//label_where->setText(str);

		if(FFTSignal != NULL){
			log << "delete  FFTSignal" << std::endl;
			delete FFTSignal;
			FFTSignal = NULL;
		}
		FFTSignal = mainModel->GiveFFTSignal(cycleNumber);


		spinBox_FFTGidrophone1->setMaximum((*FFTSignal).sizex() - 1);
		spinBox_FFTGidrophone2->setMaximum((*FFTSignal).sizex() - 1);
		showFFTSignal1(spinBox_FFTGidrophone1->value());
		showFFTSignal2(spinBox_FFTGidrophone2->value());

		//QString label = tr("ÃÛÃÛÃÛ cycle: %1").arg(cycleNumber);
		QString label;
		label.sprintf("FFT cycle: %d", cycleNumber);

		QwtText text(label);
		text.setFont(QFont("Helvetica", 10, QFont::Bold));
		text.setColor(Qt::green);

		peakSignalMarker->setValue((double)cycleNumber, 0.0);
		peakSignalMarker->setLabel(text);
	}
}
void FFTSignalForm::showFFTSignal1(int gidrophoneNumber){

	const bool doReplot = qwtPlot_FFTCycle->autoReplot();

	double* x = new double[(*FFTSignal).sizey()];
	double* y = new double[(*FFTSignal).sizey()];

	for(int i = 0; i < (*FFTSignal).sizey(); i++){
		x[i] = i;
		y[i] = (*FFTSignal)(gidrophoneNumber, i);
	}

	FFTSignalCurve1->setSamples(x, y, (*FFTSignal).sizey());

	QString str;
	str.sprintf("Gidrophone %d FFT signal",
		gidrophoneNumber
	);


	FFTSignalCurve1->setTitle(str);
	qwtPlot_FFTCycle->setAutoReplot(doReplot);
}
void FFTSignalForm::showFFTSignal2(int gidrophoneNumber){

	const bool doReplot = qwtPlot_FFTCycle->autoReplot();

	double* x = new double[(*FFTSignal).sizey()];
	double* y = new double[(*FFTSignal).sizey()];

	for(int i = 0; i < (*FFTSignal).sizey(); i++){
		x[i] = i;
		y[i] = (*FFTSignal)(gidrophoneNumber, i);
	}

	FFTSignalCurve2->setSamples(x, y, (*FFTSignal).sizey());

	QString str;
	str.sprintf("Gidrophone %d FFT signal",
		gidrophoneNumber
	);


	FFTSignalCurve2->setTitle(str);
	qwtPlot_FFTCycle->setAutoReplot(doReplot);
}
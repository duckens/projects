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


#include "containers/arrays.h"
#include "signalForm.h"




signalForm::signalForm(dualVector<double>* gidroSignal){
	log.open("signalForm.log");
	log << "constructor call" << std::endl;

	setupUi(this);
	
	signal = gidroSignal;
	
	qwtPlot_signal->setAutoReplot(false);
	
	//plot
	qwtPlot_signal->setCanvasBackground(QColor(Qt::darkBlue));
	qwtPlot_signal->setTitle("Signal");
	qwtPlot_signal->setAxisTitle(QwtPlot::xBottom, "Time points");
	qwtPlot_signal->setAxisTitle(QwtPlot::yLeft, "Amplitude");
	
	//legend
	QwtLegend *legend = new QwtLegend;
    qwtPlot_signal->insertLegend(legend, QwtPlot::BottomLegend);
	
	//grid
	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->enableXMin(true);
	grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
	grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
	grid->attach(qwtPlot_signal);


	
	//curve
	signalCurve = new QwtPlotCurve("Gidrophone 0 signal");
	signalCurve->setPen(QPen(Qt::yellow));
	signalCurve->attach(qwtPlot_signal);
	

	//display signal curve
	showSignalFromGidrophone(0);	
	
	
	
	
	// zoomer
	signalZoomer = new ScrollZoomer( /*QwtPlot::xBottom, QwtPlot::yLeft, */
		qwtPlot_signal->canvas());
	signalZoomer->setRubberBand(QwtPicker::RectRubberBand);
	signalZoomer->setRubberBandPen(QColor(Qt::green));
	signalZoomer->setTrackerMode(QwtPicker::ActiveOnly);
	signalZoomer->setTrackerPen(QColor(Qt::white));

    // panner
    signalPanner = new QwtPlotPanner(qwtPlot_signal->canvas());
    signalPanner->setMouseButton(Qt::MidButton);
	
	// picker
    signalPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, 
        qwtPlot_signal->canvas());
    signalPicker->setStateMachine(new QwtPickerDragPointMachine());
    signalPicker->setRubberBandPen(QColor(Qt::green));
    signalPicker->setRubberBand(QwtPicker::CrossRubberBand);
    signalPicker->setTrackerPen(QColor(Qt::white));

    //setCentralWidget(qwtPlot_signal);
	
	// toolbar
	QToolBar *toolBar = new QToolBar(this);

	
	// zoom button
    QToolButton *btnZoom = new QToolButton(toolBar);
    btnZoom->setText("Zoom");
    btnZoom->setIcon(QIcon(zoom_xpm));
    btnZoom->setCheckable(true);
    btnZoom->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(btnZoom);
	
    connect(btnZoom, SIGNAL(toggled(bool)), SLOT(enableZoomMode(bool)));
	
	
	// adjust spinboxes and connect to showSignalFromGidrophone()
	spinBox_gidrophonenumber->setMinimum(0);
	spinBox_gidrophonenumber->setMaximum(signal->sizex() - 1);
	
	connect(spinBox_gidrophonenumber, SIGNAL( valueChanged(int) ), this, SLOT( showSignalFromGidrophone(int) ) );
	
	enableZoomMode(false);
	qwtPlot_signal->setAutoReplot(true);

}
void signalForm::showSignalFromGidrophone(int gidrophoneNumber){
	log << "showSignalFromGidrophone(" << gidrophoneNumber << ") call" << std::endl;

	const bool doReplot = qwtPlot_signal->autoReplot();
	
	int size;
	int coeff;
	/*
	coeff = signal->sizey() / 10000;
	if (coeff < 1){
		size = signal->sizey();
		coeff = 1;
	} else {
		size = signal->sizey() / coeff;
	}
	*/
	size = signal->sizey();
	coeff = 1;
	log << "\t signal->sizey() = " << signal->sizey() << std::endl << "\t size = " << size << std::endl << "\t coeff = " << coeff << std::endl;
	
	double* x = new double[size];
	double* y = new double[size];
		
	for(int i = 0; i < size; i++){
		x[i] = i;
		y[i] = (*signal)(gidrophoneNumber, i * coeff);
	}
	
	signalCurve->setSamples(x, y, size);
	
	char str2[10];
	//itoa(gidrophoneNumber, str2, 10);
	sprintf(str2, "%d", gidrophoneNumber);
	char str[80];
	strcpy (str,"Gidrophone ");
	strcat (str, str2);
	strcat (str," signal");
	
	signalCurve->setTitle(str);
	qwtPlot_signal->setAutoReplot(doReplot);
}


// zooming
void signalForm::enableZoomMode(bool on){
	log << "enableZoomMode call" << std::endl;
	signalPanner->setEnabled(on);

	signalZoomer->setEnabled(on);
	signalZoomer->zoom(0);
	
	signalPicker->setEnabled(!on);
}

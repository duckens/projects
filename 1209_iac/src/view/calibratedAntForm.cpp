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

#include "model/classes/inlineAntena.h"
#include "view/calibratedAntForm.h"

#include "classes/scrollZoomer.h"
#include "pixmaps/pixmaps.h"


calibratedAntForm::calibratedAntForm(model* mainModel){
	log.open("calibratedAntForm.log");
	log << "constructor call" << std::endl;
	
	
	
	
	// attach ui form
	setupUi(this);
	
	
	
	
	// get data about antena and sources from model
	inlineAntena antena = mainModel->giveAntena();
	sourceSequence sources = mainModel->giveSources();
	int gigrophonesAmount = antena.size();
	
	double antennaNumber[gigrophonesAmount];
	double antennaX[gigrophonesAmount];
	double antennaY[gigrophonesAmount];
	
	
	
	log << "**\n calibrated antena \n**" << std::endl;
	log << "(x, y)" << std::endl;
	
	// translate it into simple arrays (required for )
	for(int i = 0; i < gigrophonesAmount; i++){
		antennaNumber[i] = i;
		antennaX[i] = antena[i].x;
		antennaY[i] = antena[i].y;
		log << "(" << antena[i].x << ", "<< antena[i].y << ")" << std::endl;
	}
	
	
	
	
	// qwtplot antena x and antena y
	qwtPlot_calibratedAnt->setAutoReplot(false);
	
	qwtPlot_calibratedAnt->setCanvasBackground(QColor(Qt::darkBlue));
	qwtPlot_calibratedAnt->setTitle("Antenna");
	qwtPlot_calibratedAnt->setAxisTitle(QwtPlot::xBottom, "Gidrophone number");
	qwtPlot_calibratedAnt->setAxisTitle(QwtPlot::yLeft, "Position [m]");
	
	QwtLegend *legend = new QwtLegend;
    qwtPlot_calibratedAnt->insertLegend(legend, QwtPlot::BottomLegend);
	
	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->enableXMin(true);
	grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
	grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
	grid->attach(qwtPlot_calibratedAnt);
	
	QwtPlotCurve* peakSignalCurve1 = new QwtPlotCurve("Antenna X");
	peakSignalCurve1->setPen(QPen(Qt::cyan));
	peakSignalCurve1->attach(qwtPlot_calibratedAnt);

	QwtPlotCurve* peakSignalCurve2 = new QwtPlotCurve("AntennaY");
	peakSignalCurve2->setPen(QPen(Qt::yellow));
	peakSignalCurve2->attach(qwtPlot_calibratedAnt);
	
	peakSignalCurve1->setSamples(antennaNumber, antennaX, gigrophonesAmount);
	peakSignalCurve2->setSamples(antennaNumber, antennaY, gigrophonesAmount);
	
	
	peakSignalCurve1->setTitle("X");
	peakSignalCurve2->setTitle("Y");

	qwtPlot_calibratedAnt->setAutoReplot(true);
	// end of qwtplot antena x and antena y
	
	
	

	// plot "map"
	qwtPlot_map->setAutoReplot(false);
	
	qwtPlot_map->setCanvasBackground(QColor(Qt::darkBlue));
	qwtPlot_map->setTitle("Antenna");
	qwtPlot_map->setAxisTitle(QwtPlot::xBottom, "Gidrophone number");
	qwtPlot_map->setAxisTitle(QwtPlot::yLeft, "Position [m]");
	
	QwtLegend *legend_map = new QwtLegend;
    qwtPlot_map->insertLegend(legend_map, QwtPlot::BottomLegend);
	
	QwtPlotGrid *grid_map = new QwtPlotGrid;
	grid_map->enableXMin(true);
	grid_map->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
	grid_map->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
	grid_map->attach(qwtPlot_map);
	
	QwtPlotCurve* antenaCurve = new QwtPlotCurve("Antenna X");
	antenaCurve->setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	antenaCurve->attach(qwtPlot_map);
	
	for(int i = 0; i < sources.size(); i++){
		
		QwtPlotCurve* sourceCrossH = new QwtPlotCurve("source");
		sourceCrossH->setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		sourceCrossH->attach(qwtPlot_map);
		double* sourceXh = new double[2];
		double* sourceYh = new double[2];
		sourceXh[0] = sources[i].x;
		sourceXh[1] = sources[i].x;
		sourceYh[0] = sources[i].y - 100;
		sourceYh[1] = sources[i].y + 100;
		
		sourceCrossH->setSamples(sourceXh, sourceYh, 2);
		
		
		
		QwtPlotCurve* sourceCrossV = new QwtPlotCurve("source");
		sourceCrossV->setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		sourceCrossV->attach(qwtPlot_map);
		double* sourceXv = new double[2];
		double* sourceYv = new double[2];
		sourceXv[0] = sources[i].x - 100;
		sourceXv[1] = sources[i].x + 100;
		sourceYv[0] = sources[i].y;
		sourceYv[1] = sources[i].y;
		
		sourceCrossV->setSamples(sourceXv, sourceYv, 2);
		/*
		QwtPlotMarker* marker = new QwtPlotMarker();
		marker->setValue(sources[i].x, sources[i].y);
		marker->setLineStyle(QwtPlotMarker::Cross);
		marker->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
		marker->setLinePen(QPen(QColor(200,150,0), 0, Qt::DashDotLine));
		marker->setSymbol( new QwtSymbol(QwtSymbol::Diamond, 
			QColor(Qt::yellow), QColor(Qt::green), QSize(7,7)));
		marker->attach(qwtPlot_map);
		*/
	}
	
	antenaCurve->setSamples(antennaX, antennaY, gigrophonesAmount);
	
	antenaCurve->setTitle("antena");
	
	qwtPlot_map->setAutoReplot(true);
	// end of plot "map"
	
	
	
	// zoom buttons
	toolButton_zoomCalibratedAnt->setText("Zoom");
	toolButton_zoomCalibratedAnt->setIcon(QIcon(zoom_xpm));
	toolButton_zoomCalibratedAnt->setCheckable(true);
	toolButton_zoomCalibratedAnt->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	connect(toolButton_zoomCalibratedAnt, SIGNAL(toggled(bool)), SLOT(enableZoomModeAntena(bool)));

	toolButton_zoomMap->setText("Zoom");
	toolButton_zoomMap->setIcon(QIcon(zoom_xpm));
	toolButton_zoomMap->setCheckable(true);
	toolButton_zoomMap->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	connect(toolButton_zoomMap, SIGNAL(toggled(bool)), SLOT(enableZoomModeMap(bool)));
	// end of zoom buttons
	
	
	
	// zoomer for antena
	// zoomer
	peakSignalZoomer_antena = new ScrollZoomer( /*QwtPlot::xBottom, QwtPlot::yLeft, */
		qwtPlot_calibratedAnt->canvas());
	peakSignalZoomer_antena->setRubberBand(QwtPicker::RectRubberBand);
	peakSignalZoomer_antena->setRubberBandPen(QColor(Qt::green));
	peakSignalZoomer_antena->setTrackerMode(QwtPicker::ActiveOnly);
	peakSignalZoomer_antena->setTrackerPen(QColor(Qt::white));

    // panner
	peakSignalPanner_antena = new QwtPlotPanner(qwtPlot_calibratedAnt->canvas());
	peakSignalPanner_antena->setMouseButton(Qt::MidButton);
	
	// picker
	peakSignalPicker_antena = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, 
		qwtPlot_calibratedAnt->canvas());
	peakSignalPicker_antena->setStateMachine(new QwtPickerDragPointMachine());
	peakSignalPicker_antena->setRubberBandPen(QColor(Qt::green));
	peakSignalPicker_antena->setRubberBand(QwtPicker::CrossRubberBand);
	peakSignalPicker_antena->setTrackerPen(QColor(Qt::white));

	enableZoomModeAntena(false);
	// end of zoomer for antena


	// zoomer for map
	// zoomer
	peakSignalZoomer_map = new ScrollZoomer( /*QwtPlot::xBottom, QwtPlot::yLeft, */
		qwtPlot_map->canvas());
	peakSignalZoomer_map->setRubberBand(QwtPicker::RectRubberBand);
	peakSignalZoomer_map->setRubberBandPen(QColor(Qt::green));
	peakSignalZoomer_map->setTrackerMode(QwtPicker::ActiveOnly);
	peakSignalZoomer_map->setTrackerPen(QColor(Qt::white));

    // panner
	peakSignalPanner_map = new QwtPlotPanner(qwtPlot_map->canvas());
	peakSignalPanner_map->setMouseButton(Qt::MidButton);
	
	// picker
	peakSignalPicker_map = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, 
		qwtPlot_map->canvas());
	peakSignalPicker_map->setStateMachine(new QwtPickerDragPointMachine());
	peakSignalPicker_map->setRubberBandPen(QColor(Qt::green));
	peakSignalPicker_map->setRubberBand(QwtPicker::CrossRubberBand);
	peakSignalPicker_map->setTrackerPen(QColor(Qt::white));

	enableZoomModeMap(false);
	// end of zoomer for map
	
}
calibratedAntForm::~calibratedAntForm(){
	log << "destructor call" << std::endl;
	log.close();
}
void calibratedAntForm::enableZoomModeAntena(bool on){
    peakSignalPanner_antena->setEnabled(on);
    peakSignalZoomer_antena->setEnabled(on);
    peakSignalPicker_antena->setEnabled(!on);
}
void calibratedAntForm::enableZoomModeMap(bool on){
    peakSignalPanner_map->setEnabled(on);
    peakSignalZoomer_map->setEnabled(on);
    peakSignalPicker_map->setEnabled(!on);
}
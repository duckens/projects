#ifndef CHOOSECYCLESFORM_H
#define CHOOSECYCLESFORM_H

#include <QtGui>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>

#include<fstream>
#include<vector>


#include "containers/arrays.h"

#include "model/model.h"
#include "classes/FFTCycleMarker.h"
#include "classes/markerList.h"

#include "ui_chooseCyclesForm.h"


class chooseCyclesForm : public QWidget, private Ui::chooseCyclesForm {

    Q_OBJECT
	
	public:
		model* mainModel;
		dualVector<double>* peakSignal;
		dualVector<double>* FFTSignal;

		std::ofstream log;
		
		int DFTDimention;
		int overlay;
		
		markerList* listOfMarkers;
		
		chooseCyclesForm(model* mainModelGiven, bool showAlreadyPrecalcedCycles = false);
		~chooseCyclesForm();
		//void askMainModelToEvaluate();
		void reCreateZoomer();
		void reCreateZoomer2nd();

		void emptyFFTPlot();
		//void hideCurrentMarker();
		
		//void highlightCycleItem( QListWidgetItem* itemMarker );
		//void dehighlightCycleItem( QListWidgetItem* itemMarker );
		//void dehighlightCurrentCycleItem();
		//void dropAllMarkers();
		//void removeMarker(QListWidgetItem* marker);

		QToolButton* btnZoom;
		
		QwtPlotCurve* FFTSignalCurve1;
		QwtPlotCurve* FFTSignalCurve2;
		QwtPlotCurve* peakSignalCurve1;
		QwtPlotCurve* peakSignalCurve2;
		
		QwtPlotZoomer* peakSignalZoomer;
		QwtPlotPicker* peakSignalPicker;
		QwtPlotPanner* peakSignalPanner;
		
		QwtPlotZoomer* FFTSignalZoomer;
		QwtPlotPicker* FFTSignalPicker;
		QwtPlotPanner* FFTSignalPanner;
		
		QwtPlotMarker* peakSignalMarker;
		
		
		QListWidgetItem* justDeletedMaker;
		
		//std::vector<FFTCycleMarker*> FFTCycleMarkers;
		
	
	public Q_SLOTS:
	
		void showPeakSignal1(int gidrophoneNumber);
		void showPeakSignal2(int gidrophoneNumber);
		
		void showFFTSignal1(int gidrophoneNumber);
		void showFFTSignal2(int gidrophoneNumber);
		
		void enableZoomMode(bool);
		void enableZoomMode2nd(bool);
		void showCurrentMarker(const QPoint &);
	
		void proceedCalibrate();
	

	
	
};
#endif
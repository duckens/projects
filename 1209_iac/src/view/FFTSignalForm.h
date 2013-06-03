#ifndef FFTSIGNALFORM_H
#define FFTSIGNALFORM_H

#include <QtGui>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>

#include <fstream>
#include <vector>


#include "containers/arrays.h"

#include "model/model.h"
#include "classes/FFTCycleMarker.h"
#include "classes/markerList.h"

#include "../release/ui/ui_FFTSignalForm.h"


class FFTSignalForm : public QWidget, private Ui::FFTSignalForm {

    Q_OBJECT
	
	public:
		model* mainModel;
		dualVector<double>* peakSignal;
		dualVector<double>* FFTSignal;

		std::ofstream log;
		
		int DFTDimention;
		int overlay;
		
		markerList* listOfMarkers;
		
		FFTSignalForm(model*);
		~FFTSignalForm();
		//void askMainModelToEvaluate();
		void reCreateZoomer();
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
		
		QwtPlotMarker* peakSignalMarker;
		
		
		QListWidgetItem* justDeletedMaker;
		
		//std::vector<FFTCycleMarker*> FFTCycleMarkers;
		
	
	public Q_SLOTS:
	
		void showPeakSignal1(int gidrophoneNumber);
		void showPeakSignal2(int gidrophoneNumber);
		
		void showFFTSignal1(int gidrophoneNumber);
		void showFFTSignal2(int gidrophoneNumber);
		
		//void evaluatePeakSignal(void);
		void enableZoomMode(bool);
		
		void showCurrentMarker(const QPoint &);
		//void addFFTCycle();
		
	//	void chooseCycleItem( QListWidgetItem*, QListWidgetItem* );
	//	void removeCurrentCycle();
		
		//void shiftCurrentCycleLeft();
		//void shiftCurrentCycleRight();
		
		//void writeSlicedSignal();
		//void addMarker();

	
	
};
#endif
#ifndef CALIBRATEDANTFORM_H
#define CALIBRATEDANTFORM_H

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





#include "../release/ui/ui_calibratedAntForm.h"


class calibratedAntForm : public QWidget, private Ui::calibratedAntForm {

    Q_OBJECT
	
	private:
		std::ofstream log;
		
		QwtPlotZoomer* peakSignalZoomer_antena;
		QwtPlotPicker* peakSignalPicker_antena;
		QwtPlotPanner* peakSignalPanner_antena;
		
		QwtPlotZoomer* peakSignalZoomer_map;
		QwtPlotPicker* peakSignalPicker_map;
		QwtPlotPanner* peakSignalPanner_map;
	
	public:
		
		calibratedAntForm(model*);
		~calibratedAntForm();
	
	public Q_SLOTS:
	
		void enableZoomModeAntena(bool);
		void enableZoomModeMap(bool);
		

	
	
};
#endif
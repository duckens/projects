#ifndef SIGNALFORM_H
#define SIGNALFORM_H

#include <fstream>


#include <QtGui>
#include <qwt_plot_curve.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>


#include "containers/arrays.h"


#include "../release/ui/ui_signalForm.h"


class signalForm : public QWidget, private Ui::MySignalForm {

    Q_OBJECT
	
	private:
		std::ofstream log;
	
	public:
		dualVector<double>* signal;
		QwtPlotCurve *signalCurve;
		
		QwtPlotZoomer *signalZoomer;
		QwtPlotPicker *signalPicker;
		QwtPlotPanner *signalPanner;
		
		signalForm(dualVector<double>*);
	
	public Q_SLOTS:
	
		void showSignalFromGidrophone(int gidrophoneNumber);

	    void enableZoomMode(bool);

	
};
#endif
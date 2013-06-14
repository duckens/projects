#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qwt_plot_curve.h>

#include "ui_mainWindow.h"
#include "../signalFileReader/signalFileReader.h"

class mainWindow : public QMainWindow, private Ui::MainWindow {

	Q_OBJECT
	
	private:

		std::vector<int>* data;
		dualVector<int>* data_dual;
		
		signalFileReader* reader;
		double* x;
		double* y;

		QwtPlotCurve *signalCurve;

	public:

		mainWindow(QWidget* parent = 0);

	public Q_SLOTS:

		void openFileButtonClick();
		void getData(int fictiveParameter);
	
	Q_SIGNALS:

};

#endif

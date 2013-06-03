#ifndef FFTCYCLEMARKER_H
#define FFTCYCLEMARKER_H


#include <QtGui>
#include <qwt_plot_marker.h>


class FFTCycleMarker : public QListWidgetItem, public QwtPlotMarker{
	private:
		int FFTCycleNumber;
	public:
		FFTCycleMarker(int number, double maxInFrequency = 0);
		~FFTCycleMarker();
		void setCycleNumber(int number);
		int cycleNumber();
	
};
#endif
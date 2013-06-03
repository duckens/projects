#include <QtGui>

#include "view/classes/FFTCycleMarker.h"

FFTCycleMarker::~FFTCycleMarker(){
	this->detach();
}
FFTCycleMarker::FFTCycleMarker(int number, double maxInFrequency){
	this->setCycleNumber(number);
	this->setValue(number, 0.0);
	this->setLineStyle(QwtPlotMarker::VLine);
	this->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
	this->setLinePen(QPen(Qt::cyan, 0, Qt::DashDotLine));
	QString str;
	if(maxInFrequency == 0){
		str = QObject::tr("cycle number : %1").arg(number);
	} else {
		str = QObject::tr("Precalced : %1, freq : %2").arg(number).arg(maxInFrequency);
	}
	this->setText(str);
}
void FFTCycleMarker::setCycleNumber(int number){
	FFTCycleNumber = number;
	this->setXValue(number);
}

int FFTCycleMarker::cycleNumber(){
	return FFTCycleNumber;
}
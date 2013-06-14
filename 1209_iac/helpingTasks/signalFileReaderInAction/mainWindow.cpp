#include "mainWindow.h"

mainWindow::mainWindow(QWidget* parent){
	
	setupUi(this);

	data = NULL;
	data_dual = NULL;

	x = NULL;
	y = NULL;

	reader = new signalFileReader();


	qwtPlot_signal->setCanvasBackground(QColor(Qt::darkBlue));
	qwtPlot_signal->setTitle("Signal");
	qwtPlot_signal->setAxisTitle(QwtPlot::xBottom, "Measurments");
	qwtPlot_signal->setAxisTitle(QwtPlot::yLeft, "Amplitude");

	signalCurve = new QwtPlotCurve("Gidrophone 0 signal");
	signalCurve->setPen(QPen(Qt::yellow));
	signalCurve->attach(qwtPlot_signal);

	spinBox_borderLow->setValue(20480);
	spinBox_borderSize->setValue(50);

	connect( pushButton_openFile, SIGNAL( clicked() ), this, SLOT( openFileButtonClick() ) );
	connect( checkBox_dual, SIGNAL( stateChanged(int) ), this, SLOT( getData(int) ) );
	connect( spinBox_borderLow, SIGNAL( valueChanged(int) ), this, SLOT( getData(int) ) );
	connect( spinBox_borderSize, SIGNAL( valueChanged(int) ), this, SLOT( getData(int) ) );
	connect( spinBox_gidroNumber, SIGNAL( valueChanged(int) ), this, SLOT( getData(int) ) );


}

void mainWindow::openFileButtonClick(){
	bool OK = reader->open(lineEdit_filename->text());
	qDebug() << "opened OK:" << OK;
}

void mainWindow::getData(int fictiveParameter){
	int L = spinBox_borderLow->value();
	int Size = spinBox_borderSize->value();

	if(data != NULL){
		delete data;
		data = NULL;
	}
	if(data_dual != NULL){
		delete data_dual;
		data_dual = NULL;
	}

	if(x != NULL){
		delete[] x;
	}
	if(y != NULL){
		delete[] y;
	}

	qDebug() << "Looking for:" << reader->startTime() + L;
	int dataSize;
	
	int gidroNumber = spinBox_gidroNumber->value();

	if(checkBox_dual->isChecked()){


		data_dual = reader->data( reader->startTime() + L, Size );
		qDebug() << "data OK";
		dataSize = data_dual->sizey();
		qDebug() << "dataSize:" << dataSize;

		x = new double[dataSize];
		y = new double[dataSize];
			
		for(int i = 0; i < dataSize; i++){
			x[i] = i;
			y[i] = (double)((*data_dual)(gidroNumber, i));
		}

	} else {
		data = reader->data( reader->startTime() + L, Size, gidroNumber );
		qDebug() << "data OK:" << (*data)[0];
		
		dataSize = data->size();
		qDebug() << "dataSize:" << dataSize;

		x = new double[dataSize];
		y = new double[dataSize];
			
		for(int i = 0; i < dataSize; i++){
			x[i] = i;
			y[i] = (double)((*data)[i]);
		}
		
	}

	signalCurve->setSamples(x, y, dataSize);
	qwtPlot_signal->replot();

	
}

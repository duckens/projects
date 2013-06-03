#include <QtCore>
#include <QtGui>

#include "clockThread.h"

int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	// build gui
	QWidget widget;
	QLabel *label = new QLabel;
	QHBoxLayout *layout = new QHBoxLayout(&widget);
	layout->addWidget(label);
	widget.setWindowTitle("clock");

	//instantiate thread object
	ClockThread clockThread;
	QObject::connect(&clockThread, SIGNAL(sendTime(QString)), label, SLOT(setText(QString)), Qt::QueuedConnection);
	clockThread.start();
	widget.show();
	app.exec();
	clockThread.quit();
	clockThread.wait();
	return 0;
}
#ifndef HELLOTHREAD_H
#define HELLOTHREAD_H

#include <QtCore>
#include <QtGui>

class HelloThread : public QThread{
	Q_OBJECT
private:
	void run();
};

#endif

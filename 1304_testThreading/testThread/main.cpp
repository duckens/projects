#include <QtCore>
#include <QtGui>
#include "helloThread.h"


int main(int argc, char *argv[]){
	QCoreApplication app(argc, argv);
	HelloThread thread;
	thread.start();
	qDebug() << "hello from GUI thread " << app.thread()->currentThreadId();
	thread.wait();  // do not exit before the thread is completed!
	return 0;
}
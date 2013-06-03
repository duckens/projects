#include <QtCore>
#include <QtGui>

int main(int argc, char *argv[]){
	QCoreApplication app(argc, argv);
	Thread thread;
	qDebug() << "main thread ID: " << app.thread()->currentThreadId();
	WorkerObject *worker = new WorkerObject;
	thread.launchWorker(worker);
	QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);
	QMetaObject::invokeMethod(worker, "startPolling",  Qt::QueuedConnection,  Q_ARG(int, 500));
	//let application produce output for 3 seconds and quit
	QTimer::singleShot(3000, &app, SLOT(quit()));
	app.exec();
	thread.stop();
	thread.wait();
	delete worker;
	return 0;
}
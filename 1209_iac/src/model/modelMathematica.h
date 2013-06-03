#ifndef MODELMATHEMATICA_H
#define MODELMATHEMATICA_H

#include <QtCore>


class modelMathematica : public QThread {

	Q_OBJECT
	
	private:

		void run();
		
	public:

		void receiveData(void* data);
		void receiveTask(int task);
		
	Q_SIGNALS:

		void modelMathematicaFinished(int task, void* result);

};

#endif

#ifndef MODELTHREAD_H
#define MODELTHREAD_H

#include <QtCore>
#include "model/model.h"

class modelThread : public QThread {

	Q_OBJECT

	private:
		model* mainModel;
		void run();

	signals:

		void modelState(int state, QString stateDescription);

	public slots:

		void setParameters(calibrationParameters parameters);

};

#endif
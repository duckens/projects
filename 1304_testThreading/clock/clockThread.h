#ifndef CLOCKTHREAD_H
#define CLOCKTHREAD_H

#include <QtCore>
#include <QtGui>

class ClockThread : public QThread {
	Q_OBJECT

	signals:
		void sendTime(QString time);
	private:
		void run();
		QString m_lastTime;
	private slots:
		void timerHit();

};

#endif

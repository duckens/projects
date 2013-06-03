#ifndef MARKERLIST_H
#define MARKERLIST_H

#include <list>
#include <fstream>
#include <vector>
#include <QtGui>

#include <qwt_plot_marker.h>

class markerList : public QObject{

	Q_OBJECT

	private:
		//std::list<FFTCycleMarker*> markers;
		//std::list<FFTCycleMarker*>::iterator currentMarker;
		QListWidget* listWidget;
		QwtPlot* plot;
		QwtPlotMarker* pointerMarker;
		
		std::ofstream log;

	public:
		markerList(QListWidget* lw, QwtPlot* pl, QwtPlotMarker* pointMark);
		
		
		
		void highlightMarker( QListWidgetItem* itemMarker );
		void dehighlightMarker( QListWidgetItem* itemMarker );
		void dehighlightCurrentMarker();
		void deselectAllMarkers();
		
		std::vector<int>* giveCycleNumbers();


		
	public Q_SLOTS:
		void addMarker();
		void addPrecalcedMarker(int value, double maxInFrequency);
		void removeMarker(QListWidgetItem* marker);
		void removeMarker();
		void dropAllMarkers();
		
		void shiftCurrentMarkerLeft();
		void shiftCurrentMarkerRight();
		
		void markerSelected(QListWidgetItem* item);
};
#endif
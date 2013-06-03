#include <QtGui>
#include <fstream>
#include <vector>

#include "view/classes/markerList.h"
#include "view/classes/FFTCycleMarker.h"


#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>

#include <qwt_counter.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>



markerList::markerList(QListWidget* lw, QwtPlot* pl, QwtPlotMarker* pointMark){
	log.open("markerList.log");
	log << "constructor call" << std::endl;
	
	listWidget = lw;
	plot = pl;
	pointerMarker = pointMark;
}
void markerList::addMarker(){
	log << "addMarker call" << std::endl;
	int number = pointerMarker->xValue();
	//log << "int number = pointerMarker->xValue(); done, number = " << number << std::endl;
	
	FFTCycleMarker* newMarker = new FFTCycleMarker( pointerMarker->xValue() );
	//log << "FFTCycleMarker* newMarker = new FFTCycleMarker( pointerMarker->xValue() ); done" << std::endl;
	
	// store in list
	//markers.push_back(newMarker);
	// attach to plot
	newMarker->attach(plot);
	//log << "newMarker->attach(plot); done" << std::endl;
	// add to listWidget
	listWidget->addItem(newMarker);
	
	//log << "addMarker exit" << std::endl;
}
void markerList::addPrecalcedMarker(int value, double maxInFrequency){
	log << "addPrecalcedMarker call" << std::endl;
	//log << "int number = pointerMarker->xValue(); done, number = " << number << std::endl;
	
	FFTCycleMarker* newMarker = new FFTCycleMarker( value, maxInFrequency );
	//log << "FFTCycleMarker* newMarker = new FFTCycleMarker( pointerMarker->xValue() ); done" << std::endl;
	
	// store in list
	//markers.push_back(newMarker);
	// attach to plot
	newMarker->attach(plot);
	//log << "newMarker->attach(plot); done" << std::endl;
	// add to listWidget
	listWidget->addItem(newMarker);
	
	log << "addPrecalcedMarker ret" << std::endl;
}
void markerList::removeMarker(QListWidgetItem* marker){
	log << "removeMarker(QListWidgetItem*) call" << std::endl;
	listWidget->removeItemWidget(marker);
	delete marker;
	//plot->replot();
}
void markerList::removeMarker(){
	log << "removeMarker() call" << std::endl;
	if(listWidget->currentItem() != NULL){
		removeMarker(listWidget->currentItem());
	}
}
void markerList::dropAllMarkers(){
	log << "dropAllMarkers call" << std::endl;
	while(listWidget->count()){
		removeMarker(listWidget->takeItem(0));
	}
}
void markerList::shiftCurrentMarkerLeft(){
	log << "shiftCurrentMarkerLeft call" << std::endl;
	FFTCycleMarker* marker = (FFTCycleMarker*)(listWidget->currentItem());
	if(marker != NULL){
		int oldValue = marker->cycleNumber();
		marker->setCycleNumber(oldValue - 1);
	}
}
void markerList::shiftCurrentMarkerRight(){
	log << "shiftCurrentMarkerRight call" << std::endl;
	FFTCycleMarker* marker = (FFTCycleMarker*)(listWidget->currentItem());
	if(marker != NULL){
		int oldValue = marker->cycleNumber();
		marker->setCycleNumber(oldValue + 1);
	}
}
void markerList::markerSelected(QListWidgetItem* item){
	log << "markerSelected call" << std::endl;
	highlightMarker(listWidget->currentItem());
	
}
void markerList::deselectAllMarkers(){
	log << "deselectAllMarkers call" << std::endl;
	if(listWidget->currentItem() != NULL){
		listWidget->currentItem()->setSelected(false);
		dehighlightMarker(listWidget->currentItem());
	}
}
void markerList::highlightMarker( QListWidgetItem* itemMarker ){
	log << "highlightMarker(QListWidgetItem*) call" << std::endl;
	
}
void markerList::dehighlightMarker( QListWidgetItem* itemMarker ){
	log << "dehighlightMarker(QListWidgetItem*) call" << std::endl;
	
}
void markerList::dehighlightCurrentMarker(){
	log << "dehighlightCurrentMarker call" << std::endl;
	
}
std::vector<int>* markerList::giveCycleNumbers(){
	log << "giveCycleNumbers call" << std::endl;
	std::vector<int>* cycleNumbersVector = new std::vector<int>;
	//cycleNumbersVector->push_back(470); 
	int n = listWidget->count(); 
	while(n){
		int markerNumber = ((FFTCycleMarker*)(listWidget->takeItem(--n)))->cycleNumber();
		cycleNumbersVector->push_back(markerNumber);
	}
	return cycleNumbersVector;
}
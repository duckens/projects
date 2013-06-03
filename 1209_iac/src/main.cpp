#include <QtGui>

#include "view/mainView.h"


int main(int argc, char *argv[]) {
    
	QApplication app(argc, argv);
	
	QTextCodec::setCodecForTr (QTextCodec::codecForName("Windows-1251"));
	//QTextCodec::setCodecForTr (QTextCodec::codecForName("Windows-1251"));

	mainView *view = new mainView;
	view->show();
	
    return app.exec();
}
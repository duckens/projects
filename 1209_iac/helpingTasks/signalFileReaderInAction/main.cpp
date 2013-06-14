#include <QtGui>

#include "mainWindow.h"


int main(int argc, char *argv[]) {
    
	QApplication app(argc, argv);
	
	QTextCodec::setCodecForTr (QTextCodec::codecForName("utf-8"));

	mainWindow *window = new mainWindow;
	window->show();
	
    return app.exec();
}
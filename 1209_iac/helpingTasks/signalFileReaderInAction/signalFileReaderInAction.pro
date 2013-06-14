# Linux (MINT14), dynamic link
qtAddLibrary(qwt)
TEMPLATE     = app
DEFINES    += QT_DLL
QT += svg

DEFINES		+= QWT_DLL
INCLUDEPATH	+= /usr/include/qwt
LIBS += -L/usr/lib -lqwt


CONFIG += console
DEFINES += QT_DEBUG
QMAKE_CXXFLAGS += -w


INCLUDEPATH += src
DESTDIR = release
OBJECTS_DIR = release/obj
MOC_DIR = release/moc
RCC_DIR = release/rcc
UI_DIR = release/ui


TARGET       = ../bin/signalFileReaderInAction

HEADERS = \
	mainWindow.h\
	../signalFileReader/signalFileReader.h\

	
FORMS += \
	mainWindow.ui\


SOURCES = \
	main.cpp\
	mainWindow.cpp\
	../signalFileReader/signalFileReader.cpp\

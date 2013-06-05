# Linux (MINT14), dynamic link
TEMPLATE     = app
DEFINES    += QT_DLL
QT += svg


INCLUDEPATH += .


CONFIG += console
QMAKE_CXXFLAGS += -w


INCLUDEPATH += src
DESTDIR = release
OBJECTS_DIR = release/obj
MOC_DIR = release/moc
RCC_DIR = release/rcc
UI_DIR = release/ui


TARGET       = ../bin/intFormat

HEADERS = \
	stringDecoder.h
	

SOURCES = \
	main.cpp\
	stringDecoder.cpp

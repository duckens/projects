# Linux (MINT14), dynamic link
TEMPLATE     = app
DEFINES    += QT_DLL


TARGET       = bin/testThreading

# FORMS += mainWindow.ui

HEADERS = \
	helloThread.h

	
SOURCES = \	
	main.cpp\
	helloThread.cpp

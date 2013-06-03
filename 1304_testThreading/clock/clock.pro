# Linux (MINT14), dynamic link
TEMPLATE     = app
DEFINES    += QT_DLL


TARGET       = bin/clock

# FORMS += mainWindow.ui

HEADERS = \
	clockThread.h

	
SOURCES = \	
	main.cpp\
	clockThread.cpp

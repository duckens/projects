# Linux (MINT14), dynamic link
qtAddLibrary(qwt)
TEMPLATE     = app
DEFINES    += QT_DLL QWT_DLL
QT += svg
INCLUDEPATH += /usr/include/qwt
#INCLUDEPATH += /usr/local/qwt-6.0.0/include
#INCLUDEPATH += /usr/local/qwt-6.0.0/lib
LIBS += -L/usr/lib -lqwt
#LIBS += /usr/local/qwt-6.0.0/lib/libqwt.so -lqwt


# Windows, static link:
#qtAddLibrary(qwt)
#LIBS += C:/Qt/qwt-6.0.2-static/lib/libqwt.a
#LIBS += C:/Qt/qwt-6.0.2-static/lib/libqwtd.a
##LIBS += C:/Qt/qwt-6.0.2-static/lib/libqwtmathml.a
#INCLUDEPATH += C:/Qt/qwt-6.0.2-static/include
#INCLUDEPATH += C:/Qt/qwt-6.0.2-static/lib
#TEMPLATE     = app
#CONFIG += static
#CONFIG += exceptions
#QMAKE_CXXFLAGS += -static
#DEFINES += PLATFORM_WINDOWS


CONFIG += console
DEFINES += QT_DEBUG
QMAKE_CXXFLAGS += -w


INCLUDEPATH += src
DESTDIR = release
OBJECTS_DIR = release/obj
MOC_DIR = release/moc
RCC_DIR = release/rcc
UI_DIR = release/ui


TARGET       = ../bin/iac

HEADERS = \
	src/view/mainView.h\
#	src/view/signalForm.h\
#	src/view/FFTSignalForm.h\
#	src/view/chooseCyclesForm.h\
#	src/view/calibratedAntForm.h\
	src/view/classes/scrollzoomer.h\
	src/view/classes/scrollbar.h\
	src/view/classes/FFTCycleMarker.h\
	src/view/classes/markerList.h\
#	src/model/modelThread.h\
	src/model/model.h\
	src/model/parser/configParser.h\
	src/model/parser/signalBinaryNachOtdelaParser.h\
	src/model/classes/calibrationProcess.h\
	src/model/classes/gidrophone.h\
	src/model/classes/inlineAntena.h\
	src/model/classes/position.h\
	src/model/classes/sourceCircleSequence.h\
	src/model/classes/sourceObject.h\
	src/model/classes/sourceSequence.h\
	src/model/classes/commonMath.h\
	src/model/misc/converters.h\
	src/containers/arrays.h\
	src/containers/modelAvaliableResources.h\
	src/containers/calibrationParameters.h\
	src/view/pixmaps/pixmaps.h\
	\
	src/model/alglib/alglibinternal.h\
	src/model/alglib/alglibmisc.h\
	src/model/alglib/ap.h\
	src/model/alglib/dataanalysis.h\
	src/model/alglib/diffequations.h\
	src/model/alglib/fasttransforms.h\
	src/model/alglib/integration.h\
	src/model/alglib/interpolation.h\
	src/model/alglib/linalg.h\
	src/model/alglib/optimization.h\
	src/model/alglib/solvers.h\
	src/model/alglib/specialfunctions.h\
	src/model/alglib/statistics.h\
	src/model/alglib/stdafx.h


	
FORMS += \
	src/ui/IACMainWindow.ui\
	src/ui/signalForm.ui\
	src/ui/fftsignalform.ui\
	src/ui/CalibratedAntForm.ui\
	src/ui/chooseCyclesForm.ui


SOURCES = \
	src/main.cpp\
	src/view/mainView.cpp\
#	src/view/signalForm.cpp\
#	src/view/FFTSignalForm.cpp\
#	src/view/chooseCyclesForm.cpp\
#	src/view/calibratedAntForm.cpp\
	src/view/classes/scrollzoomer.cpp\
	src/view/classes/scrollbar.cpp\
	src/view/classes/FFTCycleMarker.cpp\
	src/view/classes/markerList.cpp\
	src/model/model.cpp\
#	src/model/modelThread.cpp\
	src/model/parser/configParser.cpp\
	src/model/parser/signalBinaryNachOtdelaParser.cpp\
	src/model/classes/calibrationProcess.cpp\
	src/model/classes/gidrophone.cpp\
	src/model/classes/inlineAntena.cpp\
	src/model/classes/position.cpp\
	src/model/classes/sourceCircleSequence.cpp\
	src/model/classes/sourceObject.cpp\
	src/model/classes/sourceSequence.cpp\
	src/model/classes/commonMath.cpp\
	src/model/misc/converters.cpp\
	\
	src/model/alglib/alglibinternal.cpp\
	src/model/alglib/alglibmisc.cpp\
	src/model/alglib/ap.cpp\
	src/model/alglib/dataanalysis.cpp\
	src/model/alglib/diffequations.cpp\
	src/model/alglib/fasttransforms.cpp\
	src/model/alglib/integration.cpp\
	src/model/alglib/interpolation.cpp\
	src/model/alglib/linalg.cpp\
	src/model/alglib/optimization.cpp\
	src/model/alglib/solvers.cpp\
	src/model/alglib/specialfunctions.cpp\
	src/model/alglib/statistics.cpp


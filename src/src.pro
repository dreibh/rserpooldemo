TEMPLATE = app
TARGET = ../bin/rserpooldemo
INCLUDEPATH += .
SOURCES += canvas.cpp \
canvasnode.cpp \
canvasview.cpp \
configuration.cpp \
contextmenuconfig.cpp \
mainwidget.cpp \
networklistener.cpp \
node.cpp \
rserpooldemo.cpp
HEADERS += canvas.h \
canvasnode.h \
canvasview.h \
componentstatuspackets.h \
configuration.h \
contextmenuconfig.h \
mainwidget.h \
networklistener.h \
node.h
#The following line was inserted by qt3to4
QT += network xml  qt3support 

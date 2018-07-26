#-------------------------------------------------
#
# Project created by QtCreator 2017-01-09T12:40:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ARDOS2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qencoderphidgets.cpp \
    qstepperphidgets.cpp \
    qanalogphidgets.cpp

HEADERS  += mainwindow.h \
    qstepperphidgets.h \
    qencoderphidgets.h \
    qanalogphidgets.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lphidget21
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lphidget21
else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -lphidget21

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

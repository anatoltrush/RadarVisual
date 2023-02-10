TARGET  = CanVisual
CONFIG  += c++17
QT      += core gui widgets serialbus

QMAKE_TARGET_COPYRIGHT = Anatol Trush
QMAKE_TARGET_DESCRIPTION = For work with ARS 408-21

VERSION_MAJOR = 2
VERSION_MINOR = 0
VERSION_BUILD = 0

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

win32: {
RESOURCES +=
RC_FILE = myapp.rc

INCLUDEPATH += C:\ZeroMQ\include
LIBS += C:\ZeroMQ\bin\libzmq.dll

INCLUDEPATH += C:\cppzmq\include
}
unix:{
LIBS += -L/usr/local/lib \
        -lzmq
}

SOURCES += \
    converter.cpp \
    dialogconfig.cpp \
    displaydata.cpp \
    main.cpp \
    mainwindow.cpp \
    visimage.cpp \
    zmq_subscriber_modfd.cpp

HEADERS += \
    converter.h \
    dialogconfig.h \
    displaydata.h \
    entity.h \
    mainwindow.h \
    visimage.h \
    zmq_subscriber_modfd.hpp

FORMS += \
    dialogconfig.ui \
    displaydata.ui \
    mainwindow.ui

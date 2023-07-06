TARGET  = CanVisual
CONFIG  += c++17
QT      += core gui widgets

QMAKE_TARGET_COPYRIGHT = Anatol Trush
QMAKE_TARGET_DESCRIPTION = For work with ARS 408-21

VERSION_MAJOR = 4
VERSION_MINOR = 2
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

QT      += serialbus
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
    softversion.cpp \
    techlogger.cpp \
    visimage.cpp \
    zmq_client.cpp \
    zmq_subscriber_modfd.cpp

HEADERS += \
    converter.h \
    dialogconfig.h \
    displaydata.h \
    entity.h \
    mainwindow.h \
    softversion.h \
    techlogger.h \
    visimage.h \
    zmq_client.hpp \
    zmq_subscriber_modfd.hpp

FORMS += \
    dialogconfig.ui \
    displaydata.ui \
    mainwindow.ui \
    softversion.ui

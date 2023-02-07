TARGET  = CanVisual
CONFIG  += c++17
QT      += core gui widgets serialbus

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

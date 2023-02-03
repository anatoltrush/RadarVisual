TARGET  = CanVisual
CONFIG  += c++17
QT      += core gui widgets serialbus

win32: {
RESOURCES +=
RC_FILE = myapp.rc
}
unix:{
LIBS += -L/usr/local/lib \
        -lzmq
}

SOURCES += \
    converter.cpp \
    displaydata.cpp \
    main.cpp \
    mainwindow.cpp \
    visimage.cpp \
#    zmq_subscriber_modfd.cpp

HEADERS += \
    converter.h \
    displaydata.h \
    entity.h \
    mainwindow.h \
    visimage.h \
#    zmq_subscriber_modfd.hpp

FORMS += \
    displaydata.ui \
    mainwindow.ui

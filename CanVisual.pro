TARGET  = CanVisual
CONFIG  += c++14
QT      += core gui widgets serialbus charts

win32: {
RESOURCES += \
    res.qrc
RC_FILE = myapp.rc
}

SOURCES += \
    converter.cpp \
    displaydata.cpp \
    main.cpp \
    mainwindow.cpp \
    visimage.cpp

HEADERS += \
    converter.h \
    displaydata.h \
    entity.h \
    mainwindow.h \
    visimage.h

FORMS += \
    displaydata.ui \
    mainwindow.ui

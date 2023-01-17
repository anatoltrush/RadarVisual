TARGET  = CanVisual
CONFIG  += c++14
QT      += core gui widgets serialbus charts

win32: {
QT      += openglwidgets
LIBS    += -lglu32 -lopengl32
}

SOURCES += \
    converter.cpp \
    displaydata.cpp \
    main.cpp \
    mainwindow.cpp \
    visualgl.cpp

HEADERS += \
    converter.h \
    displaydata.h \
    entity.h \
    mainwindow.h \
    visualgl.h

FORMS += \
    displaydata.ui \
    mainwindow.ui

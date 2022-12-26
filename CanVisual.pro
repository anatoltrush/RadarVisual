TARGET  = CanVisual
CONFIG  += c++14
QT      += core gui widgets serialbus charts

win32: {
QT      += openglwidgets
LIBS    += -lglu32 -lopengl32
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    visualgl.cpp

HEADERS += \
    mainwindow.h \
    visualgl.h

FORMS += \
    mainwindow.ui

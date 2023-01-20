#ifndef ENTITY_H
#define ENTITY_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QTextStream>
#include <QScreen>

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <unistd.h>

#define RADAR_NUM   8

#define GET_CUR_TIME_MILLI (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
#define GET_CUR_TIME_MICRO (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())

struct CanLine{
    double timeStamp;
    QString canNum;
    QString messId;
    QString messData;
};

enum class ClusterDynProp{
    moving = 0,
    stationary = 1,
    oncoming = 2,
    stationary_candidate = 3,
    unknown = 4,
    crossing_stationary = 5,
    crossing_moving = 6,
    stopped = 7
};

struct ClusterInfo{
    uint8_t id              = 0;
    float distLong          = 0.0f;
    float distLat           = 0.0f;
    float vRelLong          = 0.0f;
    float vRelLat           = 0.0f;
    float RCS               = 0.0f;
    ClusterDynProp type     = ClusterDynProp::unknown;
    void toOpenGLCoords(){
        // TODO: void toOpenGLCoords() implement
    }
};

#endif // ENTITY_H

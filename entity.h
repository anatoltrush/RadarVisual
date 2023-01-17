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

#define GET_CUR_TIME_MILLI (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
#define GET_CUR_TIME_MICRO (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())

struct CanLine{
    double timeStamp;
    QString canNum;
    QString canId;
    QString canData;
};

struct CanFrame{
    CanLine header;
    std::vector<CanLine> clusters;
};

enum class Cluster_AmbigState{
    invalid = 0,
    ambiguous = 1,
    staggered_ramp = 2,
    unambiguous = 3,
    stationary_candidates = 4,
};

enum class Cluster_DynProp{
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
    float VRelLong          = 0.0f;
    float VRelLat           = 0.0f;
    Cluster_DynProp type    = Cluster_DynProp::unknown;
    char RCS                = 0x00;
};

struct ClusterQuality{
    uint8_t id              = 0;
    float distLong_rms      = 0.0f;
    float distLat_rms       = 0.0f;
    float VRelLong_rms      = 0.0f;
    float VRelLat_rms       = 0.0f;
    float areaLong_rms      = 0.0f;
    float areaLat_rms       = 0.0f;
    float obj_Orientation_rms       = 0.0f;
    Cluster_AmbigState ambigState   = Cluster_AmbigState::invalid;
    uint8_t invalidateState = 0;
    float Pdh0              = 0.0f;
};

#endif // ENTITY_H

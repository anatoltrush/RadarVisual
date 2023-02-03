#ifndef ENTITY_H
#define ENTITY_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QTextStream>
#include <QScreen>

#ifdef __WIN32
#else
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <math.h>

#define RADAR_NUM   8

#define GET_CUR_TIME_MILLI (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
#define GET_CUR_TIME_MICRO (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())

const size_t DATA_SIZE      = 256;

const float resRCS          = 0.5f;
const float resDistLong     = 0.2f;
const float resDistLat      = 0.2f;
const float resVRelLong     = 0.25f;
const float resVRelLat      = 0.25f;

const float resMaxDist      = 2.0f;

const float offsetRCS       = -64.0f;
const float offsetDistLong  = -500.0f;
const float offsetDistLat   = -102.3f;
const float offsetVRelLong  = -128.0f;
const float offsetVRelLat   = -64.0f;

enum class StatusSpeed{
    forward = 0,
    slowSpeed = 1,
    backward = 2
};

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
    stationaryCandidate = 3,
    unknown = 4,
    crossingStationary = 5,
    crossingMoving = 6,
    stopped = 7
};

struct ConfigInfo{
public:
    uint16_t nearZone       = 0;
    uint16_t getFarZone() const{return farZone;}
    void setFarZone(const uint16_t &value){
        farZone = value;
        nearZone = farZone / 2;
    }
private:
    uint16_t farZone        = 0;
};

struct ClusterInfo{
    uint8_t id              = 0;
    float distLong          = 0.0f;
    float distLat           = 0.0f;
    float vRelLong          = 0.0f;
    float vRelLat           = 0.0f;
    float RCS               = 0.0f;
    float Pdh0              = 0.0f;
    float azimuth           = 0.0f;
    ClusterDynProp type     = ClusterDynProp::unknown;
    void clacAzimuth(){azimuth = std::atan(distLat / distLong) * 180.0f / M_PI;}
};

#endif // ENTITY_H

#ifndef ENTITY_H
#define ENTITY_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QTextStream>
#include <QScreen>

#ifdef __WIN32
#include <QCanBus>
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
#define DATA_SIZE   256

#define GET_CUR_TIME_MILLI (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
#define GET_CUR_TIME_MICRO (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())

// --- clusters ---
const float resClustRCS         = 0.5f;
const float resClustDistLong    = 0.2f;
const float resClustDistLat     = 0.2f;
const float resClustVRelLong    = 0.25f;
const float resClustVRelLat     = 0.25f;

const float resConfRadMaxDist   = 2.0f;

const float offsetClustRCS      = -64.0f;
const float offsetClustDistLong = -500.0f;
const float offsetClustDistLat  = -102.3f;
const float offsetClustVRelLong = -128.0f;
const float offsetClustVRelLat  = -64.0f;

// --- objects --
const float resObjRCS           = 0.5f;
const float resObjDistLong      = 0.2f;
const float resObjDistLat       = 0.2f;
const float resObjVRelLong      = 0.25f;
const float resObjVRelLat       = 0.25f;

const float resObjAngle         = 0.4f;
const float resObjLenght        = 0.2f;
const float resObjWidth         = 0.2f;

const float offsetObjRCS        = -64.0f;
const float offsetObjDistLong   = -500.0f;
const float offsetObjDistLat    = -204.6f;
const float offsetObjVRelLong   = -128.0f;
const float offsetObjVRelLat    = -64.0f;

const float offsetObjAngle      = -180.0f;
const float offsetObjLenght     = 0.0f;
const float offsetObjWidth      = 0.0f;

// --- clusters/objects filterts --
const float resFiltDist     = 0.1f;
const float resFiltAzim     = 0.025f;
const float resFiltRcs      = 0.025f;
const float resFiltVROnc    = 0.0315f;
const float resFiltVRDep    = 0.0315f;
const float resFiltLTim     = 0.1f;
const float resFiltSize     = 0.025f;
const float resFiltX        = 0.2f;
const float resFiltY        = 0.2f;
const float resFiltVYRL     = 0.0315f;
const float resFiltVXOnc    = 0.0315f;
const float resFiltVYLR     = 0.0315f;
const float resFiltVXDep    = 0.0315f;

const float offsetFiltAzim  = -50.0f;
const float offsetFiltRcs   = -50.0f;
const float offsetFiltX     = -500.0f;
const float offsetFiltY     = -409.5f;

enum class MsgType : uint64_t {Undefined = 0, OpenCVImage = 1001, Detections = 1003, CANMsg = 1005,
                               RadarData = 1200, AutoExposeData = 1203, ObjectInfo = 1300};

struct VersionID{
    uint8_t major   = 255;
    uint8_t minor   = 0;
    uint8_t patch   = 0;
    uint8_t extended = 0;
    uint8_t country = 0;
};

enum class InUse{
    nothing = 0,
    can     = 1,
    zmq     = 2,
    file    = 3
};

struct MessageId{
    uint64_t _msg_src   = 0;
    uint64_t _msg_num   = 0;
    uint64_t _time      = 0;

    inline MessageId& operator = (const MessageId& id){
        _msg_src = id._msg_src;
        _msg_num = id._msg_num;
        _time = id._time;
        return *this;
    }
};

#ifdef __WIN32
typedef unsigned char __u8;
typedef unsigned int __u32;
typedef __u32 canid_t;
#define CANFD_MAX_DLEN 64
struct canfd_frame{
    canid_t can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    __u8    len;     /* frame payload length in byte */
    __u8    flags;   /* additional flags for CAN FD */
    __u8    __res0;  /* reserved / padding */
    __u8    __res1;  /* reserved / padding */
    __u8    data[CANFD_MAX_DLEN] __attribute__((aligned(8)));
};

typedef QPair<QCanBusDevice::ConfigurationKey, QVariant> ConfigurationItem;

struct Settings {
    QString pluginName;
    QString deviceInterfaceName;
    QList<ConfigurationItem> configurations;
    bool useConfigurationEnabled = false;
    int modelRingBufferSize = 1000;
    bool useAutoscroll = false;
};
#endif

struct ZmqCanMessage{
    MsgType     _msg_type = MsgType::CANMsg;
    MessageId   _id;
    canfd_frame _frame;
};

enum class StatusSpeed{
    forward     = 0,
    slowSpeed   = 1,
    backward    = 2
};

struct CanLine{
    double timeStamp;
    QString canNum;
    QString messId;
    QString messData;
};

struct ConfigRadar{
public:
    bool readStatus     = false;
    bool writeStatus    = false;
    bool persistErr     = false;
    bool interference   = false;
    bool temperatErr    = false;
    bool temporarErr    = false;
    bool voltErr        = false;
    bool relay          = false;
    bool sendQual       = false;
    bool sendExt        = false;
    bool thrRcs         = false;

    uint8_t canNum      = 0;
    uint8_t id          = 0;
    uint8_t power       = 0;
    uint8_t sortInd     = 0;
    uint8_t outputType  = 0;
    uint8_t motionRxState = 0;
    uint16_t nearZone   = 0;

    uint16_t getFarZone() const{return farZone;}
    void setFarZone(const uint16_t &value){
        farZone = value;
        nearZone = farZone / 2;
    }

private:
    uint16_t farZone        = 0;
};

struct ConfigClustObj{};

enum class DynProp{
    moving              = 0,
    stationary          = 1,
    oncoming            = 2,
    stationaryCandidate = 3,
    unknown             = 4,
    crossingStationary  = 5,
    crossingMoving      = 6,
    stopped             = 7
};

struct ClusterList{
    uint8_t numExpectNear   = 0;
    uint8_t numExpectFar    = 0;
    uint8_t numExpectSumm   = 0;
    uint8_t interfVers      = 0;
    uint16_t measCount      = 0;
};

struct ClusterInfo{
    uint8_t id      = 0;
    float distLong  = 0.0f;
    float distLat   = 0.0f;
    float vRelLong  = 0.0f;
    float vRelLat   = 0.0f;
    float RCS       = 0.0f;
    float Pdh0      = 0.0f;
    float azimuth   = 0.0f;
    DynProp type    = DynProp::unknown;
    void clacAzimuth(){azimuth = std::atan(distLat / distLong) * 180.0f / M_PI;}
};

enum class ObjectClass{
    point       = 0,
    car         = 1,
    truck       = 2,
    pedestrian  = 3,
    motorcycle  = 4,
    bicycle     = 5,
    wide        = 6,
    reserved    = 7
};

struct ObjectList{
    uint8_t numExpect   = 0;
    uint8_t interfVers  = 0;
    uint16_t measCount  = 0;
};

struct ObjectInfo{
    uint8_t id      = 0;
    float distLong  = 0.0f;
    float distLat   = 0.0f;
    float vRelLong  = 0.0f;
    float vRelLat   = 0.0f;
    float RCS       = 0.0f;
    float Pdh0      = 0.0f;
    float azimuth   = 0.0f;
    float width     = 0.0f;
    float length    = 0.0f;
    float angle     = 0.0f;
    DynProp type    = DynProp::unknown;
    ObjectClass objClass = ObjectClass::reserved;
    void clacAzimuth(){azimuth = std::atan(distLat / distLong) * 180.0f / M_PI;}
};

#endif // ENTITY_H

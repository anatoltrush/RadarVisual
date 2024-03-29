#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>

#include "softversion.h"
#include "displaydata.h"
#include "zmq_subscriber_modfd.hpp"
#include "techlogger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:    
    void slotStart();
    void slotAddDisplay();
    void slotTimerTick();

    void slotInputCAN();
#ifdef __WIN32
    int sendCanFrame(const QString& canStr);
    void pluginChanged(const QString &plugin);
    void interfaceChanged(const QString &interf);
#endif
    void slotInputZMQ();
    void slotInputFile();

    void slotLoadFile();
    void slotPlayFile();
    void slotStopFile();
    void slotMirroring(bool checked);
    void slotLogging(bool checked);

private:
    Ui::MainWindow *ui;
    SoftVersion* sVersion = nullptr;
    DisplayData* displays[RADAR_NUM];
    QString statusRadMess;
    bool isAppStopped = true;
    InUse inUse = InUse::nothing;

    void sendToDisplay(const CanLine &canLine);    

    // ----- ----- ----- INPUT FROM CAN ----- ----- -----
    int handle = 0;
#ifdef __WIN32
    QList<QCanBusDeviceInfo> interfaces;
    Settings canSets;
    std::unique_ptr<QCanBusDevice> canDevice;
#else
    struct sockaddr_can sockAddr;
    struct ifreq ifr;
#endif
    qint64 numFramesReceived = 0;
    std::string deviceName;
    uint16_t delayCanUs = 5000;
    std::thread thrCanRcv;
    bool isCanOpened = false;
#ifdef __WIN32
    bool connectDevice();
    void procErrors(QCanBusDevice::CanBusError) const;
    void procReceivedFrames();
#else
    bool openCan(const std::string &device);
#endif
    void canRcv();

    // ----- ----- ----- INPUT FROM ZMQ ----- ----- -----
    bool isSubscrStarted = false;
    uint16_t delayZmqUs = 3000;
    std::thread thrZmqRcv;    
    Subscriber_modfd zmqSubscriber;
    MessageId msgId;
    QString zmqAddrRcv;

    void zmqRcv();

    // ----- ----- ----- INPUT FROM FILE ----- ----- -----
    std::thread thrPlayFile;
    bool isFileLoaded = false;
    bool isPlay = false;
    QString pathFileCanLog;
    std::vector<CanLine> canLines;

    // ----- ----- ----- LOGGER ----- ----- -----
    std::thread thrLog;
    TechLogger tLogger;
    std::mutex mutLoc;
    std::condition_variable condVar;
    QTimer* timer = nullptr;
    uint8_t indicCount = 9;

    void fillCanLines(QFile &file, int linesAmount);
    void playCanFile();  
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "displaydata.h"
#include "zmq_subscriber_modfd.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:    
    void on_pBStart_clicked();
    void on_pBAddDisplay_clicked();

    void on_rBInpCAN_clicked();
    void on_rBInpZMQ_clicked();
    void on_rBInpFile_clicked();   

    void on_pBLoadFile_clicked();
    void on_pBPlayFile_clicked();
    void on_pBStopFile_clicked();

    void on_pBVersionID_clicked();

private:
    Ui::MainWindow *ui;
    DisplayData* displays[RADAR_NUM];
    QString statusRadMess;
    bool isAppStopped = true;

    void sendToDisplay(const CanLine &canLine);

    // ----- ----- ----- INPUT FROM CAN ----- ----- -----
    int handle = 0;
#ifdef __WIN32
#else
    struct sockaddr_can sockAddr;
    struct ifreq ifr;
#endif
    std::string deviceName;
    std::thread thrCanRcv;
    bool isCanOpened = false;
#ifdef __WIN32
#else
    bool openCan(const std::string &device);
    void canRcv();
#endif

    // ----- ----- ----- INPUT FROM ZMQ ----- ----- -----
    bool isZmqStarted = false;
    uint16_t delay_us = 3000;
    std::thread thrZmqRcv;    
    Subscriber_modfd subscriber;
    MessageId msgId;
    QString addressString;

    void zmqRcv();

    // ----- ----- ----- INPUT FROM FILE ----- ----- -----
    std::thread thrPlayFile;
    bool isFileLoaded = false;
    bool isPlay = false;
    QString pathFileCanLog;
    std::vector<CanLine> canLines;

    void fillCanLines(QFile &file, int linesAmount);
    void playCanFile();  
};
#endif // MAINWINDOW_H
// TODO: Send config through ZMQ
// TODO: Test Interference
// TODO: firmware Version

// TODO: CONFIG Clusters read/write (#202)
// BUG: 702 filling wrong
// TODO: Windows (Serial bus) + ZMQ (timeout)
// TODO: ? Objects

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QScreen>

#include <thread>
#include <iostream>
#include <fstream>
#include <unistd.h>

#ifdef __WIN32
#else
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#endif

#include "converter.h"
#include "displaydata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct CanLine{
    QString timeStamp;
    QString canNum;
    QString canId;
    QString canData;
};

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pBStartApply_clicked();

    void on_rBInpCAN_clicked();
    void on_rBInpZMQ_clicked();
    void on_rBInpFile_clicked();

    void on_pBAddDisplay_clicked();

private:
    Ui::MainWindow *ui;

    DisplayData* displays[RADAR_NUM];

    QString statRadMess;

    uint64_t msgNumCan = 0;
    uint64_t msgNumZmq = 0;
    uint64_t msgNumFile = 0;

    // --- Input from CAN ---
    int handle = 0;
#ifdef __WIN32
#else
    struct sockaddr_can sockAddr;
    struct ifreq ifr;
#endif
    std::string deviceName;
    std::thread thrRcv;
    bool isCanStopped = true;
    bool isCanOpened = false;

#ifdef __WIN32
#else
    bool openCan(const std::string &device);
    void canRcv();
#endif

    // --- Input from ZMQ ---

    // --- Input from File ---
    QString pathFileCanLog;
    int wordsCount(const std::string& fname);
    std::vector<CanLine> canLines;
    void fillCanLines(QFile &file, int linesAmount);
    void playCanFile();

    uint8_t* resData;
    uint8_t resDataLen = 0;

    Converter converter;
};
#endif // MAINWINDOW_H
// TODO: Add range in properties
// TODO: OverLap
// TODO: Play button in read from file
// TODO: Show OpenGL point
// TODO: Save aspect ratio OpenGL widget

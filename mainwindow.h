#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

#include <thread>
#include <iostream>
#include <fstream>

#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "converter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct CanLine{
    QString timeStamp;
    QString canNum;
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

    void on_cBInpRadNum_activated(int index){converter.currRadarNum = index;}

private:
    Ui::MainWindow *ui;

    QString statRadMess;

    uint64_t msgNumCan = 0;
    uint64_t msgNumZmq = 0;
    uint64_t msgNumFile = 0;

    // --- Input from CAN ---
    int handle = 0;
    struct sockaddr_can sockAddr;
    struct ifreq ifr;
    std::string deviceName;
    std::thread thrRcv;
    bool isCanStopped = true;
    bool isCanOpened = false;

    bool openCan(const std::string &device);
    void canRcv();

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

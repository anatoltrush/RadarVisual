#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <thread>
#include <iostream>

#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>

#include "filter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pBStartApply_clicked();

    void on_rBInpCAN_clicked();
    void on_rBInpZMQ_clicked();
    void on_rBInpFile_clicked();

    void on_cBInpRadNum_activated(int index);

private:
    Ui::MainWindow *ui;

    uint64_t msgNumCan = 0;
    uint64_t msgNumZmq = 0;
    uint64_t msgNumFile = 0;

    // --- Input from CAN ---
    int handle = 0;
    struct sockaddr_can sockAddr;
    struct ifreq ifr;
    std::string deviceName;
    //std::atomic<std::string> deviceName;
    std::thread thrRcv;
    bool isCanStopped = true;

    bool openCan(const QString &device);
    void canRcv();

    // --- Input from ZMQ ---

    // --- Input from File ---

    uint8_t* resData;
    uint8_t resDataLen = 0;

    Filter filter;
};
#endif // MAINWINDOW_H

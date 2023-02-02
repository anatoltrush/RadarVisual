#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "displaydata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:    
    void on_pBStartApply_clicked();
    void on_pBAddDisplay_clicked();

    void on_rBInpCAN_clicked();
    void on_rBInpZMQ_clicked();
    void on_rBInpFile_clicked();   

    void on_pBLoadFile_clicked();
    void on_pBPlayFile_clicked();
    void on_pBStopFile_clicked();

private:
    Ui::MainWindow *ui;
    DisplayData* displays[RADAR_NUM];
    QString statusRadMess;

    void sendToDisplay(const CanLine &canLine);

    // ----- ----- ----- INPUT FROM CAN ----- ----- -----
    int handle = 0;
#ifdef __WIN32
#else
    struct sockaddr_can sockAddr;
    struct ifreq ifr;
#endif
    std::string deviceName;
    std::thread thrRcv;
    std::thread thrPlayFile;
    bool isCanStopped = true;
    bool isCanOpened = false;

#ifdef __WIN32
#else
    bool openCan(const std::string &device);
    void canRcv();
#endif

    // ----- ----- ----- INPUT FROM ZMQ ----- ----- -----
    QString pathString;

    // ----- ----- ----- INPUT FROM FILE ----- ----- -----
    bool isFileLoaded = false;
    bool isPlay = false;
    QString pathFileCanLog;
    std::vector<CanLine> canLines;

    void fillCanLines(QFile &file, int linesAmount);
    void playCanFile();  
};
#endif // MAINWINDOW_H
// TODO: ZMQ input + convert + show
// TODO: Calc and show vehicle speed
// TODO: Draw zones
// TODO: ? Time chart
// BUG: 702 filling wrong

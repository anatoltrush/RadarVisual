#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QClipboard>

#include "entity.h"
#include "converter.h"
#include "zmq_client.hpp"

namespace Ui {
class DialogConfig;
}

class DialogConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConfig(QWidget *parent = nullptr);
    ~DialogConfig();

    ConfigRadar configRadar;
    CollDetState collDetState;
    std::vector<CollRegion> regions;
    std::vector<QColor>* colorsWarnLevel = nullptr;

    uint64_t prev402GotMs   = 0;
    uint64_t period402GotMs = 980; // every 1 sec
    uint64_t prev408GotMs   = 0;
    uint64_t period408GotMs = 1200;
    bool is408Got = false;

    InUse* inUse = nullptr;
    std::string deviceName;

    bool is203Got       = false;
    uint16_t fltClust   = 0;
    uint16_t fltObj     = 0;

    void updateConfigUI();
    void updateCollDetStateUI();

signals:
    int signalCanSend(const QString& strData);

private slots:
    void clearResString();
    void genRadConfigCommand();
    void genClObjConfigCommand();
    void genCollStateCommand();
    void genCollRegionCommand();
    void onPBSend();
    void sendOne();
    void sendMulti();
    void updRegListUI();
    void copyInClipBoard();

    void showHideSetRadQual(bool checked);
    void showHideSetRadExt(bool checked);
    void showHideSetRadThr(bool checked);
    void showHideSetRadStore(bool checked);
    void showHideSetRadRelay(bool checked);
    void showHideSetRadDist(bool checked);
    void showHideSetRadSort(bool checked);
    void showHideSetRadOut(bool checked);
    void showHideSetRadPow(bool checked);
    void showHideSetRadId(bool checked);

    void tabChanged(int index);
    void selectedUIClusters();
    void selectedUIObjects();
    void selectedUIRegion(QTableWidgetItem *item);

    void showHideSetClObjNofVal(bool checked); // Nof objects
    void showHideSetClObjNofAct(bool checked);
    void showHideSetClObjDistVal(bool checked); // Dist
    void showHideSetClObjDistAct(bool checked);
    void showHideSetClObjAzimVal(bool checked); // Azim
    void showHideSetClObjAzimAct(bool checked);
    void showHideSetClObjRcsVal(bool checked); // Rcs
    void showHideSetClObjRcsAct(bool checked);
    void showHideSetClObjVRelOncVal(bool checked); // VrelOnc
    void showHideSetClObjVRelOncAct(bool checked);
    void showHideSetClObjVRelDepVal(bool checked); // VrelDep
    void showHideSetClObjVRelDepAct(bool checked);
    void showHideSetClObjVYLRVal(bool checked); // VYLR
    void showHideSetClObjVYLRAct(bool checked);
    void showHideSetClObjVXOncVal(bool checked); // VXOnc
    void showHideSetClObjVXOncAct(bool checked);
    void showHideSetClObjVYRLVal(bool checked); // VYRL
    void showHideSetClObjVYRLAct(bool checked);
    void showHideSetClObjVXDepVal(bool checked); // VXDep
    void showHideSetClObjVXDepAct(bool checked);
    void showHideSetClObjLTimVal(bool checked); // LTime
    void showHideSetClObjLTimAct(bool checked);
    void showHideSetClObjSzVal(bool checked); // Size
    void showHideSetClObjSzAct(bool checked);
    void showHideSetClObjProbVal(bool checked); // Prob
    void showHideSetClObjProbAct(bool checked);
    void showHideSetClObjXVal(bool checked); // X
    void showHideSetClObjXAct(bool checked);
    void showHideSetClObjYVal(bool checked); // Y
    void showHideSetClObjYAct(bool checked);

    void showHideSetRegCoordinates(bool checked);
    void showHideSetCollDetectTime(bool checked);

private:
    Ui::DialogConfig *ui;
    QClipboard *clipboard = nullptr;

    uint8_t delaySendMsg = 50;

    // --- ZMQ ---
    uint64_t msg_num = 0;
    Client zmqClient;
    QString zmqAddrSend;
    CanLine zmqCanLine;
    std::vector<CanLine>zmqCanLines;
};

#endif // DIALOGCONFIG_H

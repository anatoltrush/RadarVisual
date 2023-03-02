#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QDialog>

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

    ConfigRadar* configRadar = nullptr;
    ConfigClustObj* configCluster = nullptr;

    InUse* inUse = nullptr;
    std::string deviceName;

    bool is203Got       = false;
    uint16_t fltClust   = 0;
    uint16_t fltObj     = 0;

    void updateUI();

private slots:
    void clearResStr();
    void genRadConfComm();
    void genClObjConfComm();
    void send();
    void sendOne();
    void sendMulti();

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
    void updateUIClusters();
    void updateUIObjects();

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
    // 4 V4
    void showHideSetClObjLTimVal(bool checked); // LTime
    void showHideSetClObjLTimAct(bool checked);
    void showHideSetClObjSzVal(bool checked); // Size
    void showHideSetClObjSzAct(bool checked);
    void showHideSetClObjProbVal(bool checked); // Prob
    void showHideSetClObjProbAct(bool checked);
    // 2 X Y

private:
    Ui::DialogConfig *ui;

    uint8_t delayCanMs = 50;

    // --- ZMQ ---
    uint64_t msg_num = 0;
    Client zmqClient;
    QString zmqAddrSend;
    CanLine zmqCanLine;
    std::vector<CanLine>zmqCanLines;
};

#endif // DIALOGCONFIG_H

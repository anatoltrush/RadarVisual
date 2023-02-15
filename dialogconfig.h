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
    ConfigCluster* configCluster = nullptr;

    InUse* inUse = nullptr;
    std::string deviceName;

    void updateUI();

private slots:
    void clearResStr();
    void generateCommand();
    void send();

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

private:
    Ui::DialogConfig *ui;

    // --- ZMQ ---
    uint64_t msg_num = 0;
    Client zmqClient;
    QString zmqAddrSend;
    CanLine sendLine;
};

#endif // DIALOGCONFIG_H

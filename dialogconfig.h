#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QDialog>

#include "entity.h"
#include "converter.h"

namespace Ui {
class DialogConfig;
}

class DialogConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConfig(QWidget *parent = nullptr);
    ~DialogConfig();

    ConfigRadar* configRadar;
    ConfigCluster* configCluster;

    void updateUI();

private slots:
    void on_pBClearResStr_clicked();
    void on_pBRadGenerate_clicked();
    void on_pBSend_clicked();

    void on_cBSetRadQual_clicked(bool checked);
    void on_cBSetRadExt_clicked(bool checked);
    void on_cBSetRadThr_clicked(bool checked);
    void on_cBSetRadStore_clicked(bool checked);
    void on_cBSetRadRelay_clicked(bool checked);
    void on_cBSetRadDist_clicked(bool checked);
    void on_cBSetRadSort_clicked(bool checked);
    void on_cBSetRadOut_clicked(bool checked);
    void on_cBSetRadPow_clicked(bool checked);
    void on_cBSetRadId_clicked(bool checked);

private:
    Ui::DialogConfig *ui;
};

#endif // DIALOGCONFIG_H

// TODO: timer for UI update

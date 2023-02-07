#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QDialog>

#include "entity.h"

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

private:
    Ui::DialogConfig *ui;
};

#endif // DIALOGCONFIG_H

// TODO: timer for UI update

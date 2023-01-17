#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "entity.h"

#define RADAR_NUM   8

namespace Ui {class DisplayData;}

class DisplayData : public QMainWindow{
    Q_OBJECT

public:
    explicit DisplayData(QWidget *parent = nullptr);
    ~DisplayData();

    CanFrame canFrame;

private:
    Ui::DisplayData *ui;
};

#endif // DISPLAYDATA_H

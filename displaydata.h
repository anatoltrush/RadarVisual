#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <QMainWindow>

#define RADAR_NUM   8

namespace Ui {class DisplayData;}

class DisplayData : public QMainWindow
{
    Q_OBJECT

public:
    explicit DisplayData(QWidget *parent = nullptr);
    ~DisplayData();

private:
    Ui::DisplayData *ui;
};

#endif // DISPLAYDATA_H

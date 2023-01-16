#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <QMainWindow>

#define RADAR_NUM   8

namespace Ui {class DisplayData;}

struct CanLine{
    QString timeStamp;
    QString canNum;
    QString canId;
    QString canData;
};

class DisplayData : public QMainWindow{
    Q_OBJECT

public:
    explicit DisplayData(QWidget *parent = nullptr);
    ~DisplayData();

    // from file
    std::vector<CanLine> canLines;

private:
    Ui::DisplayData *ui;
};

#endif // DISPLAYDATA_H

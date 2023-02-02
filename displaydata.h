#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "converter.h"

namespace Ui {class DisplayData;}

class DisplayData : public QMainWindow{
    Q_OBJECT

public:
    explicit DisplayData(QWidget *parent = nullptr);
    ~DisplayData();

    int selfCount = -1;

    uint8_t currRadInd = 0;

    std::vector<QColor> colors;

    std::vector<ClusterInfo> clustersAll;
    std::vector<ClusterInfo> clustersFiltered;

    void receiveCanLine(const CanLine &canLine);

private slots:
    void on_cBChsDist_currentTextChanged(const QString &data);
    void on_cmBRadNum_currentIndexChanged(int index);

    void on_cBInfo_clicked(bool checked);

private:
    Ui::DisplayData *ui;

    uint8_t numExpectNear = 0;
    uint8_t numExpectFar = 0;
    uint8_t numExpectSumm = 0;
    uint16_t measCount = 0;

    void applyFilters();
    void updateShowFlags();
};

#endif // DISPLAYDATA_H

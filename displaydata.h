#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "converter.h"

namespace Ui {class DisplayData;}

class DisplayData : public QMainWindow{
    Q_OBJECT

public:
    explicit DisplayData(QWidget *parent = nullptr);
    ~DisplayData();

    uint8_t currRadInd = 0;

    std::vector<QColor> colors;

    std::vector<ClusterInfo> clustersAll;
    std::vector<ClusterInfo> clustersFiltered;

    void receiveCanLine(CanLine* canLine);

private slots:
    void on_cBChsDist_currentTextChanged(const QString &data);
    void on_cmBRadNum_currentIndexChanged(int index);

    void on_cBInfo_clicked(bool checked);

private:
    Ui::DisplayData *ui;

    int numExpect = -1;

    void applyFilters();
    void updateShowFlags();
};

#endif // DISPLAYDATA_H

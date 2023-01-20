#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "entity.h"
#include "converter.h"

namespace Ui {class DisplayData;}

class DisplayData : public QMainWindow{
    Q_OBJECT

public:
    explicit DisplayData(QWidget *parent = nullptr);
    ~DisplayData();

    uint8_t currRadInd = 0;

    std::vector<ClusterInfo> clustersAll;
    std::vector<ClusterInfo> clustersFiltered;

    void receiveCanLine(CanLine* canLine);

private slots:
    void on_cmBRadNum_activated(int index);

private:
    Ui::DisplayData *ui;

    int numExpect = -1;

    void applyFilters();
};

#endif // DISPLAYDATA_H

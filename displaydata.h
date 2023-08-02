#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <QScrollBar>

#include "dialogconfig.h"
#include "visimage.h"

namespace Ui {class DisplayData;}

class DisplayData : public QMainWindow{
    Q_OBJECT

public:
    explicit DisplayData(QWidget *parent = nullptr);
    ~DisplayData();

    int selfCount = -1;
    VersionID* versID = nullptr;
    bool isMirrored = false;

    // --- conf window ---
    DialogConfig* dConfig = nullptr;
    void receiveCanLine(const CanLine &canLine);

private slots:
    void slotRadNum(int index);
    void slotInfo(bool checked);

    void slotConfigRadarCall();
    void slotUpdateWarningsUI();
    // --- visual ---
    void slotZoomChanged(int val);
    void slotSAHorizChanged(int val);
    void slotSAVertChanged(int val);    

signals:
    void signRadarWarningsUI();
    void signUpdRegionList();

private:
    Ui::DisplayData *ui;

    VisImage *vDraw = nullptr;
    const float aspect = 0.46f;
    QPointF saCentr = QPointF(0.5f, 0);
    uint8_t countStartResize = 0;

    void resizeEvent(QResizeEvent *event) override;

    // --- clusters ---
    ClusterList clustList;
    std::vector<ClusterInfo> clustersAll;
    std::vector<ClusterInfo> clustersFiltered;

    // --- objects ---
    ObjectList objList;
    std::vector<ObjectInfo> objectsAll;
    std::vector<ObjectInfo> objectsFiltered;

    void applyFilters();
    void updateShowFlags();

    // --- SPPED ---
    std::array<uint8_t, DATA_SIZE> histoVLong;
    std::array<ClusterInfo, DATA_SIZE> m_dataInfo;
    StatusSpeed statusSpeed = StatusSpeed::slowSpeed;
    float averSpeedKalman   = 0.0f;
    float averQuality       = 0.0f;
    float speedVehicle      = 0.0f;
    float speedQuality      = 0.0f;

    float koeffKalmanSpeed  = 0.1f; // 0.1...1.0 smooth...sharp
    float koeffKalmanQuality = 0.2f;
    int16_t zonaAnalyseQuality = 10;

    int calcSpeed();
    void showSpeedUI();
};

#endif // DISPLAYDATA_H

#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "dialogconfig.h"

namespace Ui {class DisplayData;}

class DisplayData : public QMainWindow{
    Q_OBJECT

public:
    explicit DisplayData(QWidget *parent = nullptr);
    ~DisplayData();

    int selfCount = -1;
    VersionID* versID = nullptr;

    // --- conf window ---
    DialogConfig* dConfig = nullptr;
    void receiveCanLine(const CanLine &canLine);

private slots:
    void chooseDist(const QString &data);
    void radNum(int index);
    void info(bool checked);

    // --- CONFIGS ---
    void configRadarCall();
    // ---
    void updateWarningsUI();

signals:
    void signalWarningsUI();

private:
    Ui::DisplayData *ui;
    // ---
    std::vector<QColor> colors;

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

    float koeffKalmanSpeed  = 0.5f;
    float koeffKalmanQuality = 0.2f;
    int16_t zonaAnalyseQuality = 10;

    int calcSpeed();
    void showSpeedUI();
};

#endif // DISPLAYDATA_H

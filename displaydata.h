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
    ConfigRadar configRadar;

    std::vector<QColor> colors;

    std::vector<ClusterInfo> clustersAll;
    std::vector<ClusterInfo> clustersFiltered;

    void receiveCanLine(const CanLine &canLine);

private slots:
    void on_cBChsDist_currentTextChanged(const QString &data);
    void on_cmBRadNum_currentIndexChanged(int index);

    void on_cBInfo_clicked(bool checked);

    // --- CONFIGS ---
    void on_pBConfigRadar_clicked();

private:
    Ui::DisplayData *ui;
    // ---
    DialogConfig* dConfig;
    // ---
    uint8_t numExpectNear   = 0;
    uint8_t numExpectFar    = 0;
    uint8_t numExpectSumm   = 0;
    uint16_t measCount      = 0;

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

    float koeffKalmanSpeed = 0.5f;
    float koeffKalmanQuality = 0.2f;
    int16_t zonaAnalyseQuality = 10;

    int calcSpeed();
    void showSpeedUI();
};

#endif // DISPLAYDATA_H

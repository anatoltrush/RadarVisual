#ifndef VISIMAGE_H
#define VISIMAGE_H

#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>

#include "entity.h"
#include "converter.h"

class VisImage : public QWidget{

public:
    VisImage(QWidget *parent = nullptr);

    bool isShowInfo = false;
    float aspect    = 0.0f;
    std::vector<QColor>* colors;

    ConfigRadar *configInfo = nullptr;

    ClusterList clustList;
    std::vector<ClusterInfo> clusters;
    std::vector<bool> properties;

    // --- objects ---
    ObjectList objList;
    std::vector<ObjectInfo> objects;

    void resizeAspect();

private:
    QPainter *painter;
    QPoint curs;

    // NOTE: Config grid
    int slicesOneSide   = 6; // parts
    int minLeftM        = -60; // meters
    int gridStepPx      = 0;
    float gridStepM     = 0.0f;

    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *event) override;

    void drawZones();
    void drawAxes();
    void drawRadar();    
    void drawClusters();
    void drawObjectsInfo();
    void drawObjectsExt();
    void drawCursor();
    int calcRad(float rcs);

protected:
    void mouseMoveEvent(QMouseEvent *event) override {curs = event->pos();};
};

#endif // VISIMAGE_H

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
    std::vector<QColor> colorsDynProp;
    std::vector<QColor> colorsWarnLevel = std::vector<QColor>(4);
    std::vector<bool> showProperties;

    ConfigRadar configRadar;
    std::vector<CollRegion>* regions = nullptr;

    QPoint sACls;
    QPoint sAObj;

    // --- clusters ---
    ClusterList clustList;
    std::vector<ClusterInfo> clusters;    

    // --- objects ---
    ObjectList objList;
    std::vector<ObjectInfo> objects;

private:
    QPainter *painter;
    QPoint curs;

    // NOTE: Config grid
    int slicesOneSide   = 6; // parts
    int minLeftM        = -60; // meters
    int gridStepPx      = 0;
    float gridStepM     = 0.0f;

    void paintEvent(QPaintEvent *) override;

    void drawZones();
    void drawRegions();
    void drawAxes();
    void drawRadar();    
    void drawClusters();
    void drawObjectsInfo();
    void drawObjectsExt();
    void drawCursor();
    int calcRadius(float rcs);

protected:
    void mouseMoveEvent(QMouseEvent *event) override {curs = event->pos();}
};

#endif // VISIMAGE_H

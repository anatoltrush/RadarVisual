#ifndef VISIMAGE_H
#define VISIMAGE_H

#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>

#include "entity.h"
#include "converter.h"

class VisImage : public QWidget
{
public:
    VisImage(QWidget *parent = nullptr);

    bool isShowInfo     = false;
    uint8_t numClNear   = 0;
    uint8_t numClFar    = 0;
    uint8_t numClSumm   = 0;
    uint16_t measCount  = 0;    
    float aspect        = 0.0f;

    ConfigInfo *configInfo = nullptr;

    std::vector<bool> props;
    std::vector<ClusterInfo> clusters;
    std::vector<QColor>* colors;

    void resizeAspect();

private:
    QPainter *painter;
    QPoint curs;

    // NOTE: config grid
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
    void drawCursor();
    int calcRad(float rcs);

protected:
    void mouseMoveEvent(QMouseEvent *event) override {curs = event->pos();};
};

#endif // VISIMAGE_H

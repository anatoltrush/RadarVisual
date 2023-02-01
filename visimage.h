#ifndef VISIMAGE_H
#define VISIMAGE_H

#include <QPainter>
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

    std::vector<bool> props;
    std::vector<ClusterInfo> clusters;
    std::vector<QColor>* colors;

    void resizeAspect();

private:
    QPainter *painter;
    QPoint curs;

    int stepPx = 0;
    int minLeftM = -50;
    float gridStepM = 0.0f;

    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *event) override;

    void drawAxes();
    void drawRadar();
    void drawCursor();
    void drawClusters();
    int calcRad(float rcs);

protected:
    void mouseMoveEvent(QMouseEvent *event) override {curs = event->pos();};
};

#endif // VISIMAGE_H

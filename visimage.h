#ifndef VISIMAGE_H
#define VISIMAGE_H

#include <QPainter>
#include <QResizeEvent>

#include "entity.h"

class VisImage : public QWidget
{
public:
    VisImage(QWidget *parent = nullptr);

    float aspect = 0.0f;

    std::vector<ClusterInfo> clusters;

    std::vector<QColor>* colors;

    void resizeAspect();

private:
    QPainter *painter;

    int stepPx = 0;

    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *event) override;

    void drawAxes();
    void drawRadar();
    void drawClusters();
};

#endif // VISIMAGE_H

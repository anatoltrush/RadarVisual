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

    void resizeAspect();

private:
    QPainter *painter;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *event) override;

    void drawAxes();
    void drawRadar();
    void drawClusters();
};

#endif // VISIMAGE_H

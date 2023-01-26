#include "visimage.h"

VisImage::VisImage(QWidget *parent) : QWidget(parent){
    this->setStyleSheet("background-color: #E6E6E6");
}

void VisImage::resizeAspect(){
    int hei = width() / aspect;
    QSize size(width(), hei);
    resize(size);
}

void VisImage::paintEvent(QPaintEvent *){
    painter = new QPainter(this);
    painter->eraseRect(rect());

    drawAxes();
    drawRadar();
    drawClusters();

    painter->end();
}

void VisImage::resizeEvent(QResizeEvent *event){
    QMainWindow* locParent = static_cast<QMainWindow*>(parent());
    int parHei = locParent->height();
    parHei -= 10;
    setMaximumHeight(parHei);

    int wid = event->size().width();
    int hei = wid / aspect;
    QSize size(wid, hei);
    resize(size);
}

void VisImage::drawAxes(){
    QPen penAx = QPen(Qt::black, 1);
    painter->setPen(penAx);
    painter->drawLine(QLine(0, height() - 1, width(), height() - 1));
    painter->drawLine(QLine(width() / 2, 0, width() / 2, height()));

    // --- grid ---
    int slices = 10;
    stepPx = width() / slices;
    if(stepPx <= 0) return;

    QPen penGrid = QPen(Qt::gray, 1);
    qreal crossSize = 6;
    QVector<qreal> dashes {crossSize, static_cast<double>(stepPx - crossSize)};
    penGrid.setDashPattern(dashes);
    painter->setPen(penGrid);
    // vert
    for (int i = 0; i < slices + 1; i++) {
        int xCoord = i * stepPx;
        painter->drawLine(QLine(xCoord, 0+(crossSize/2), xCoord, height()));
        // ---m---
        painter->drawText(QPoint(xCoord, height()-2), QString::number(-50 + i * slices)+"m");
    }
    // horiz
    for (int i = 0; i < height() / stepPx + 1; i++) {
        int yCoord = height() - i * stepPx;
        painter->drawLine(QLine(0-(crossSize/2), yCoord, width(), yCoord));
        // ---m---
        painter->drawText(QPoint(width()/2, yCoord), QString::number(i * slices)+"m");
    }
}

void VisImage::drawRadar(){
    QPen penRad = QPen(Qt::black, 1);
    painter->setPen(penRad);
    painter->setBrush(Qt::darkGray);

    if(stepPx <= 0) return;
    int wRad = stepPx * 0.5f;
    int hRad = stepPx * 0.15f;
    painter->drawRect(width()/2 - wRad/2, height() - hRad, wRad, hRad);
}

void VisImage::drawClusters(){
    // draw points
    QPen penClust = QPen(Qt::black, 1);
    painter->setPen(penClust);

    if(stepPx <= 0) return;
    for (const auto& cl : clusters) {
        QColor currCol = (*colors)[static_cast<uint8_t>(cl.type)];
        painter->setBrush(currCol);
        int wCl = width()/2 + (cl.distLat / 10.0f * stepPx);
        int hCl = height() - cl.distLong / 10.0f * stepPx;
        painter->drawEllipse(QPoint(wCl, hCl), 5, 5);
    }

    // draw text
}

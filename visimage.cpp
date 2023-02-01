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
    drawCursor();

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
    constexpr int slices = 5 * 2/*sides*/;
    gridStepM = (minLeftM * 2 * -1) / (float)slices;
    stepPx = width() / slices;
    if(stepPx <= 0) return;

    QPen penGrid = QPen(Qt::gray, 1);
    qreal crossSize = 6;
    QVector<qreal> dashes {crossSize, static_cast<double>(stepPx - crossSize)};
    penGrid.setDashPattern(dashes);
    painter->setPen(penGrid);
    // vert lines
    for (int i = 0; i < slices + 1; i++) {
        int xCoord = i * stepPx;
        painter->drawLine(QLine(xCoord, 0 + (crossSize/2), xCoord, height()));
        // ---m---
        painter->drawText(QPoint(xCoord-3, height()-2), QString::number(minLeftM + i * gridStepM) + "m");
    }
    // horiz lines
    for (int i = 0; i < height() / stepPx + 1; i++) {
        int yCoord = height() - i * stepPx;
        painter->drawLine(QLine(-crossSize/2, yCoord, width(), yCoord));
        // ---m---
        painter->drawText(QPoint(width()/2, yCoord), QString::number(i * gridStepM) + "m");
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

void VisImage::drawCursor(){
    QPen penCursor = QPen(Qt::darkGray, 1);
    painter->setPen(penCursor);
    int curX = (curs.x() - width()/2) / (float)stepPx * gridStepM;
    int curY = (height() - curs.y()) / (float)stepPx * gridStepM;
    painter->drawText(curs.x(), curs.y(), "x:" + QString::number(curX) + "|y:" + QString::number(curY));
}

void VisImage::drawClusters(){
    QPen penClust = QPen(Qt::black, 1);
    painter->setPen(penClust);

    if(stepPx <= 0) return;
    for (const auto& cl : clusters) {
        QColor currCol = (*colors)[static_cast<uint8_t>(cl.type)];
        painter->setBrush(currCol);
        int wCl = width()/2 + (cl.distLat / gridStepM * stepPx);
        int hCl = height() - cl.distLong / gridStepM * stepPx;
        int radius = calcRad(cl.RCS);
        painter->drawEllipse(QPoint(wCl, hCl), radius, radius);
        // --- text ---
        if(isShowInfo){
            QString textInfo;
            if(props[0]) textInfo = Converter::floatCutOff(cl.RCS, 1);
            if(props[1]) textInfo = Converter::floatCutOff(cl.distLong, 1);
            if(props[2]) textInfo = Converter::floatCutOff(cl.distLat, 1);
            if(props[3]) textInfo = Converter::floatCutOff(cl.vRelLong, 1);
            if(props[4]) textInfo = Converter::floatCutOff(cl.vRelLat, 1);
            if(props[5]) textInfo = Converter::floatCutOff(cl.Pdh0, 1);
            if(props[6]) textInfo = Converter::floatCutOff(cl.azimuth, 1);
            painter->drawText(wCl+2, hCl-2, textInfo);
        }
    }

    painter->drawText(0, 12, "Measure count: " + QString::number(measCount));
    painter->drawText(0, 26, "Clusters in frame (all): " + QString::number(numClSumm));
    painter->drawText(0, 40, "Clusters in frame (filtered): " + QString::number(clusters.size()));
    painter->drawText(0, 54, "Far zone: " + QString::number(numClFar));
    painter->drawText(0, 68, "Near zone: " + QString::number(numClNear));
}

int VisImage::calcRad(float rcs){
    int res = (int)((rcs - offsetRCS) / gridStepM);
    if(res <= 0) return 1;
    return res;
}

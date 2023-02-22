#include "visimage.h"

VisImage::VisImage(QWidget *parent) : QWidget(parent){
    this->setStyleSheet("background-color: #F5F5F5"); // start color
}

void VisImage::resizeAspect(){
    int hei = width() / aspect;
    QSize size(width(), hei);
    resize(size);
}

void VisImage::paintEvent(QPaintEvent *){
    painter = new QPainter(this);
    painter->eraseRect(rect());

    drawZones();
    drawAxes();
    drawRadar();    
    drawClusters();
    //drawObjectsInfo(); // NOTE: other drawing
    drawObjectsExt();
    drawCursor();

    painter->end();
}

void VisImage::resizeEvent(QResizeEvent *event){
    QMainWindow* locParent = static_cast<QMainWindow*>(parent());
    int parentHeight = locParent->height();
    parentHeight -= 10;
    setMaximumHeight(parentHeight);

    int wid = event->size().width();
    int hei = wid / aspect;
    QSize size(wid, hei);
    resize(size);
}

void VisImage::drawZones(){
    // --- red ---
    int farRadius = (configInfo->getFarZone() / (float)gridStepM) * gridStepPx;
    QPainterPath farPath;
    farPath.moveTo(width() / 2, height());
    QRectF rectFar(width() / 2 - farRadius, height() - farRadius, farRadius * 2, farRadius * 2);
    farPath.arcTo(rectFar, 81, 18);

    QColor lightRed(255, 200, 200);
    QPen penZoneRed = QPen(lightRed, 1);
    painter->setPen(penZoneRed);
    painter->setBrush(lightRed);
    painter->drawPath(farPath);

    // --- blue ---
    int nearRadius = (configInfo->nearZone / (float)gridStepM) * gridStepPx;
    QPainterPath nearPath;
    nearPath.moveTo(width() / 2, height());
    QRectF rectNear(width() / 2 - nearRadius, height() - nearRadius, nearRadius * 2, nearRadius * 2);
    nearPath.arcTo(rectNear, 30, 120);

    QColor lightBlue(160, 210, 255);
    QPen penZoneBlue = QPen(lightBlue, 1);
    painter->setPen(penZoneBlue);
    painter->setBrush(lightBlue);
    painter->drawPath(nearPath);
}

void VisImage::drawAxes(){
    QPen penAx = QPen(Qt::black, 1);
    painter->setPen(penAx);
    painter->drawLine(QLine(0, height() - 1, width(), height() - 1));
    painter->drawLine(QLine(width() / 2, 0, width() / 2, height()));

    // --- grid ---
    const uint8_t slicesTwoSides = slicesOneSide * 2/*sides*/;
    gridStepM = (minLeftM * 2 * -1) / (float)slicesTwoSides;
    gridStepPx = width() / slicesTwoSides;
    if(gridStepPx <= 0) return;

    QPen penGrid = QPen(Qt::gray, 1);
    qreal crossSize = 6;
    QVector<qreal> dashes {crossSize, static_cast<double>(gridStepPx - crossSize)};
    penGrid.setDashPattern(dashes);
    painter->setPen(penGrid);
    // --- vert lines ---
    for (uint8_t i = 0; i < slicesTwoSides + 1; i++) {
        int xCoord = i * gridStepPx;
        painter->drawLine(QLine(xCoord, 0 + (crossSize/2), xCoord, height()));
        // ---m---
        painter->drawText(QPoint(xCoord-3, height()-2), QString::number(minLeftM + i * gridStepM) + "m");
    }
    // --- horiz lines ---
    for (int i = 0; i < height() / gridStepPx + 1; i++) {
        int yCoord = height() - i * gridStepPx;
        painter->drawLine(QLine(-crossSize/2, yCoord, width(), yCoord));
        // ---m---
        painter->drawText(QPoint(width()/2, yCoord), QString::number(i * gridStepM) + "m");
    }
}

void VisImage::drawRadar(){
    QPen penRad = QPen(Qt::black, 1);
    painter->setPen(penRad);
    painter->setBrush(Qt::darkGray);

    if(gridStepPx <= 0) return;
    int wRad = gridStepPx * 0.5f;
    int hRad = gridStepPx * 0.15f;
    painter->drawRect(width()/2 - wRad/2, height() - hRad, wRad, hRad);
}

void VisImage::drawClusters(){
    QPen penClust = QPen(Qt::black, 1);
    painter->setPen(penClust);
    if(gridStepPx <= 0) return;

    for (const auto& cl : clusters) {
        QColor currCol = (*colors)[static_cast<uint8_t>(cl.type)];
        painter->setBrush(currCol);
        int wCl = width()/2 + cl.distLat / (float)gridStepM * gridStepPx;
        int hCl = height() - cl.distLong / (float)gridStepM * gridStepPx;
        int radius = calcRad(cl.RCS);
        painter->drawEllipse(QPoint(wCl, hCl), radius, radius);
        // --- text ---
        if(isShowInfo){
            QString textInfo;
            if(properties[0]) textInfo = Converter::floatCutOff(cl.RCS, 1);
            if(properties[1]) textInfo = Converter::floatCutOff(cl.distLong, 1);
            if(properties[2]) textInfo = Converter::floatCutOff(cl.distLat, 1);
            if(properties[3]) textInfo = Converter::floatCutOff(cl.vRelLong, 1);
            if(properties[4]) textInfo = Converter::floatCutOff(cl.vRelLat, 1);
            if(properties[5]) textInfo = Converter::floatCutOff(cl.Pdh0, 1);
            if(properties[6]) textInfo = Converter::floatCutOff(cl.azimuth, 1);
            painter->drawText(wCl+2, hCl-2, textInfo);
        }
    }
    painter->drawText(1, 12, "<---Clusters--->");
    painter->drawText(1, 26, "Measure count: " + QString::number(clustList.measCount));
    painter->drawText(1, 40, "Clusters in frame (filtered): " + QString::number(clusters.size()));
    painter->drawText(1, 54, "Clusters in frame (all): " + QString::number(clustList.numExpectSumm));
    painter->drawText(1, 68, "Far zone (" + QString::number(configInfo->getFarZone()) + "m): " + QString::number(clustList.numExpectFar));
    painter->drawText(1, 82, "Near zone (" + QString::number(configInfo->nearZone) + "m): " + QString::number(clustList.numExpectNear));
    // ---
    clustList = ClusterList();
    clusters.clear();
}

void VisImage::drawObjectsInfo(){
    QPen penObjInfo = QPen(Qt::black, 1);
    painter->setPen(penObjInfo);
    if(gridStepPx <= 0) return;

    for (const auto& obj : objects) {
        QColor colObjInfo = (*colors)[static_cast<uint8_t>(obj.type)];
        painter->setBrush(colObjInfo);
        int wObj = width()/2 + obj.distLat / (float)gridStepM * gridStepPx;
        int hObj = height() - obj.distLong / (float)gridStepM * gridStepPx;
        int side = calcRad(obj.RCS);
        painter->drawRect(wObj-side/2, hObj-side/2, side, side);
        // --- text ---
        if(isShowInfo){
            QString textInfo;
            if(properties[0]) textInfo = Converter::floatCutOff(obj.RCS, 1);
            if(properties[1]) textInfo = Converter::floatCutOff(obj.distLong, 1);
            if(properties[2]) textInfo = Converter::floatCutOff(obj.distLat, 1);
            if(properties[3]) textInfo = Converter::floatCutOff(obj.vRelLong, 1);
            if(properties[4]) textInfo = Converter::floatCutOff(obj.vRelLat, 1);
            if(properties[5]) textInfo = Converter::floatCutOff(obj.Pdh0, 1);
            if(properties[6]) textInfo = Converter::floatCutOff(obj.azimuth, 1);
            painter->drawText(wObj+2, hObj-2, textInfo);
        }
    }
    painter->drawText(width()/2+1, 12, "<---Objects--->");
    painter->drawText(width()/2+1, 26, "Measure count: " + QString::number(objList.measCount));
    painter->drawText(width()/2+1, 40, "Objects in frame (filtered): " + QString::number(objects.size()));
    painter->drawText(width()/2+1, 54, "Objects in frame (all): " + QString::number(objList.numExpect));
}

void VisImage::drawObjectsExt(){
    if(gridStepPx <= 0) return;
    painter->setBrush(QBrush());

    for (const auto& obj : objects) {
        QColor colObjExt = (*colors)[static_cast<uint8_t>(obj.type)];
        painter->setPen(QPen(colObjExt, 2));
        int xObj = width()/2 + obj.distLat / (float)gridStepM * gridStepPx;
        int yObj = height() - obj.distLong / (float)gridStepM * gridStepPx;
        int wPx = obj.width / (float)gridStepM * gridStepPx;
        if(wPx < 1) wPx = 1;
        int lPx = obj.length / (float)gridStepM * gridStepPx;
        if(lPx < 1) lPx = 1;
        painter->translate(xObj, yObj);
        painter->rotate(obj.angle);
        painter->drawRect(-wPx/2, -lPx, wPx, lPx);
        painter->resetTransform();
        // --- text ---
        QPen penObjInfo = QPen(Qt::black, 1);
        painter->setPen(penObjInfo);
        if(isShowInfo){
            QString textInfo;
            if(properties[0]) textInfo = Converter::floatCutOff(obj.RCS, 1);
            if(properties[1]) textInfo = Converter::floatCutOff(obj.distLong, 1);
            if(properties[2]) textInfo = Converter::floatCutOff(obj.distLat, 1);
            if(properties[3]) textInfo = Converter::floatCutOff(obj.vRelLong, 1);
            if(properties[4]) textInfo = Converter::floatCutOff(obj.vRelLat, 1);
            if(properties[5]) textInfo = Converter::floatCutOff(obj.Pdh0, 1);
            if(properties[6]) textInfo = Converter::floatCutOff(obj.azimuth, 1);
            painter->drawText(xObj+2, yObj-2, textInfo);
        }
    }
    painter->drawText(width()/2+1, 12, "<---Objects--->");
    painter->drawText(width()/2+1, 26, "Measure count: " + QString::number(objList.measCount));
    painter->drawText(width()/2+1, 40, "Objects in frame (filtered): " + QString::number(objects.size()));
    painter->drawText(width()/2+1, 54, "Objects in frame (all): " + QString::number(objList.numExpect));
    // ---
    objList = ObjectList();
    objects.clear();
}

void VisImage::drawCursor(){
    QPen penCursor = QPen(Qt::darkGray, 1);
    painter->setPen(penCursor);
    int curX = (curs.x() - width()/2) / (float)gridStepPx * gridStepM;
    int curY = (height() - curs.y()) / (float)gridStepPx * gridStepM;
    float diag = curX*curX + curY*curY;
    QString diagStr = Converter::floatCutOff(std::sqrt(diag), 1);
    float az = std::atan(curX / (float)curY) * 180.0f / M_PI;
    QString azStr = Converter::floatCutOff(az, 1);
    painter->drawText(curs.x(), curs.y(), "x:" + QString::number(curX) + "m|y:" + QString::number(curY) + "m");
    painter->drawText(curs.x(), curs.y() + 14, "d:" + diagStr + "m|az:" + azStr+"d");
}

int VisImage::calcRad(float rcs){
    int res = (int)((rcs - offsetClustRCS) / (float)gridStepM);
    if(res <= 0) return 1;
    return res;
}

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
    int wid = event->size().width();
    int hei = wid / aspect;
    QSize size(wid, hei);
    resize(size);
}

void VisImage::drawAxes(){
    QPen pen = QPen(Qt::black);
    pen.setWidth(1);
    painter->setPen(pen);
    int vertGap = 13;
    painter->drawLine(QLine(0, height() - vertGap, width(), height() - vertGap));
    painter->drawLine(QLine(width() / 2, 0, width() / 2, height()));
    painter->drawText(QPoint(width() / 2 + 2, height()), "0m");
}

void VisImage::drawRadar(){

}

void VisImage::drawClusters(){
    // draw points
    // draw text
}

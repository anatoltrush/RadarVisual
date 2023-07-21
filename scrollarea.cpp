#include "scrollarea.h"

ScrollArea::ScrollArea(QWidget *parent) : QScrollArea(parent){}

void ScrollArea::mouseMoveEvent(QMouseEvent *event){
    *curs = event->pos();
}

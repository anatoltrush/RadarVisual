#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QScrollArea>
#include <QMouseEvent>

class ScrollArea : public QScrollArea
{
public:
    ScrollArea(QWidget *parent = nullptr);

    QPoint* curs = nullptr;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

};

#endif // SCROLLAREA_H

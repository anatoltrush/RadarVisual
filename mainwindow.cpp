#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    /*QPainter painter(this);
    QColor coordLineColor(255, 0, 0, 255);

    QPen apen = QPen(coordLineColor);
    apen.setWidth(5);
    painter.setPen(apen);

    painter.drawLine(QLine(0,0,300,0));
    painter.drawLine(QLine(0,0,0,300));

    painter.drawText(QPoint(5,13), "0,0");
    painter.drawText(QPoint(280,13), "300");
    painter.drawText(QPoint(5,295), "300");*/
}


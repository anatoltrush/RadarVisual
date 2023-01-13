#include "displaydata.h"
#include "ui_displaydata.h"

DisplayData::DisplayData(QWidget *parent) : QMainWindow(parent), ui(new Ui::DisplayData){
    ui->setupUi(this);

    for(int i = 0; i < RADAR_NUM; i++)
        ui->cBRadNum->addItem("Radar " + QString::number(i));
}

DisplayData::~DisplayData(){
    delete ui;
}

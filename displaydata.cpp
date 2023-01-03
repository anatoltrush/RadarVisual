#include "displaydata.h"
#include "ui_displaydata.h"

DisplayData::DisplayData(QWidget *parent) : QDialog(parent), ui(new Ui::DisplayData){
    ui->setupUi(this);
}

DisplayData::~DisplayData(){
    delete ui;
}

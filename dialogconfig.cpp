#include "dialogconfig.h"
#include "ui_dialogconfig.h"

DialogConfig::DialogConfig(QWidget *parent): QDialog(parent), ui(new Ui::DialogConfig){
    ui->setupUi(this);
}

DialogConfig::~DialogConfig(){
    delete ui;
}

void DialogConfig::updateUI(){
    // --- write ---
    configRadar->writeStatus ? ui->rBCurrWriteS->setChecked(true) : ui->rBCurrWriteF->setChecked(true);

    // --- read ---
    configRadar->readStatus ? ui->rBCurrReadS->setChecked(true) : ui->rBCurrReadF->setChecked(true);

    // --- dist ---
    ui->lECurrRadDist->setText(QString::number(configRadar->getFarZone()));

    // --- pers error ---
    configRadar->persistErr ? ui->rBCurrPersErrAct->setChecked(true) : ui->rBCurrPersErrNo->setChecked(true);

    // ---  ---

    // ---  ---

    // ---  ---

    // ---  ---

    // ---  ---
}
